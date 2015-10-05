/* 
 * File:   JumboBuff.h
 * Author: s
 *
 * Created on June 20, 2015, 1:59 PM
 */

#ifndef JUMBOBUFF_H
#define	JUMBOBUFF_H

namespace LobKo {

    struct JumboBuff {
        explicit JumboBuff(unsigned int buffSize);
        ~JumboBuff();
        
        void reInit();

        const unsigned int buffSize_;
        const char* const start_;
        const char* currentPos_;
        const char* watermark_;
    private:
        JumboBuff(const JumboBuff& orig);
        JumboBuff& operator=(const JumboBuff& rhs);
    };
}

#endif	/* JUMBOBUFF_H */

