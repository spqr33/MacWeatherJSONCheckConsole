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
#include <bits/shared_ptr_base.h>
#include "HTTPRequest.h"
#include "QueuesMaster.h"
#include "TaskHolder.h"
#include <fstream>
#include "HTTPRequestErrorsQueue.h"
#include <stdlib.h> // atoi

using namespace std;
using namespace LobKo;

using std::string;

/*
 * 
 */
int main(int argc, char** argv) {
    ////////////////////////////////////
    if ( argc < 3 ) {
        std::cerr << " Usage:\t\t " << argv[0] << "\t    simultaneous_downloads\t" << " /path/to/taskFile" << std::endl;
        return EXIT_FAILURE;
    }
    const int simultaneous_download_max = 10;
    int simultaneous_download_number = ::atoi(argv[1]);
    ;

    if ( simultaneous_download_number <= 0 || simultaneous_download_number > simultaneous_download_max ) {
        simultaneous_download_number = simultaneous_download_max;
    }

    std::ifstream dataFile(argv[2]);
    if ( !dataFile.is_open() ) {
        std::cerr << "File \"" << argv[2] << "\" can't be opened." << std::endl;
        return EXIT_FAILURE;
    }
    shared_ptr<TaskHolder> spTaskHolder = TaskHolderBuilder::build(dataFile);
    dataFile.clear();
    dataFile.close();

    QueuesMaster qmaster(spTaskHolder);
    qmaster.process(simultaneous_download_number);

    HTTPRequestErrorsQueue& eq = *qmaster.reqErrorsQ().get();
    cerr << eq << std::endl;

    return EXIT_SUCCESS;
}

