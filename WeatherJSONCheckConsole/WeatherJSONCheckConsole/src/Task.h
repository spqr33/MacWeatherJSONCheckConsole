/* 
 * File:   Task.h
 * Author: s
 *
 * Created on June 20, 2015, 9:35 PM
 */

#ifndef TASK_H
#define	TASK_H
#include <string>

using std::string;

namespace LobKo {

    class Task {
    public:

        enum taskResult {
            TASK_DUMMY, TASK_ERROR, TASK_SUCCESS
        };
        Task(string remoteResourcePath, string localFileName);
        virtual ~Task();
        inline const string& remoteResource() const;
        inline const string& localFileName() const;
        inline void setTaskResult(taskResult res);
        inline taskResult getTaskResult()const;
    private:
        std::string remoteResourcePath_;
        std::string localFileName_;
        taskResult result_;
    };
};

const string& LobKo::Task::remoteResource() const {
    return remoteResourcePath_;
}

const string& LobKo::Task::localFileName() const {
    return localFileName_;
}

void LobKo::Task::setTaskResult(taskResult res) {
    result_ = res;
}

LobKo::Task::taskResult LobKo::Task::getTaskResult() const {
    return result_;
}
#endif	/* TASK_H */

