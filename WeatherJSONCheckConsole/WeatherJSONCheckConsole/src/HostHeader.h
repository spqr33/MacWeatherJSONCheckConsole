/* 
 * File:   HostHeader.h
 * Author: s
 *
 * Created on June 16, 2015, 4:19 PM
 */

#ifndef HOSTHEADER_H
#define	HOSTHEADER_H

#include "AHeader.h"
#include <string>

using std::string;

namespace LobKo {
    class AHeader;
    
    class HostHeader : public AHeader {
    public:
        HostHeader(const string& name, const string& value);
        HostHeader(const HostHeader& orig);
        virtual ~HostHeader();
        
        static const string& name();
    private:

    };
};

#endif	/* HOSTHEADER_H */

