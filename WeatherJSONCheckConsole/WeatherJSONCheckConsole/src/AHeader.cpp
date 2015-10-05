/* 
 * File:   AHeader.cpp
 * Author: s
 * 
 * Created on June 16, 2015, 4:08 PM
 */

#include "AHeader.h"
#include "HostHeader.h"
#include "Content_Length.h"

LobKo::AHeader::AHeader(const string& name, const string& value) :
name_(name),
value_(value) {
}

LobKo::AHeader::~AHeader() {
}

shared_ptr<LobKo::AHeader> LobKo::AHeader::makeHeader(const string& name, const string& value) {
    if ( name == "Host" ) {
        return shared_ptr<AHeader>(new HostHeader(name, value));
    }
    if ( name == "Content-Length" ) {
        return shared_ptr<AHeader>(new Content_Length(name, value));
    }
}

const string& LobKo::AHeader::getName() const {
    return name_;
}

const string& LobKo::AHeader::getValue() const {
    return value_;
}