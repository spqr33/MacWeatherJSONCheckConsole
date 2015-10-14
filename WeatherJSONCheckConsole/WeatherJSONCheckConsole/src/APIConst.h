//
//  APIConst.h
//  WeatherJSONCheckConsole
//
//  Created by Admin on 07.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#ifndef __WeatherJSONCheckConsole__APIConst__
#define __WeatherJSONCheckConsole__APIConst__

#include <string>
#include <memory>
#include "URL.h"

namespace LobKo {
    struct APIConst {
        static APIConst& instance();
        
        const std::string hostname = "api.openweathermap.org";
        const std::string urlRoot = "http://api.openweathermap.org/";
        const std::string weatherQuery = "data/2.5/weather/?q=";
        const std::string weatherImgQuery = "img/w/";
        const std::string imgExtention = ".png";
        const std::string ampersand = "&";
        const std::string APPIDkeyEqual = "APPID=";
        const std::string APPIDvalue = "f8bbb1a030af9c566c1a284abbeb7db8";
        
        struct Icon{
            Icon(const std::string& name) : name_(name) {};
            std::string name_;
        };
        
        static std::shared_ptr<URL> getURL(const Icon& icon);
        
    private:
        APIConst();
        APIConst(APIConst& orig);
        APIConst& operator=(APIConst& rhs);
    }; // struct APIConst
}//namespace

#endif /* defined(__WeatherJSONCheckConsole__APIConst__) */
