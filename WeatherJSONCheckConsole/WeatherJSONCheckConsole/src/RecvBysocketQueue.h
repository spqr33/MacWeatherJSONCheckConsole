/* 
 * File:   RecvBysocketQueue.h
 * Author: s
 *
 * Created on June 19, 2015, 7:56 PM
 */

#ifndef RECVBYSOCKETQUEUE_H
#define	RECVBYSOCKETQUEUE_H

#include <memory>
#include "HTTPRequest.h"
#include "QueuesMaster.h"
#include <map>
#include <sys/socket.h>         //getsockopt

using std::map;
using std::queue;

namespace LobKo {
    class QueuesMaster;

    class RecvBySocketQueue {
    public:
        explicit RecvBySocketQueue(QueuesMaster*);
        ~RecvBySocketQueue();

        void add(int socketFD, shared_ptr<HTTPResponse>);
        void process();
        inline unsigned int getSystemRCVBUF_Size(int socketFd) const;
        inline int getQueueSize() const;
    private:
        RecvBySocketQueue(const RecvBySocketQueue& orig);
        int readFromSocket(int socketFD, const char* buff, int buffSize);
        QueuesMaster& qMaster_;

        map<int, queue<shared_ptr<HTTPResponse> > > map_;
    };
};

unsigned int LobKo::RecvBySocketQueue::getSystemRCVBUF_Size(int socketFd) const {
    static bool sizeAlreadyCalculated = false;
    static int size = 0;
    static const unsigned int defaultSize = 87380;

    if (!sizeAlreadyCalculated) {
        static socklen_t len = sizeof (size);
        if (getsockopt(socketFd, SOL_SOCKET, SO_RCVBUF, &size, &len) < 0) {
            size = defaultSize;
        }
        sizeAlreadyCalculated = true;
    }

    return size;
};

int LobKo::RecvBySocketQueue::getQueueSize() const {
    return map_.size();
};
#endif	/* RECVBYSOCKETQUEUE_H */

