/* 
 * File:   Content_Length.cpp
 * Author: s
 * 
 * Created on June 22, 2015, 9:06 PM
 */

#include "Content_Length.h"

LobKo::Content_Length::Content_Length(const string& name, const string& value) :
AHeader(name, value) {
}

LobKo::Content_Length::~Content_Length() {

}

const string& LobKo::Content_Length::name() {
    static const string name = "Content-Length";

    return name;
}

unsigned int LobKo::Content_Length::getValueAsDecimalNumber() const {
    char string_size = value_.size();

    unsigned int result_number = 0;
    unsigned int multiplicator = 1;
    for ( char i = string_size - 1; i >= 0; --i ) {
        result_number += (value_[i] - '0') * multiplicator;
        multiplicator *= 10;
    }

    return result_number;
}

unsigned int LobKo::Content_Length::getValueAsDecimalNumber(Content_Length& p) {
    char string_size = p.value_.size();

    unsigned int result_number = 0;
    unsigned int multiplicator = 1;
    for ( char i = string_size - 1; i >= 0; --i ) {
        result_number += (p.value_[i] - '0') * multiplicator;
        multiplicator *= 10;
    }

    return result_number;
}
