#include "utils.h"
#include <iostream>
#include <iomanip>
#include <sys/socket.h> // struct in_addr
#include <netinet/in.h>
#include <arpa/inet.h>  // inet_ntop
#include <netdb.h>      // gethostbyname

std::pair<LobKo::DNSResult, std::shared_ptr<LobKo::IPAdress> >
LobKo::DomainNameResolver::operator ()(const string& host) {
    using namespace std;

    shared_ptr<IPAdress> spIPAddress(new IPAdress());

    struct hostent* pHostEnt;
    pHostEnt = gethostbyname(host.c_str());
    if ( pHostEnt == NULL ) {
#ifndef NDEBUG
        cerr << "Resolving " << host << " failed" << endl;
#endif  
        return std::make_pair(DNSResult::DNS_RESOLVE_ERROR, spIPAddress);
    } else {
#ifndef NDEBUG
        cout << "Resolving " << host << ":" << endl;

        struct in_addr** pp_in_addr_list;
        pp_in_addr_list = (struct in_addr**) pHostEnt->h_addr_list;

        while (*pp_in_addr_list != NULL) {
            char buff[INET_ADDRSTRLEN + 1];
            const char *c = inet_ntop(AF_INET, *(pp_in_addr_list), buff, INET_ADDRSTRLEN + 1);

            cout << buff << " ";
            pp_in_addr_list++;
        };
        cout << endl;
#endif
        spIPAddress.get()->addMultiple((struct in_addr**) pHostEnt->h_addr_list);

        return std::make_pair(DNSResult::DNS_RESOLVE_SUCCESS, spIPAddress);
    }
}
