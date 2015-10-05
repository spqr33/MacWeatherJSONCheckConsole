/* 
 * File:   HostHeader.cpp
 * Author: s
 * 
 * Created on June 16, 2015, 4:19 PM
 */

#include "HostHeader.h"

LobKo::HostHeader::HostHeader(const string& name, const string& value) :
AHeader(name, value) {
    ;
}

LobKo::HostHeader::~HostHeader() {
    ;
}

const  string& LobKo::HostHeader::name() {
    static const string name = "Host";
    
    return name;
}