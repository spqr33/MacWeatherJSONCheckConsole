/* 
 * File:   RecvBysocketQueue.cpp
 * Author: s
 * 
 * Created on June 19, 2015, 7:56 PM
 */

#include "RecvBysocketQueue.h"
#include "JumboBuff.h"
#include "HTTPResponse.h"
#include <vector>
#include <unistd.h> //close()
#include "Action.h"
#include "SaveFile.h"

LobKo::RecvBySocketQueue::RecvBySocketQueue(QueuesMaster* qMaster) : qMaster_(*qMaster) {
    assert(qMaster && "SendBySocketQueue(): QueuesMaster is zero");
}

LobKo::RecvBySocketQueue::~RecvBySocketQueue() {
    ;
}

void LobKo::RecvBySocketQueue::add(int socketFD, shared_ptr<HTTPResponse> HTTPResponse) {
    map_[socketFD].push(HTTPResponse);
};

void LobKo::RecvBySocketQueue::process() {
    if ( map_.empty() ) {
        return;
    } else {
        typedef map<int, queue<shared_ptr<HTTPResponse> > >::iterator MapIterator;
        MapIterator iterMap = map_.begin();
        MapIterator iterMapEnd = map_.end();
        vector<MapIterator> vecEraseItersAtEnd;

        for (; iterMap != iterMapEnd; ++iterMap ) {
            queue<shared_ptr<HTTPResponse> >& q = iterMap->second;

            if ( q.empty() ) {
                vecEraseItersAtEnd.push_back(iterMap);
            } else {
                int socketFD = iterMap->first;
                shared_ptr<HTTPResponse> spHTTPResp = q.front();

                if ( spHTTPResp->getJumboBuff().use_count() == 0 ) {
                    int buffSize = getSystemRCVBUF_Size(socketFD);
                    shared_ptr<JumboBuff> spJumboBuff(new JumboBuff(buffSize));

                    spHTTPResp->setJumboBuff(spJumboBuff);
                }

                int readBytesfromSock;
                const char* const buff = spHTTPResp->getJumboBuff()->watermark_;

                int canReadBytes = spHTTPResp->getJumboBuff()->start_ + spHTTPResp->getJumboBuff()->buffSize_ - buff;
                if ( canReadBytes == 0 ) {
                    //Buffer is full. It is necessary to process it or reinit. **
                };

                readBytesfromSock = this->readFromSocket(socketFD, buff, canReadBytes);

                if ( readBytesfromSock < 0 ) {
#ifndef NDEBUG
                    std::cerr << "Reading error from socket occured. ***" << std::endl;
#endif
                } else if ( readBytesfromSock == 0 ) {
#ifndef NDEBUG
                    std::cerr << "Connection was closed by remote side. ****" << std::endl;
#endif
                } else { // reading successful
                    spHTTPResp->getJumboBuff()->watermark_ += readBytesfromSock;
                    //chose state
                    if ( spHTTPResp->get_parse_state() == HTTPResponse::parse_states::state_response_line ) {
                        HTTPResponse::parseStatus status;

                        status = spHTTPResp->parseResponseLine();
                        //PARSE_SUCCESS, PARSE_BUFFER_END_REACHED, PARSE_WATERMARK_REACHED,
                        //PARSE_PROTO_ERROR, PARSE_CODE_ERROR, PARSE_REASON_STR_ERROR,
                        //PARSE_NOT_CORRESPOND_STATE, PARSE_HEADER_ERROR, PARSE_ALL_HEADERS_SUCCESS

                        if ( status == HTTPResponse::PARSE_SUCCESS ) {
#ifndef NDEBUG
                            std::cout << spHTTPResp->get_proto() << " "
                                    << spHTTPResp->get_status_code() << " "
                                    << spHTTPResp->get_reason_phrase()
                                    << std::endl;
#endif
                        } else if ( status == HTTPResponse::PARSE_WATERMARK_REACHED ) {
                            continue;
                        } else if ( status == HTTPResponse::PARSE_BUFFER_END_REACHED ) {
                            spHTTPResp->getJumboBuff()->reInit();
                            continue;
                        }
                        if ( status == HTTPResponse::PARSE_PROTO_ERROR ||
                                status == HTTPResponse::PARSE_CODE_ERROR ||
                                status == HTTPResponse::PARSE_REASON_STR_ERROR ||
                                status == HTTPResponse::PARSE_NOT_CORRESPOND_STATE ) {
                            // error while parsing occured  *
#ifndef NDEBUG
                            std::cout << "Error while response line parsing occured" << std::endl;

#endif                          
                            qMaster_.reqErrorsQ()->add(spHTTPResp->getLinkedHTTPRequest());
                            q.pop();
                        }
                    } // end if ( spHTTPResp->get_parse_state() == HTTPResponse::parse_states::state_response_line ) {
                    if ( spHTTPResp->get_parse_state() == HTTPResponse::state_response_line_finished ||
                            spHTTPResp->get_parse_state() == HTTPResponse::state_headers ) {

                        HTTPResponse::parseStatus status;
                        bool parseHeadersExit_Flag = true;

                        while (parseHeadersExit_Flag) {
                            status = spHTTPResp->parseHeaderLine();

                            if ( status == HTTPResponse::PARSE_ALL_HEADERS_SUCCESS ) {
                                break;
                            } else if ( status == HTTPResponse::PARSE_SUCCESS ) {
                                spHTTPResp->assignKnownHeader(spHTTPResp->get_curr_name(), spHTTPResp->get_curr_value());
#ifndef NDEBUG
                                //std::cout << spHTTPResp->get_curr_name() << ":"
                                //<< " " << spHTTPResp->get_curr_value() << std::endl;
#endif
                            } else if ( status == HTTPResponse::PARSE_WATERMARK_REACHED ) {
                                parseHeadersExit_Flag = false;
                            } else if ( status == HTTPResponse::PARSE_BUFFER_END_REACHED ) {
                                spHTTPResp->getJumboBuff()->reInit();
                                parseHeadersExit_Flag = false;
                            } else if ( status == HTTPResponse::PARSE_HEADER_ERROR ) {
                                // error while parsing occured  *
                                parseHeadersExit_Flag = false;
#ifndef NDEBUG
                                // error while parsing occured  *
                                std::cout << "Error while header parsing occurred" << std::endl;
#endif     
                                qMaster_.reqErrorsQ()->add(spHTTPResp->getLinkedHTTPRequest());
                                q.pop();
                            }
                        }
                        /////////////////!!!
                        if ( parseHeadersExit_Flag == false ) {
                            continue;
                            //main for (; iterMap != iterMapEnd; ++iterMap )
                        }
                        /////////////////!!!
                    }
                    if ( spHTTPResp->get_parse_state() == HTTPResponse::state_headers_finished ) {
                        //todo dont forget about codes 1xx
#ifndef NDEBUG
                        std::cout << "Headers parsing finished" << std::endl;
#endif                         
                        if ( spHTTPResp->get_status_code() == "200" && spHTTPResp->spContent_Length.use_count() != 0 ) {
                            shared_ptr<Action> spAction = spHTTPResp->getLinkedHTTPRequest()->getAction();
                            Action::result action_res;

                            action_res = spAction->takeData(spHTTPResp->getJumboBuff()->currentPos_,
                                    spHTTPResp->getJumboBuff()->watermark_,
                                    spHTTPResp->spContent_Length->getValueAsDecimalNumber());

                            if ( action_res == Action::result::ERROR_OCCURED ) {
#ifndef NDEBUG
                                // error while saving occured  *
                                std::cout << "Error while saving occurred" << std::endl;
#endif  
                                //error while writing results
                                qMaster_.reqErrorsQ()->add(spHTTPResp->getLinkedHTTPRequest());
                                q.pop();
                            } else if ( action_res == Action::result::NOT_ALL_DATA_RCVD ) {
#ifndef NDEBUG
                                std::cout << "Saving file" << std::endl;
#endif  
                                spHTTPResp->getJumboBuff()->reInit();
                            } else if ( action_res == Action::result::ALL_DATA_RCVD ) {
                                SaveFile *save_file_act = dynamic_cast<LobKo::SaveFile*> (spAction.get());
                                if ( save_file_act != NULL ) {
                                    std::cout << "File "
                                            << save_file_act->get_file_name()
                                            << " saved" << std::endl;
                                }
                                q.pop();
                            }
                        } else {
                            qMaster_.reqErrorsQ()->add(spHTTPResp->getLinkedHTTPRequest());
                            q.pop();
                        }
                    }
                } // end else { // reading successful

                if ( q.empty() ) {
                    vecEraseItersAtEnd.push_back(iterMap);
                }
            } // end else. Current q is not empty
        }; // end  for (; iterMap != iterMapEnd; ++iterMap )

        for ( vector<MapIterator>::iterator i = vecEraseItersAtEnd.begin(); i != vecEraseItersAtEnd.end(); ++i ) {
            close((*i)->first);
            map_.erase(*i);
        };
        //std::for_each(vecEraseItersAtEnd.begin(), vecEraseItersAtEnd.end()), map_.erase());
    }
}

int LobKo::RecvBySocketQueue::readFromSocket(int socketFD, const char* buff, int buffSize) {
    assert((buff != NULL) && " RecvBySocketQueue::readFromSocket, Zero pointer\n ");
    assert((socketFD > 0) && " RecvBySocketQueue::readFromSocket, socketFD < 0\n ");

    ssize_t readBytes;

    readBytes = read(socketFD, const_cast<char*> (buff), buffSize);

    return readBytes;
}