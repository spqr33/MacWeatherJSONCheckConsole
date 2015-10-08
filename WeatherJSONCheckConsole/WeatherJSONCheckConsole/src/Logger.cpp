//
//  Logger.cpp
//  WeatherJSONCheckConsole
//
//  Created by Admin on 06.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//
#include "Logger.h"
#include "iostream"

#include <ctime>
#include <ratio>
#include <chrono>

namespace LobKo {
//Static
    std::map<std::string, std::shared_ptr<File>> Logger::files_;
    std::mutex Logger::mutexFiles_;
//End Static
    
    Logger::Logger(std::string& f)
    {
        std::lock_guard<std::mutex> lock(mutexFiles_);
        
        if(files_.count(f)>0)
        {
            spFile_ = files_[f];
            return;
        }
        
        files_[f] = std::make_shared<File>();
        files_[f]->file_.open(f, std::ios::app);
        files_[f]->fileName_ = f;
        spFile_ = files_[f];
        
    }
    Logger::~Logger()
    {
        if(spFile_.use_count() == 2) // myself and map
        {
            std::lock_guard<std::mutex> lock(mutexFiles_);
            files_.erase(spFile_->fileName_);
        }
    }

    void Logger::write_line(std::string& line)
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t ( now );
        std::string strTime = ctime(&tt);
        strTime[strTime.size()-1] = ' ';
        
        std::lock_guard<std::mutex> lock(spFile_->mutexFile_);
        spFile_->file_ << strTime << " || " << line << std::endl;
    }
    void write_line(const std::string& line) {
        write_line(const_cast<std::string&>(line));
    }
    
    
} //end namespace
