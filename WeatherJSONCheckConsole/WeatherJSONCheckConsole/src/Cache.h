//
//  Cache.h
//  WeatherJSONCheckConsole
//
//  Created by Admin on 07.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#ifndef __WeatherJSONCheckConsole__Cache__
#define __WeatherJSONCheckConsole__Cache__

#include <stdio.h>
#include <string>
#include <map>
#include <utility>
#include <memory>
#include "Logger.h"
#include "JumboBuff.h"


namespace LobKo {

    struct CacheObject;
    
    class Cache {
    public:
        typedef  std::string UrlString;
        
        explicit Cache(Logger& log);
        virtual ~Cache();
        
        void add(UrlString& url, const char* start, const char* end, const std::string& name);
        bool check(const UrlString& url) const ;
        
        std::pair< std::shared_ptr<JumboBuff>, std::shared_ptr<CacheObject> > get(const UrlString&);
        
    private:
        std::map <UrlString, std::shared_ptr<CacheObject> > map_;
        std::string                                         cachePath_;
        std::string                                         cacheDirName_;
        Logger                                              logger_;
        
        virtual void init();
        void remove(const UrlString& url);
        
        Cache(const Cache& orig) = delete;
        const Cache& operator=(const Cache& rhs) = delete;
    };
    struct CacheObject {
        CacheObject(const std::string& path, const std::string& name, uint32_t size) :
        fullPath_(path), name_(name), size_(size){}
        
        const std::string fullPath_;
        const std::string name_;
        const long    size_;
    };

}


#endif /* defined(__WeatherJSONCheckConsole__Cache__) */
