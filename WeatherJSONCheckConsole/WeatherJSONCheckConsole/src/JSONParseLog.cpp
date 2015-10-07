//
//  JSONParseLog.cpp
//  WeatherJSONCheckConsole
//
//  Created by Admin on 06.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#include "JSONParseLog.h"
#include <algorithm>
#include <iterator>
#include "picojson.h"

namespace LobKo{
    
    JSONParseLog::JSONParseLog(Logger& log)     {
        buffer_.reserve(128);
    }
    
     Action::result JSONParseLog::takeData(const char* const startBuf, const char* const endBuf, uint64_t totalDataSize) {
        
        std::copy(startBuf, endBuf, std::back_inserter(buffer_));
         for(auto x :buffer_) {
             std::cout << x;
         }
         std::cout << "Log this =)" << std::endl;
        
            //LOG!!
            picojson::value v;
            std::string     err;
            picojson::parse(v, buffer_.data(), buffer_.data()+buffer_.size(), &err);
            
            if ( !err.empty() ) {
                //Log error
                
            } else if ( !v.is<picojson::object>() ) {
                //Log error
            } else {
                std::string indent("");
                JSONParseAndFill(v.get<picojson::object>(), indent);
            }
            return Action::ALL_DATA_RCVD;
        //}
    }
    
    void JSONParseLog::JSONParseAndFill(picojson::value::object &obj, std::string indent)
    {
        picojson::value::object::const_iterator i = obj.begin();
        for ( ; i != obj.end(); ++i ) {
            
            picojson::value v =  i->second;
            if (i->second.is<picojson::object>()) {
                std::cout << indent << i->first << " : "<< std::endl;
                JSONParseAndFill(v.get<picojson::object>(), indent + "  ");
            } else if (i->second.is<picojson::array>()) {
                std::cout << indent << i->first << " : ["<< std::endl;
                JSONParseAndFill(v.get<picojson::array>(), indent + "  ");
                std::cout << " ]"<< std::endl;
            } else {
                std::cout << indent << i->first << " : " << i->second << std::endl;
            }
        }
    }
    
    void JSONParseLog::JSONParseAndFill(picojson::value::array &obj, std::string indent)
    {
        picojson::value::array::iterator i = obj.begin();
        for ( ; i != obj.end(); ++i ) {
            picojson::value v = *i;
            
            if ((*i).is<picojson::object>()) {
                JSONParseAndFill(v.get<picojson::object>(), indent + "  ");
            } else if ( (*i).is<picojson::array>() ) {
                JSONParseAndFill(v.get<picojson::array>(), indent + "  ");
            }
        }
    }
    
    
    
    
    
} //namespace