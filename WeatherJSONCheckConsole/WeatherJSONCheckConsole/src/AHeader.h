/* 
 * File:   AHeader.h
 * Author: s
 *
 * Created on June 16, 2015, 4:08 PM
 */

#ifndef AHEADER_H
#define	AHEADER_H

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

namespace LobKo {

    class AHeader {
    public:
        AHeader(const string& name, const string& value);
        virtual ~AHeader() = 0;

        virtual const string& getName() const;
        virtual const string& getValue() const;
        
        static shared_ptr<AHeader> makeHeader(const string& name, const string& value);
    protected:
        string name_;
        string value_;
    };
};

#endif	/* AHEADER_H */

