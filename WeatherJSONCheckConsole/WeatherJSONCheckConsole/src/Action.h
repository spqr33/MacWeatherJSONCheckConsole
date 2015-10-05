/* 
 * File:   Action.h
 * Author: s
 *
 * Created on June 25, 2015, 2:18 PM
 */

#ifndef ACTION_H
#define	ACTION_H

#include <cstdint>

namespace LobKo {

    class Action {
    public:

        enum result {
            NOT_ALL_DATA_RCVD, ALL_DATA_RCVD, ERROR_OCCURED
        };
        Action();
        virtual ~Action();

        virtual result takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totaDataSize) = 0;
    private:
        Action(const Action& orig);
    };
};
#endif	/* ACTION_H */

