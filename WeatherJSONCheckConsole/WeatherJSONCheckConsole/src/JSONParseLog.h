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
#include "QueuesMaster.h"
#include <memory>

namespace LobKo {
    class JSONParseLog : public Action {
    public:
        JSONParseLog (std::shared_ptr<Logger>& log, QueuesMaster& qmaster);
        
        virtual result takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totalDataSize);
    private:
        JSONParseLog(const JSONParseLog& orig) = delete;
        JSONParseLog& operator=(const JSONParseLog& rhs) = delete;
        
        void JSONParseAndFill(picojson::value::object &obj, std::string indent);
        void JSONParseAndFill(picojson::value::array &obj, std::string indent);
     
        std::vector<char>           buffer_;
        std::weak_ptr<Logger>       wpLog_;
        QueuesMaster&               qmaster_;
        
        void log(const std::string& line);
    };
}// namespace

#endif /* defined(__WeatherJSONCheckConsole__JSONParseLog__) */
