/* 
 * File:   TaskHolder.cpp
 * Author: s
 * 
 * Created on June 20, 2015, 10:07 PM
 */

#include "TaskHolder.h"
#include <sstream>
#include <iostream>

LobKo::TaskHolder::TaskHolder() {
    tasksVec_.reserve(16);
}

LobKo::TaskHolder::~TaskHolder() {
    ;
}

void LobKo::TaskHolder::addTask(const Task& newTask) {
    tasksVec_.push_back(newTask);
}

vector<LobKo::Task>& LobKo::TaskHolder::getTasks() {
    return tasksVec_;
}

shared_ptr<LobKo::TaskHolder> LobKo::TaskHolderBuilder::build(ifstream& file_in) {
    shared_ptr<TaskHolder> spBuildThis(new TaskHolder);

    std::string str;

    file_in.clear();
    bool flag = true;
    do {
        std::getline(file_in, str);
        if ( file_in.bad() || file_in.fail() ) {
            break;
        } else if ( file_in.eof() ) {
            flag == false;
        }
        std::istringstream istream(str);

        string res;
        string filepath;

        istream >> res >> filepath;
        if ( res.size() && res[0] != '#' ) {
            if ( filepath.size() ) {
                spBuildThis->addTask(Task(res, filepath));
            }
        }
    } while (flag);


    return spBuildThis;
}