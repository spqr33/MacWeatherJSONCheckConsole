//
//  SaveFileAndCache.h
//  WeatherJSONCheckConsole
//
//  Created by Admin on 08.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#ifndef __WeatherJSONCheckConsole__SaveFileAndCache__
#define __WeatherJSONCheckConsole__SaveFileAndCache__

#include "SaveFile.h"
#include "URL.h"
#include <memory>

namespace LobKo {
    class SaveFileAndCache : public SaveFile {
    public:
        SaveFileAndCache(const std::string& fileName, std::shared_ptr<URL> url); //throw std::runtime_error
        virtual ~SaveFileAndCache();
        
        virtual result takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totaDataSize);
    private:
        std::shared_ptr<URL> url_;
    }; //class SaveFileAndCache
    
    
}; // namespace

#endif /* defined(__WeatherJSONCheckConsole__SaveFileAndCache__) */
