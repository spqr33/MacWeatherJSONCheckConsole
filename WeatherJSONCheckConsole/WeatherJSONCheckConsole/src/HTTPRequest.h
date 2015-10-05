/* 
 * File:   HTTPRequest.h
 * Author: s
 *
 * Created on June 16, 2015, 2:27 PM
 */

#ifndef HTTPREQUEST_H
#define	HTTPREQUEST_H

#include <string>
#include <memory>
#include "URL.h"
#include "HeadersHolder.h"
#include "HTTPResponse.h"
#include "Action.h"

using std::shared_ptr;
using std::weak_ptr;

namespace LobKo {
    class HTTPResponse;

    class HTTPRequestType {
    public:

        enum Type {
            GET
        };
        explicit HTTPRequestType(HTTPRequestType::Type); //throw std::invalid_argument
        ~HTTPRequestType();
        const std::string& getRequestType() const;
    private:
        std::string requestType_;
    };

    class HTTPProto {
    public:

        enum Type {
            HTTP0_9, HTTP1_0, HTTP1_1
        };
        explicit HTTPProto(HTTPProto::Type); //throw std::invalid_argument
        ~HTTPProto();
        const std::string& getRequestProto() const;
        static const std::string& getRequestProtoByType(HTTPProto::Type);
    private:
        std::string protoType_;
    };

    class HTTPRequest {
    public:
        HTTPRequest(const HTTPRequestType& reqType, shared_ptr<URL> spURL, const HTTPProto& reqProto);
        ~HTTPRequest();

        void addHeader(shared_ptr<AHeader>);

        const string& getResultString() const;
        const shared_ptr<URL>& getURL() const;

        inline in_addr_t getLastUsedIp() const;
        inline void setLastUsedIp(in_addr_t);
        inline int bytesSent() const;
        inline void setBytesSent(int bytes);
        inline void setHTTPResponse(shared_ptr<HTTPResponse> sp);

        inline void setAction(shared_ptr<Action> newAction);
        inline const shared_ptr<Action> getAction() const;
    private:
        HTTPRequestType requestType_;
        HTTPProto requestProto_;
        shared_ptr<URL> spURL_;
        shared_ptr<HeadersHolder> headerHolder_;

        shared_ptr<Action> spAction_;

        mutable string resultString_;
        mutable bool isResultStringActual_;

        in_addr_t lastUsedIP_NetOrder_;
        int bytesSent_;

        std::weak_ptr<HTTPResponse> wpHTTPResponse_;
    };
};

in_addr_t LobKo::HTTPRequest::getLastUsedIp() const {
    return lastUsedIP_NetOrder_;
}

void LobKo::HTTPRequest::setLastUsedIp(in_addr_t ip) {
    lastUsedIP_NetOrder_ = ip;
}

int LobKo::HTTPRequest::bytesSent() const {
    return bytesSent_;
}

void LobKo::HTTPRequest::setBytesSent(int bytes) {
    bytesSent_ = bytes;
}

void LobKo::HTTPRequest::setHTTPResponse(shared_ptr<LobKo::HTTPResponse> sp) {
    wpHTTPResponse_ = sp;
}

void LobKo::HTTPRequest::setAction(shared_ptr<Action> newAction) {
    spAction_ = newAction;
}

const shared_ptr<LobKo::Action> LobKo::HTTPRequest::getAction() const {
    return spAction_;
}
#endif	/* HTTPREQUEST_H */

