;//
//  Cache.cpp
//  WeatherJSONCheckConsole
//
//  Created by Admin on 07.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#include "Cache.h"
#include <unistd.h> // getcwd unlink
#include <iostream>
#include <string>
#include <sys/types.h>  //mkdir
#include <sys/stat.h>   //mkdir
#include <fstream>


namespace LobKo {
    Cache::Cache() :
    cacheDirName_("cache")
    {
        init();
    }
    
    void Cache::init() {
        char* absPath = getcwd(NULL, 2048);
    
        if (absPath == NULL) {
            std::string err("Can't obtain current directory");
            log(err);
            exit(-1);
        }
  
        int status;
        status = mkdir(cacheDirName_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ( status < 0 ) {
            std::string err("Can't create the cache directory");
            log(err);
            exit(-1);
        }
        cachePath_ = absPath;
        cachePath_ += "/";
        cachePath_ += cacheDirName_;
        cachePath_ += "/";
        
        free(absPath);
    }
    
    Cache::~Cache() {
        ;
    }
    
    bool Cache::check(const UrlString& url) const {
        auto iter = map_.find(url);
        
        return (iter == map_.end())? false : true;
    }
    
    void Cache::add(const UrlString& url, long fileSize, const std::string& name){
        std::string fullPath = getAbsCachePath() + name;
        
        auto spCacheObject = std::make_shared<CacheObject>(fullPath, name, fileSize);
        map_[url] = spCacheObject;
        logger_->write_line(name + "was cached.");
    }
    
    void Cache::remove(const UrlString& url) {
        auto iter = map_.find(url);
        
        if (iter != map_.end()) {
            auto obj = iter->second;
            unlink(obj->fullPath_.c_str());
            
            map_.erase(iter);
        }
    }
    
    std::pair<std::shared_ptr<JumboBuff>, std::shared_ptr<CacheObject> > Cache::get(const UrlString& url) {
        auto iter = map_.find(url);
        std::shared_ptr<CacheObject> spCacheObj;
        std::shared_ptr<JumboBuff> spJumbo;
        
        if (iter != map_.end()) {
            spCacheObj = iter->second;
            spJumbo = std::make_shared<JumboBuff>(spCacheObj->size_);
            

            std::ifstream f(spCacheObj->fullPath_.c_str(), std::ifstream::binary | std::ofstream::in);
            if (f.is_open()) {
                char* buff = const_cast<char*>(spJumbo->start_);
                f.read(buff, spCacheObj->size_);
                
                if ( !f.good() ) {
                    std::shared_ptr<CacheObject> spCacheObjEmpty;
                    std::shared_ptr<JumboBuff>   spJumboEmpty;
                    
                    remove(url);
                    
                    return std::pair<std::shared_ptr<JumboBuff>, std::shared_ptr<CacheObject>>(spJumboEmpty, spCacheObjEmpty);
                }
            }

        }
        return std::pair<std::shared_ptr<JumboBuff>, std::shared_ptr<CacheObject>>(spJumbo, spCacheObj);
    }
    
    Cache& Cache::instance() {
        static Cache single;
        
        return single;
    }
    
    void Cache::setLogger(std::shared_ptr<Logger>& logger) const {
        logger_ = logger;
    }
    
    void Cache::log(std::string& line) const {
        logger_->write_line(line);
    }
    
    std::shared_ptr<CacheObject> Cache::getInfo(const UrlString& url) {
        auto iter = map_.find(url);
        std::shared_ptr<CacheObject> spCacheObj;
        
        if ( iter != map_.end() ) {
            spCacheObj = iter->second;
        }
        return spCacheObj;
    }
}