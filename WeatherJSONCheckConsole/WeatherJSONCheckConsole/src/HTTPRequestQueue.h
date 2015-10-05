/* 
 * File:   HTTPRequestQueue.h
 * Author: s
 *
 * Created on June 17, 2015, 8:09 PM
 */

#ifndef HTTPREQUESTQUEUE_H
#define	HTTPREQUESTQUEUE_H

#include "HTTPRequest.h"
#include "QueuesMaster.h"
#include "utils.h"
#include  <memory>
#include  <queue>

namespace LobKo {
    class QueuesMaster;

    class HTTPRequestQueue {
    public:
        HTTPRequestQueue(QueuesMaster*);
        virtual ~HTTPRequestQueue();

        void add(shared_ptr<HTTPRequest> sp);
        void process(int requestsNumber = 1);
        inline int getQueueSize() const;
    private:
        /**
         * It is necessary to clean up if error 
         * @param remoteIP must be in nework order!
         * @param remotePort must be in nework order!
         * @return  socketFD, -1 if errors are occured
         */
        int getConnectedSocket(in_addr_t remoteIP, uint16_t remotePort) const;
        HTTPRequestQueue(const HTTPRequestQueue& orig);

        std::queue<shared_ptr<HTTPRequest> > queue_;
        QueuesMaster& qMaster_;
        DomainNameResolver resolver_;
    };
};

int LobKo::HTTPRequestQueue::getQueueSize() const {
    return queue_.size();
}
#endif	/* HTTPREQUESTQUEUE_H */

