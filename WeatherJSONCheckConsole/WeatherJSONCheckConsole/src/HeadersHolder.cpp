/* 
 * File:   HeaderHolder.cpp
 * Author: s
 * 
 * Created on June 16, 2015, 4:31 PM
 */

#include <assert.h>
#include "HeadersHolder.h"

LobKo::HeadersHolder::HeadersHolder() :
isResultActual_(false) {
    ;
}

LobKo::HeadersHolder::~HeadersHolder() {
    ;
}

void LobKo::HeadersHolder::add(shared_ptr<AHeader> sp) {
    list_.push_back(sp);
    isResultActual_ = false;
}

const string& LobKo::HeadersHolder::getAllHeaderWithValue() const {
    if ( isResultActual_ == false ) {
        isResultActual_ = true;

        list<shared_ptr<AHeader> >::const_iterator kIter = list_.begin();
        list<shared_ptr<AHeader> >::const_iterator kIterEnd = list_.end();

        result_ = "";
        for (; kIter != kIterEnd; ++kIter ) {
            assert((*kIter).get() && "HeadersHolder::getAllHeaderWithValue(), ZERO poiter");
            result_ = (*kIter)->getName();
            result_ += ": ";
            result_ += (*kIter)->getValue();
        }
    }
    return result_;
}