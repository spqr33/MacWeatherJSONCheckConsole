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
#include <memory>
#include "APIConst.h"
#include "Cache.h"
#include "HTTPRequest.h"
#include "QueuesMaster.h"
#include "SaveFileAndCache.h"

namespace LobKo{
    
//    JSONParseLog::JSONParseLog(Logger& log) :
//    log_{
//        buffer_.reserve(128);
//    }
    JSONParseLog::JSONParseLog (std::shared_ptr<Logger>& log, QueuesMaster& qmaster) :
    wpLog_(log), qmaster_(qmaster){
        buffer_.reserve(512);
    }
    Action::result JSONParseLog::takeData(const char* const startBuf, const char* const endBuf, uint64_t totalDataSize) {
        
        std::copy(startBuf, endBuf, std::back_inserter(buffer_));
        std::string strLog(startBuf, endBuf);
        log(strLog);
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
                std::cout << indent << i->first << " : " << i->second.to_str() << std::endl;
                //waiting weather.icon
                if (i->first == "icon") {
                    std::shared_ptr<URL>    spURL = APIConst::getURL(APIConst::Icon(i->second.to_str()));
                    Cache&                  cache = Cache::instance();
                    if (cache.check(spURL->originalRequestString()) == true){
                        //URL was not cached
                        std::shared_ptr<HTTPRequest> request(new HTTPRequest(HTTPRequestType(HTTPRequestType::GET),
                                                                        spURL,
                                                                        HTTPProto(HTTPProto::HTTP1_0))
                                                        );
                        log(spURL->originalRequestString());
                        
                        request->setAction(std::make_shared<SaveFileAndCache>(                                                                            i->second.to_str()+APIConst::instance().imgExtention,
                                           spURL)
                                           );
                        
                        qmaster_.setHTTPRequest(request);
                        qmaster_.process(1);
                       
                    } else {
                        //URL was cached
                        std::string fullPath =  cache.getInfo(spURL->originalRequestString())->fullPath_;
                        
                        std::string logMessage = "URL " + spURL->originalRequestString() + " was cached, ";
                        logMessage += "Please look at " + fullPath + " file";
                        log(logMessage);
                    }
                }
                
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
    
    void JSONParseLog::log(const std::string& line) {
        std::shared_ptr<Logger> spLog = wpLog_.lock();
        
        if (spLog.get() != nullptr) {
            spLog->write_line(line);
        }
    }
    
} //namespace