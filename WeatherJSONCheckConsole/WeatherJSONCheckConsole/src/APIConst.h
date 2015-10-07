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

namespace LobKo {
    struct APIConst {
        static APIConst& instance();
        
        const std::string hostname = "api.openweathermap.org";
        const std::string urlRoot = "http://api.openweathermap.org/";
        const std::string weatherQuery = "data/2.5/weather/?q=";
        const std::string weatherImgQuery = "img/w/";
        const std::string imgExtention = ".png";
        
        
    private:
        APIConst()
        APIConst(APIConst& orig);
        APIConst& operator=(APIConst& rhs);
    }; // struct APIConst
}//namespace

#endif /* defined(__WeatherJSONCheckConsole__APIConst__) */
