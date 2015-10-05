/* 
 * File:   Task.cpp
 * Author: s
 * 
 * Created on June 20, 2015, 9:35 PM
 */

#include "Task.h"

LobKo::Task::Task(string remoteResourcePath, string localFileName) :
remoteResourcePath_(remoteResourcePath),
localFileName_(localFileName),
result_(TASK_DUMMY) {
}

LobKo::Task::~Task() {
    ;
}

