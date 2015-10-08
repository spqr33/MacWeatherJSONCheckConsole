//
//  SaveFileAndCache.h
//  WeatherJSONCheckConsole
//
//  Created by Admin on 08.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#ifndef __WeatherJSONCheckConsole__SaveFileAndCache__
#define __WeatherJSONCheckConsole__SaveFileAndCache__

#include "Action.h"
#include "URL.h"
#include <memory>
#include <fstream>

namespace LobKo {
    class SaveFileAndCache : public Action {
    public:
        SaveFileAndCache(const std::string& fileName, std::shared_ptr<URL> url); //throw std::runtime_error
        virtual ~SaveFileAndCache();
        
        virtual result takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totaDataSize);
    protected:
        std::shared_ptr<URL> url_;
       
        std::uint64_t alreadyBytesSaved_;
        std::ofstream fout_;
        std::string saveTo_FileName_;
        std::string saveTo_FilePath_;

    }; //class SaveFileAndCache
    
    
}; // namespace

#endif /* defined(__WeatherJSONCheckConsole__SaveFileAndCache__) */
