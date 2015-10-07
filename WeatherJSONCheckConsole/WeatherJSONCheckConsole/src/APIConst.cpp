//
//  APIConst.cpp
//  WeatherJSONCheckConsole
//
//  Created by Admin on 07.10.15.
//  Copyright (c) 2015 weather. All rights reserved.
//

#include "APIConst.h"

namespace LobKo {
    APIConst::APIConst() {
        ;
    }
    
    APIConst& APIConst::instance(){
        static APIConst self;
        
        return self;
    }
}