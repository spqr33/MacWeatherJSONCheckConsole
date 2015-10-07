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
    Cache::Cache(Logger& log) :
    cacheDirName_("cache"),
    logger_(log)
    {
        init();
    }
    
    void Cache::init() {
        char* absPath = getcwd(NULL, 1024);
    
        if (absPath == NULL) {
            std::string err("Can't obtain current directory");
            logger_.write_line(err);
            exit(-1);
        }
  
        int status;
        status = mkdir(cacheDirName_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if ( status < 0 ) {
            std::string err("Can't create the cache directory");
            logger_.write_line(err);
            exit(-1);
        }
        cachePath_ = absPath;
        cachePath_ += "/";
        cachePath_ += cacheDirName_;
        
        free(absPath);
    }
    
    Cache::~Cache() {
        ;
    }
    
    bool Cache::check(const UrlString& url) const {
        auto iter = map_.find(url);
        
        return (iter == map_.end())? false : true;
    }
    
    void Cache::add(UrlString& url, const char* start, const char* end, const std::string& name){
        remove(url);
        
        long fileSize = end - start;
        std::string fullPath = cachePath_ + name;
        {
            std::ofstream f(fullPath.c_str(), std::ofstream::binary| std::ofstream::out);
            if (f.is_open()) {
                f.write(start, fileSize);
                
                if ( !f.good() ) {
                    return;
                }
            }
        }
        
        auto spCacheObject = std::make_shared<CacheObject>(fullPath, name, fileSize);
        map_[url] = spCacheObject;
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
}