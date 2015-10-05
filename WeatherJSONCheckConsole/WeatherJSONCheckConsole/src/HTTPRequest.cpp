/* 
 * File:   HTTPRequest.cpp
 * Author: s
 * 
 * Created on June 16, 2015, 2:27 PM
 */

#include "HTTPRequest.h"
#include "utils.h"
#include "HostHeader.h"

LobKo::HTTPRequest::HTTPRequest(const HTTPRequestType& reqType,
        shared_ptr<URL> spURL,
        const HTTPProto& reqProto) :
requestType_(reqType),
requestProto_(reqProto),
spURL_(spURL),
isResultStringActual_(false),
headerHolder_(new HeadersHolder()),
lastUsedIP_NetOrder_(0),
bytesSent_(0)
{
    if ( requestProto_.getRequestProto() == HTTPProto::getRequestProtoByType(HTTPProto::Type::HTTP1_1) ) {
        shared_ptr<AHeader> sp = AHeader::makeHeader(HostHeader::name(), spURL_->getHost());
        headerHolder_->add(sp);
    }
}

LobKo::HTTPRequest::~HTTPRequest() {
}

const string& LobKo::HTTPRequest::getResultString() const {
    if ( isResultStringActual_ == false ) {
        isResultStringActual_ = true;
        //process

        resultString_ = requestType_.getRequestType();
        resultString_ += " ";
        resultString_ += spURL_.get()->getPath();
        resultString_ += " ";
        resultString_ += requestProto_.getRequestProto();
        resultString_ += SymbolCode::CR;
        resultString_ += SymbolCode::LF;
        resultString_ += headerHolder_->getAllHeaderWithValue();
        resultString_ += SymbolCode::CR;
        resultString_ += SymbolCode::LF;
        resultString_ += SymbolCode::CR;
        resultString_ += SymbolCode::LF;
    }

    return resultString_;
}

const shared_ptr<LobKo::URL>& LobKo::HTTPRequest::getURL() const {
    return spURL_;
}

void LobKo::HTTPRequest::addHeader(shared_ptr<AHeader> sp) {
    headerHolder_->add(sp);
    isResultStringActual_ = false;
}


/////////////////class HTTPRequestType

LobKo::HTTPRequestType::HTTPRequestType(HTTPRequestType::Type type) {//throw std::invalid_argument
    switch (type) {
        case GET:
            requestType_ = "GET";
            break;
        default:
            throw std::invalid_argument("Unknown request type");
    }
}

LobKo::HTTPRequestType::~HTTPRequestType() {
    ;
}

const std::string& LobKo::HTTPRequestType::getRequestType() const {
    return requestType_;
}

/////////////////class HTTPProto

LobKo::HTTPProto::HTTPProto(HTTPProto::Type type) { //throw std::invalid_argument
    switch (type) {
        case HTTP0_9:
            protoType_ = "HTTP/0.9";
            break;
        case HTTP1_0:
            protoType_ = "HTTP/1.0";
            break;
        case HTTP1_1:
            protoType_ = "HTTP/1.1";
            break;
        default:
            throw std::invalid_argument("Unknown request proto");
    }
}

LobKo::HTTPProto::~HTTPProto() {
    ;
}

const std::string& LobKo::HTTPProto::getRequestProto() const {
    return protoType_;
}

const std::string& LobKo::HTTPProto::getRequestProtoByType(HTTPProto::Type type) {
    static string s0_9 = "HTTP/0.9";
    static string s1_0 = "HTTP/1.0";
    static string s1_1 = "HTTP/1.1";
    static string epmty = "";


    switch (type) {
        case HTTP0_9:
            return s0_9;
        case HTTP1_0:
            return s1_0;
        case HTTP1_1:
            return s1_1;
        default:
            return epmty;
    };
}