/* 
 * File:   HTTPRequestErrorsQueue.h
 * Author: s
 *
 * Created on June 17, 2015, 8:26 PM
 */

#ifndef HTTPREQUESTERRORSQUEUE_H
#define	HTTPREQUESTERRORSQUEUE_H
#include <queue>
#include <memory>
#include "HTTPRequest.h"
#include <list>

namespace LobKo {

    class HTTPRequestErrorsQueue {
    public:
        friend std::ostream& operator<<(std::ostream& out, HTTPRequestErrorsQueue& q);

        enum {
            HOLDER_MAX_SIZE = 1000
        };
        HTTPRequestErrorsQueue(int holderSize = HOLDER_MAX_SIZE);
        virtual ~HTTPRequestErrorsQueue();

        void add(shared_ptr<HTTPRequest> sp);
        std::list<shared_ptr<HTTPRequest> >& getContainer();

        uint32_t size() const {
            return list_.size();
        }
    private:
        HTTPRequestErrorsQueue(const HTTPRequestErrorsQueue& orig);

        std::list<shared_ptr<HTTPRequest> > list_;
        const int maxSize_;
    };

    std::ostream& operator<<(std::ostream& out, HTTPRequestErrorsQueue& q);
};
#endif	/* HTTPREQUESTERRORSQUEUE_H */

