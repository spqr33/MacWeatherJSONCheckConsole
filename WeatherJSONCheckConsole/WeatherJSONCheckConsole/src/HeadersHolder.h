/* 
 * File:   HeaderHolder.h
 * Author: s
 *
 * Created on June 16, 2015, 4:31 PM
 */

#ifndef HEADERHOLDER_H
#define	HEADERHOLDER_H

#include <list>
#include <memory>
#include "AHeader.h"

using std::list;
using std::shared_ptr;

namespace LobKo {

    class HeadersHolder {
    public:
        HeadersHolder();
        virtual ~HeadersHolder();

        void add(shared_ptr<AHeader>);
        const string& getAllHeaderWithValue() const;
    private:
        list<shared_ptr<AHeader> > list_;
        
        mutable bool isResultActual_;
        mutable string result_;
    };
};

#endif	/* HEADERHOLDER_H */

