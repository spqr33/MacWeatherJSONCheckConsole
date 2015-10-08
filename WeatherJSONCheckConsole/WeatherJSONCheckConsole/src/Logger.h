//
//  Logger.h
//  WeatherJSONCheckConsole
//
//  Created by Admin on 06.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#ifndef WeatherJSONCheckConsole_Logger_h
#define WeatherJSONCheckConsole_Logger_h


#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include <memory>

namespace LobKo{
    
    struct File
    {
        std::ofstream   file_;
        std::mutex      mutexFile_;
        std::string     fileName_;
    };
    
    class Logger
    {
    public:
        explicit Logger(std::string& f);
        ~Logger();

        void write_line(std::string line);
    private:
        static std::map<std::string, std::shared_ptr<File> >    files_;
        static std::mutex                                       mutexFiles_;
        std::shared_ptr<File>                                   spFile_;
    };
}

#endif
