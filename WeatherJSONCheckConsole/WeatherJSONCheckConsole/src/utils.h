/* 
 * File:   utils.h
 * Author: s
 *
 * Created on June 12, 2015, 5:03 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <utility>
#include <memory>
#include <string>
#include "IPAdress.h"

using std::string;

namespace LobKo {

    enum SymbolCode {
        ZERO = '\0', TAB = '\t', LF = '\n', CR = '\r'
    };

    enum DNSResult {
        DNS_RESOLVE_ERROR = -1, DNS_RESOLVE_SUCCESS
    };

    struct DomainNameResolver {
        std::pair<DNSResult, std::shared_ptr<IPAdress> > operator()(const string& host);
    };
}



#endif	/* UTILS_H */

