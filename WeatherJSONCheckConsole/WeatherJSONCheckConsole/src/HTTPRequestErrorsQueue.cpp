/* 
 * File:   HTTPRequestErrorsQueue.cpp
 * Author: s
 * 
 * Created on June 17, 2015, 8:26 PM
 */

#include "HTTPRequestErrorsQueue.h"
#include "URL.h"

LobKo::HTTPRequestErrorsQueue::HTTPRequestErrorsQueue(int holderSize) :
maxSize_(holderSize) {
    ;
}

LobKo::HTTPRequestErrorsQueue::~HTTPRequestErrorsQueue() {

}

void LobKo::HTTPRequestErrorsQueue::add(shared_ptr<HTTPRequest> sp) {
    int size = list_.size();

    if ( size == maxSize_ ) {
        list_.erase(list_.begin());
    }
    list_.push_back(sp);
}

std::ostream& LobKo::operator<<(std::ostream& out, LobKo::HTTPRequestErrorsQueue& q) {
    if ( q.size() != 0 ) {
        out << "Errors happened while processing these resources:" << std::endl;

        std::list<shared_ptr<LobKo::HTTPRequest> >::iterator iter_list = q.getContainer().begin();
        std::list<shared_ptr<LobKo::HTTPRequest> >::iterator iter_list_end = q.getContainer().end();

        for (; iter_list != iter_list_end; ++iter_list ) {
            out << (*iter_list)->getURL()->originalRequestString() << std::endl;
        }
    }
    return out;
}

std::list<shared_ptr<LobKo::HTTPRequest> >& LobKo::HTTPRequestErrorsQueue::getContainer() {
    return list_;
}