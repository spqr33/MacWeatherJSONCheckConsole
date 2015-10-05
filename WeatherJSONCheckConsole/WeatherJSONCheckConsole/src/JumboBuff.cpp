/* 
 * File:   JumboBuff.cpp
 * Author: s
 * 
 * Created on June 20, 2015, 1:59 PM
 */

#include "JumboBuff.h"

LobKo::JumboBuff::JumboBuff(unsigned int buffSize) :
buffSize_(buffSize),
start_(new char[buffSize]),
currentPos_(start_),
watermark_(start_) {
}

LobKo::JumboBuff::~JumboBuff() {
    delete [] start_;
}

void LobKo::JumboBuff::reInit() {
    currentPos_ = start_;
    watermark_ = start_;
}