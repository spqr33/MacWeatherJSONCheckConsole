/* 
 * File:   QueuesMaster.cpp
 * Author: s
 * 
 * Created on June 17, 2015, 9:57 PM
 */

#include "QueuesMaster.h"
#include <stdexcept>
#include "Action.h"
#include "SaveFile.h"

LobKo::QueuesMaster::QueuesMaster(shared_ptr<HTTPRequest> initialisationHTTPRequest) :
reqQueue_(new HTTPRequestQueue(this)),
errQueue_(new HTTPRequestErrorsQueue),
sendQueue_(new SendBySocketQueue(this)),
recvQueue_(new RecvBySocketQueue(this)),
initialisationHTTPRequest_(initialisationHTTPRequest) {
    reqQueue_->add(initialisationHTTPRequest_);
}

LobKo::QueuesMaster::QueuesMaster(shared_ptr<TaskHolder> spTaskHolder) : reqQueue_(new HTTPRequestQueue(this)),
errQueue_(new HTTPRequestErrorsQueue),
sendQueue_(new SendBySocketQueue(this)),
recvQueue_(new RecvBySocketQueue(this)) {
    loadTasks(spTaskHolder);
}

LobKo::QueuesMaster::~QueuesMaster() {
    ;
}

void LobKo::QueuesMaster::loadTasks(shared_ptr<TaskHolder> spTaskHolder) {
    if ( spTaskHolder.get() != NULL ) {
        vector<Task>::iterator iterVec = spTaskHolder->getTasks().begin();
        vector<Task>::iterator iterVecEnd = spTaskHolder->getTasks().end();

        for (; iterVec != iterVecEnd; ++iterVec ) {
            try {
                shared_ptr<URL> spURL(new URL(iterVec->remoteResource()));
                HTTPRequestType type(LobKo::HTTPRequestType::Type::GET);
                HTTPProto proto(LobKo::HTTPProto::Type::HTTP1_1);

                shared_ptr<HTTPRequest> spHTTPRequest(new HTTPRequest(type, spURL, proto));

                shared_ptr<Action> spAction(new SaveFile(iterVec->localFileName()));
                spHTTPRequest->setAction(spAction);

                reqQueue_->add(spHTTPRequest);

            } catch (const std::invalid_argument& ia) {
                iterVec->setTaskResult(LobKo::Task::taskResult::TASK_ERROR);
            } catch (const std::runtime_error& re) {
                iterVec->setTaskResult(LobKo::Task::taskResult::TASK_ERROR);
            }
        }
    }
}

void LobKo::QueuesMaster::process(int simultaneous_resources_recvng) {
    bool cannot_exit = true;

    int size_HTTPRequestQueue = reqQueue_->getQueueSize();
    int size_RecvBySocketQueue = recvQueue_->getQueueSize();
    int size_SendBySocketQueue;
    while (cannot_exit == true) {

        int n;
        if ( simultaneous_resources_recvng - size_RecvBySocketQueue < size_HTTPRequestQueue ) {
            n = simultaneous_resources_recvng - size_RecvBySocketQueue;
        } else {
            n = size_HTTPRequestQueue;
        }
        for (; n > 0; --n ) {
            reqQueue_->process();
        }
        sendQueue_->process();
        recvQueue_->process();

        size_HTTPRequestQueue = reqQueue_->getQueueSize();
        size_RecvBySocketQueue = recvQueue_->getQueueSize();
        size_SendBySocketQueue = recvQueue_->getQueueSize();

        if ( size_RecvBySocketQueue == 0 ) {
            if ( size_RecvBySocketQueue == size_HTTPRequestQueue ) {
                if ( size_RecvBySocketQueue == size_SendBySocketQueue ) {
                    cannot_exit = false;
                }
            }
        }
    }
}