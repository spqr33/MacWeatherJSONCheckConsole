/* 
 * File:   IPAdress.cpp
 * Author: s
 * 
 * Created on June 12, 2015, 10:45 PM
 */

#include "IPAdress.h"

LobKo::IPAdress::IPAdress() {
    ;
}


LobKo::IPAdress::~IPAdress() {
    ;
}

void LobKo::IPAdress::add(struct in_addr* p) {
    list_in_addr_.push_front(*p);
}

void LobKo::IPAdress::addMultiple(struct in_addr** pp) {
    for (; *pp != NULL; ++pp ) {
        list_in_addr_.push_front(**pp);
    }
}

in_addr_t LobKo::IPAdress::getIP() const {
    if ( list_in_addr_.empty() ) {
        return 0;
    } else {
         return (*list_in_addr_.begin()).s_addr;
    }
}

std::ostream& LobKo::operator<<(std::ostream& out, IPAdress& ipAdress) {
    std::list<struct in_addr>::iterator Iter = ipAdress.list_in_addr_.begin();
    std::list<struct in_addr>::iterator IterEND = ipAdress.list_in_addr_.end();

    out << "Recors count: " << ipAdress.list_in_addr_.size() << std::endl;
    for (; Iter != ipAdress.list_in_addr_.end(); ) {
        in_addr_t i = (*Iter).s_addr;
        out << i << std::endl;
        ++Iter;
    }

    return out;
}