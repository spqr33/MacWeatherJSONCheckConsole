/* 
 * File:   IPAdress.h
 * Author: s
 *
 * Created on June 12, 2015, 10:45 PM
 */

#ifndef IPADRESS_H
#define	IPADRESS_H

#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace LobKo {

    class IPAdress {
        friend std::ostream& operator<<(std::ostream&, IPAdress&);
    public:
        IPAdress();
        IPAdress(const IPAdress& orig);
        virtual ~IPAdress();
        void add(struct in_addr*);
        void addMultiple(struct in_addr**);
        in_addr_t getIP() const;
    private:
        std::list<struct in_addr> list_in_addr_;
    };

    std::ostream& operator<<(std::ostream&, const IPAdress&);
};
#endif	/* IPADRESS_H */

