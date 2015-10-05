/* 
 * File:   Content_Length.h
 * Author: s
 *
 * Created on June 22, 2015, 9:06 PM
 */

#ifndef CONTENT_LENGTH_H
#define	CONTENT_LENGTH_H

#include "AHeader.h"
#include <string>

using std::string;

namespace LobKo {
    class AHeader;

    class Content_Length : public AHeader {
    public:
        Content_Length(const string& name, const string& value);
        Content_Length(const Content_Length& orig);
        virtual ~Content_Length();

        unsigned int getValueAsDecimalNumber() const;
        static unsigned int getValueAsDecimalNumber(Content_Length& p);
        static const string& name();
    private:
    };
};

#endif	/* CONTENT_LENGTH_H */

