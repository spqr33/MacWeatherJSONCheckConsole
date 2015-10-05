/* 
 * File:   URL.cpp
 * Author: s
 * 
 * Created on June 12, 2015, 4:09 PM
 */

#include "URL.h"
#include <stdexcept>
#include "utils.h"

LobKo::URL::URL(const string& original_string) :
original_request_string_(original_string) {
    const char* p1;
    const char* p2;

    p1 = original_string.c_str();
    p2 = p1;
    if ( *p2 == '\0' ) {
        throw std::invalid_argument("received incorrect string");
    }
    //delete leading whitespaces
    while (*p2 == SymbolCode::ZERO ||
            *p2 == SymbolCode::TAB ||
            *p2 == SymbolCode::LF ||
            *p2 == SymbolCode::CR) {
        ++p2;
    }
    if ( *p2 == '\0' ) {
        throw std::invalid_argument("received incorrect string");
    }
    p1 = p2;
    //getting schema
    while (*p2 != ':') {
        //todo: a need to check schema
        if ( *p2 == '\0' ) {
            throw std::invalid_argument("received incorrect string");
        }
        ++p2;
    }
    schema_ = std::string(p1, p2 - p1);

    ++p2;
    for ( int i = 0; i < 2; ++i ) {
        if ( *p2 != '/' || *p2 == '\0' ) {
            throw std::invalid_argument("received incorrect string");
        }
        ++p2;
    }

    p1 = p2;
    const char* pColon = NULL;
    const char* pAt = NULL;
    const char* pColonTwice = NULL;
    states_hostname stateHostname = begin;

    for ( char ch; *p2 != '\0' && *p2 != '/'; ++p2 ) {
        ch = *p2;

        switch (stateHostname) {
            case begin:
                if ( isAllowedSymb(ch) ) {
                    stateHostname = allowedsym;
                } else {
                    throw std::invalid_argument("received incorrect string");
                };
                break;
            case allowedsym:
                if ( isAllowedSymb(ch) ) {
                    break;
                } else if ( ch == '@' ) {
                    pAt = p2;
                    stateHostname = allowedsym_at;
                } else if ( ch == ':' ) {
                    pColon = p2;
                    stateHostname = allowedsym_colon;
                } else {
                    throw std::invalid_argument("received incorrect string");
                };
                break;
            case allowedsym_at:
                if ( isAllowedSymb(ch) ) {
                    stateHostname = allowedsym_at_allowedsym;
                } else {
                    throw std::invalid_argument("received incorrect string");
                };
                break;
            case allowedsym_at_allowedsym:
                if ( isAllowedSymb(ch) ) {
                    break;
                } else if ( ch == ':' ) {
                    pColon = p2;
                    stateHostname = allowedsym_at_allowedsym_colon;

                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_at_allowedsym_colon:
                if ( isDigit(ch) ) {
                    stateHostname = allowedsym_at_allowedsym_colon_digit;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_at_allowedsym_colon_digit:
                if ( !isDigit(ch) ) {
                    throw std::invalid_argument("received incorrect string");
                }
                break; //user@host:[port] branch ended
            case allowedsym_colon:
                if ( isDigit(ch) ) {
                    stateHostname = allowedsym_colon_digit;
                } else if ( isAllowedSymb(ch) ) {
                    stateHostname = allowedsym_colon_allowedsym;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_colon_digit:
                if ( isDigit(ch) ) {
                    break;
                } else if ( isAllowedSymb(ch) ) {
                    stateHostname = allowedsym_colon_allowedsym;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_colon_allowedsym:
                if ( isAllowedSymb(ch) ) {
                    break;
                } else if ( ch == '@' ) {
                    pAt = p2;
                    stateHostname = allowedsym_colon_allowedsym_at;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_colon_allowedsym_at:
                if ( isAllowedSymb(ch) ) {
                    stateHostname = allowedsym_colon_allowedsym_at_allowedsym;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_colon_allowedsym_at_allowedsym:
                if ( isAllowedSymb(ch) ) {
                    break;
                } else if ( ch == ':' ) {
                    pColonTwice = p2;
                    stateHostname = allowedsym_colon_allowedsym_at_allowedsym_colon;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_colon_allowedsym_at_allowedsym_colon:
                if ( isDigit(ch) ) {
                    stateHostname = allowedsym_colon_allowedsym_at_allowedsym_colon_digit;
                } else {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            case allowedsym_colon_allowedsym_at_allowedsym_colon_digit:
                if ( !isDigit(ch) ) {
                    throw std::invalid_argument("received incorrect string");
                }
                break;
            default:
                throw std::invalid_argument("received incorrect string");
        }; //switch
    }; //for (; *p2 != '\0' && *p2 != '/'; ++p2 )
    //Not allowed states
    switch (stateHostname) {
        case allowedsym_at:
        case allowedsym_at_allowedsym_colon:
        case allowedsym_colon:
        case allowedsym_colon_allowedsym_at:
        case allowedsym_colon_allowedsym_at_allowedsym_colon:
            throw std::invalid_argument("received incorrect string, is ended by : or @");
            break;
    }
    //Allowed states processing
    if ( stateHostname == allowedsym ) {
        host_ = std::string(p1, p2 - p1);
        port_ = htons(Default::PORT);
    } else if ( stateHostname == allowedsym_at_allowedsym ) {
        username_ = std::string(p1, pAt - p1);
        host_ = std::string(pAt + 1, p2 - pAt - 1);
        port_ = htons(Default::PORT);
    } else if ( stateHostname == allowedsym_at_allowedsym_colon_digit ) {
        port_ = netOrderPortFromString(pColon + 1, p2 - 1);
        username_ = std::string(p1, pAt - p1);
        host_ = std::string(pAt + 1, pColon - pAt - 1);
    } else if ( stateHostname == allowedsym_colon_digit ) {
        host_ = std::string(p1, pColon - p1);
        port_ = netOrderPortFromString(pColon + 1, p2 - 1);
    } else if ( stateHostname == allowedsym_colon_allowedsym ) {
        port_ = netOrderPortFromString(pColon + 1, p2 - 1);
        host_ = std::string(p1, pColon - p1);
    } else if ( stateHostname == allowedsym_colon_allowedsym_at_allowedsym ) {
        username_ = std::string(p1, pColon - p1);
        password_ = std::string(pColon + 1, pAt - pColon - 1);
        host_ = std::string(pAt + 1, p2 - pAt - 1);
        port_ = htons(Default::PORT);
    } else if ( stateHostname == allowedsym_colon_allowedsym_at_allowedsym_colon_digit ) {
        port_ = netOrderPortFromString(pColonTwice + 1, p2 - 1);
        username_ = std::string(p1, pColon - p1);
        password_ = std::string(pColon + 1, pAt - pColon - 1);
        host_ = std::string(pAt + 1, pColonTwice - pAt - 1);
    }

    if ( *p2 == '\0' ) {
        path_ = "/"; //on *nix

        return;
    };
    p1 = p2;
    for ( ++p2; *p2 != '?' && *p2 != '\0'; ++p2 ) {
        ;
    }
    path_ = std::string(p1, p2 - p1);
    //todo: fill
    //LobKo::QuerySring query_string_;
    //string fragment_id;
}

LobKo::URL::~URL() {
    ;
}

const string & LobKo::URL::getHost() const {
    return host_;
}

const string & LobKo::URL::getPath() const {
    return path_;
}

const uint16_t& LobKo::URL::getPort() const {
    return port_;
}

std::ostream& LobKo::operator<<(std::ostream& out, const URL& url) {
    out << "___________________" << std::endl;
    out << "schema_:" << url.schema_ << ":\n";
    out << "username_:" << url.username_ << ":\n";
    out << "password_:" << url.password_ << ":\n";
    out << "host_:" << url.host_ << ":\n";
    out << "port_(big endian):" << std::dec << url.port_ << " port_(little endian):" << ntohs(url.port_) << ":\n";
    out << "path_:" << url.path_ << ":\n";
    out << "___________________" << std::endl;

    return out;
};

