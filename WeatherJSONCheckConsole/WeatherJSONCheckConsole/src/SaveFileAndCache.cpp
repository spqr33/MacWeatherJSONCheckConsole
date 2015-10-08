//
//  SaveFileAndCache.cpp
//  WeatherJSONCheckConsole
//
//  Created by Admin on 08.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#include "SaveFileAndCache.h"
#include "Cache.h"

namespace LobKo {
    SaveFileAndCache::SaveFileAndCache(const std::string& fileName, std::shared_ptr<URL> url) :
    SaveFile(Cache::instance().getAbsCachePath()+"/"+fileName),
    url_(url)
    {
    }
    
    SaveFileAndCache::~SaveFileAndCache() {
    }
    
    Action::result
    SaveFileAndCache::takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totaDataSize){
        Cache& cache = Cache::instance();
        
        cache.add(url_->originalRequestString(), startBuf, endBuf, get_file_name());
       
        return Action::ALL_DATA_RCVD;
    }
}