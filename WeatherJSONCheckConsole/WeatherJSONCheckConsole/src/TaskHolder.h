/* 
 * File:   TaskHolder.h
 * Author: s
 *
 * Created on June 20, 2015, 10:07 PM
 */

#ifndef TASKHOLDER_H
#define	TASKHOLDER_H
#include <vector>
#include "Task.h"
#include <memory>
#include <string>
#include <fstream>

using std::vector;
using std::shared_ptr;
using std::string;
using std::ifstream;

namespace LobKo {

    class TaskHolder {
    public:
        TaskHolder();
        virtual ~TaskHolder();

        void addTask(const Task&);
        vector<Task>& getTasks();
    private:
        TaskHolder(const TaskHolder& orig);
        vector<Task> tasksVec_;
    };

    struct TaskHolderBuilder {
        static shared_ptr<TaskHolder> build(ifstream& load_tasks_from_file);
    };
};
#endif	/* TASKHOLDER_H */

