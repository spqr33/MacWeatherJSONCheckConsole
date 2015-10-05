/* 
 * File:   HTTPRequestQueue.cpp
 * Author: s
 * 
 * Created on June 17, 2015, 8:09 PM
 */

#include "HTTPRequestQueue.h"
#include <utility>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <strings.h> // bzero

LobKo::HTTPRequestQueue::HTTPRequestQueue(QueuesMaster* qmaster) :
qMaster_(*qmaster) {
    assert(qmaster && "HTTPRequestQueue(): QueuesMaster is zero");
}

LobKo::HTTPRequestQueue::~HTTPRequestQueue() {
}

void LobKo::HTTPRequestQueue::add(shared_ptr<HTTPRequest> sp) {
    queue_.push(sp);
}

void LobKo::HTTPRequestQueue::process(int requestsNumber) {
#ifdef HTTPREQUESTQUEUE_H_DEBUG
    std::cout << "HTTPRequestQueue size:" << queue_.size() << std::endl;
#endif
    if ( queue_.empty() ) {
        return;
    }

    shared_ptr<HTTPRequest> spHTTPRequest = queue_.front();
    const std::string& hostname = spHTTPRequest->getURL()->getHost();

    std::pair<DNSResult, shared_ptr<IPAdress> > pair;
    pair = resolver_(hostname);

    if ( pair.first == DNSResult::DNS_RESOLVE_ERROR ) {
        spHTTPRequest->setLastUsedIp(0);
        qMaster_.reqErrorsQ()->add(spHTTPRequest);
    } else {
        in_addr_t ip = (pair.second).get()->getIP();
        uint16_t port = spHTTPRequest->getURL()->getPort();

        int socketFD;
        if ( (socketFD = getConnectedSocket(ip, port)) < 0 ) { //Error while connecting
#ifndef NDEBUG
            std::cout << "Unable to connect to host "
                    << spHTTPRequest->getURL()->getHost() << std::endl;
#endif
            qMaster_.reqErrorsQ()->add(spHTTPRequest);
        } else {
#ifndef NDEBUG
            std::cout << "Host " << spHTTPRequest->getURL()->getHost() 
                    << " connected " << std::endl;
#endif
            qMaster_.sendQ()->add(socketFD, spHTTPRequest);
        }
    }
    queue_.pop();
}

int LobKo::HTTPRequestQueue::getConnectedSocket(in_addr_t remoteIP, uint16_t remotePort) const {
    int socketfd;

    if ( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        return -1;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof (serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = remotePort;
    serverAddr.sin_addr.s_addr = remoteIP;

    if ( connect(socketfd, (struct sockaddr*) &serverAddr, sizeof (serverAddr)) < 0 ) {
        close(socketfd);
        return -1;
    }
    return socketfd;
}