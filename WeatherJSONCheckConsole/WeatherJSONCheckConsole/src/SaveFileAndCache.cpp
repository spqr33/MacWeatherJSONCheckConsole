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
    url_(url),
    alreadyBytesSaved_(0),
    saveTo_FileName_(fileName),
    saveTo_FilePath_(Cache::instance().getAbsCachePath()){
        using std::ios_base;
        fout_.open(saveTo_FilePath_ + saveTo_FileName_, ios_base::out /*| ios_base::app */ | ios_base::binary);
        
        if ( !fout_.is_open() ) {
            throw std::runtime_error("Can't open the file: " + fileName);
        }
    }
    
    SaveFileAndCache::~SaveFileAndCache() {
    }
    
    Action::result
    SaveFileAndCache::takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totaDataSize){
        Cache& cache = Cache::instance();
        
        uint64_t delta = endBuf - startBuf;
        
        totaDataSize -= alreadyBytesSaved_;
        
        if ( delta < totaDataSize ) {
            fout_.write(startBuf, delta);
            alreadyBytesSaved_ += delta;
            
            if ( !fout_.good() ) {
                fout_.close();
                return Action::ERROR_OCCURED;
            } else {
                return Action::NOT_ALL_DATA_RCVD;
            }
        } else {
            fout_.write(startBuf, totaDataSize);
            alreadyBytesSaved_ += delta;
            
            if ( !fout_.good() ) {
                fout_.close();
                return Action::ERROR_OCCURED;
            } else {
                cache.add(url_->originalRequestString(), alreadyBytesSaved_, saveTo_FileName_);
                return Action::ALL_DATA_RCVD;
            }
        }
        
        
       
        return Action::ALL_DATA_RCVD;
    }
}