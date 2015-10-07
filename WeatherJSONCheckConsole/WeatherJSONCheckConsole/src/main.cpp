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
//#include <bits/shared_ptr_base.h>
#include "HTTPRequest.h"
#include "QueuesMaster.h"
#include "TaskHolder.h"
#include <fstream>
#include "HTTPRequestErrorsQueue.h"
#include <stdlib.h> // atoi
#include "Logger.h"
#include "JSONParseLog.h"
#include "Cache.h"

using namespace std;
using namespace LobKo;


/*
 * 
 */
int main(int argc, char** argv) {
    ////////////////////////////////////
    const int simultaneous_download_max = 1;

    string logName("log.txt");
    Logger log(logName);
    QueuesMaster qmaster;
    Cache        cache(log);


    HTTPRequestErrorsQueue& eq = *qmaster.reqErrorsQ().get();
    cerr << eq << std::endl;

//////////////
//API const
    const string hostname = "api.openweathermap.org";
    const string urlRoot = "http://api.openweathermap.org/";
    const string weatherQuery = "data/2.5/weather/?q=";
    const string weatherImgQuery = "img/w/";
    const string imgExtention = ".png";

 
    //
    string cityName;
    while (true) {
        cout << "Please, enter a city name, or type exit" << endl;
        cin >> cityName;
        if ( cityName == "exit" ) {
            break;
        }
        shared_ptr<URL>         spWeatherUrl(new URL(urlRoot + weatherQuery + cityName));
        shared_ptr<HTTPRequest> request(new HTTPRequest(HTTPRequestType(HTTPRequestType::GET),
                                                        spWeatherUrl,
                                                        HTTPProto(HTTPProto::HTTP1_0))
                                        );
        std::cout << spWeatherUrl->originalRequestString();
        request->setAction(std::make_shared<JSONParseLog>(log));
       
        qmaster.setHTTPRequest(request);
        qmaster.process(simultaneous_download_max);
        
        cout << "Processing finished" << endl;
}
    
    return EXIT_SUCCESS;
}

