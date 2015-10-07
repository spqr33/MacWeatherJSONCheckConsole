//
//  JSONParseLog.h
//  WeatherJSONCheckConsole
//
//  Created by Admin on 06.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#ifndef __WeatherJSONCheckConsole__JSONParseLog__
#define __WeatherJSONCheckConsole__JSONParseLog__

#include "Action.h"
#include "Logger.h"
#include <vector>
#include "picojson.h"

namespace LobKo {
    class JSONParseLog : public Action {
    public:
        explicit JSONParseLog(Logger& log);
        
        virtual result takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totalDataSize);
        
    private:
        JSONParseLog(const JSONParseLog& orig) = delete;
        JSONParseLog& operator=(const JSONParseLog& rhs) = delete;
        
        void JSONParseAndFill(picojson::value::object &obj, std::string indent);
        void JSONParseAndFill(picojson::value::array &obj, std::string indent);
     
        //std::uint64_t       alreadyBytesSaved_;
        std::vector<char>   buffer_;
        Logger              log_;
    };
 
    
}// namespace

#endif /* defined(__WeatherJSONCheckConsole__JSONParseLog__) */
