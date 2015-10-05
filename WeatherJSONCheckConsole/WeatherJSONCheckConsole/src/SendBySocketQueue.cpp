/* 
 * File:   SendBySocketQueue.cpp
 * Author: s
 * 
 * Created on June 18, 2015, 3:21 PM
 */

#include "SendBySocketQueue.h"
#include "RecvBysocketQueue.h"
#include <cassert>
#include <unistd.h> //write()


LobKo::SendBySocketQueue::SendBySocketQueue(QueuesMaster* qMaster) :
qMaster_(*qMaster) {
    assert(qMaster && "SendBySocketQueue(): QueuesMaster is zero");

}

LobKo::SendBySocketQueue::~SendBySocketQueue() {
}

void LobKo::SendBySocketQueue::add(int socketFD, shared_ptr<HTTPRequest> spHTTPRequest) {
#ifdef SENDBYSOCKETQUEUE_H_DEBUG
    std::cout << "In add() function of LobKo::SendBySocketQueue:" << std::endl;
#endif

    map_[socketFD].push(spHTTPRequest);
#ifdef SENDBYSOCKETQUEUE_H_DEBUG
    std::cout << "In add() function of LobKo::SendBySocketQueue: End" << std::endl;
#endif
};

void LobKo::SendBySocketQueue::process() {
    if ( map_.empty() ) {
        return;
    } else {
        typedef map<int, queue<shared_ptr<HTTPRequest> > >::iterator MapIterator;
        MapIterator iterMap = map_.begin();
        MapIterator iterMapEnd = map_.end();
        vector<MapIterator> vecEraseItersAtEnd;

        for (; iterMap != iterMapEnd; ++iterMap ) {
            std::queue<shared_ptr<HTTPRequest> >& q = iterMap->second;

            //                while (!q.empty()) { //pipelining
            if ( q.empty() ) {
                vecEraseItersAtEnd.push_back(iterMap);
            } else {
                int socketFD = iterMap->first;
                shared_ptr<HTTPRequest> spHTTPRequest = q.front();
                int writedBytes = 0;
                int needToWriteBytes = spHTTPRequest->getResultString().size() - spHTTPRequest->bytesSent();

                writedBytes = writeToSocket(socketFD, spHTTPRequest->getResultString().c_str(), needToWriteBytes);
                if ( writedBytes < 0 ) {
                    close(socketFD);
                    qMaster_.reqErrorsQ()->add(spHTTPRequest);
                    q.pop();
                } else if ( writedBytes < needToWriteBytes ) {
                    spHTTPRequest->setBytesSent(spHTTPRequest->bytesSent() + writedBytes);
                    //cleanup? - no.
                    continue; // loop on map_
                } else { //sent all of the data
                    spHTTPRequest->setBytesSent(spHTTPRequest->getResultString().size());


                    shared_ptr<HTTPResponse> spResponse(new HTTPResponse(spHTTPRequest));
                    spHTTPRequest->setHTTPResponse(spResponse);

                    qMaster_.recvQ()->add(socketFD, spResponse);
                    q.pop();
                }

                if ( q.empty() ) {
                    vecEraseItersAtEnd.push_back(iterMap);
                }
            }
        } // end  for (; iterMap != iterMapEnd; ++iterMap )

        for ( vector<MapIterator>::iterator i = vecEraseItersAtEnd.begin(); i != vecEraseItersAtEnd.end(); ++i ) {
            map_.erase(*i);
        }
    }
}

int LobKo::SendBySocketQueue::writeToSocket(int socketFD, const char* begin, size_t n) const {
    assert((begin != NULL) && " SendBySocketQueue::writeToSocket, Zero pointer\n ");
    assert((socketFD > 0) && " SendBySocketQueue::writeToSocket, socketFD < 0\n ");
    ssize_t writedBytes = 0;

    writedBytes = write(socketFD, begin, n);

    return writedBytes;
}
