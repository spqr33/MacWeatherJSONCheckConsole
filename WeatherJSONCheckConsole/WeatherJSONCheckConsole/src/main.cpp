/* 
 * File:   main.cpp
 * Author: s
 *
 * Created on June 12, 2015, 3:56 PM
 */

#include <cstdlib>
#include "utils.h"
#include "IPAdress.h"
#include "URL.h"
#include <string>
#include <utility> //pair
#include <memory>
#include <iostream>
#include "HTTPRequest.h"
#include "QueuesMaster.h"
#include "TaskHolder.h"
#include <fstream>
#include "HTTPRequestErrorsQueue.h"
#include <stdlib.h> // atoi
#include "Logger.h"
#include "JSONParseLog.h"
#include "Cache.h"
#include "APIConst.h"

using namespace std;
using namespace LobKo;


/*
 * 
 */
int main(int argc, char** argv) {
////////////////
    const int simultaneous_download_max = 1;

    string                  logName("log.txt");
    std::shared_ptr<Logger> spLog = std::make_shared<Logger>(logName);
    QueuesMaster            qmaster;
    Cache&                  cache = Cache::instance();
    cache.setLogger(spLog);
    const APIConst&         invariant = APIConst::instance();
//////////////
    string          cityName;
    std::string     urlRoot         = invariant.urlRoot;
    std::string     weatherQuery    = invariant.weatherQuery;
    
    while (true) {
        cout << "Please, enter a city name, or type exit" << endl;
        cin >> cityName;
        if ( cityName == "exit" ) {
            break;
        }
        //shared_ptr<URL>         spWeatherUrl(new URL(urlRoot + weatherQuery + cityName));
        shared_ptr<URL>         spWeatherUrl = make_shared<URL>( urlRoot +
                                                                weatherQuery +
                                                                cityName +
                                                                invariant.ampersand +
                                                                invariant.APPIDkeyEqual +
                                                                invariant.APPIDvalue );
        shared_ptr<HTTPRequest> request(new HTTPRequest(HTTPRequestType(HTTPRequestType::GET),
                                                        spWeatherUrl,
                                                        HTTPProto(HTTPProto::HTTP1_0))
                                        );
        spLog->write_line(spWeatherUrl->originalRequestString());
        request->setAction(std::make_shared<JSONParseLog>(spLog, qmaster));
       
        qmaster.setHTTPRequest(request);
        qmaster.process(simultaneous_download_max);
        qmaster.process(simultaneous_download_max);
        
        spLog->write_line("______________________________________");
    }
    
    return EXIT_SUCCESS;
}

