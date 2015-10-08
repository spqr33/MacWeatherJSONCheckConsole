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
    
    std::shared_ptr<URL> APIConst::getURL(const Icon& icon) {
        APIConst&               api = APIConst::instance();
        std::shared_ptr<URL>    spIcon = std::make_shared<URL>(api.urlRoot +
                                                               api.weatherImgQuery +
                                                               icon.name_ +
                                                               api.imgExtention
        );
        
        return spIcon;
    }
}