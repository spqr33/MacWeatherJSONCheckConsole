/* 
 * File:   SaveFile.h
 * Author: s
 *
 * Created on June 25, 2015, 2:18 PM
 */

#ifndef SAVEFILE_H
#define	SAVEFILE_H

#include "Action.h"
#include <string>
#include <fstream>
#include <cstdint>

namespace LobKo {

    class SaveFile : public Action {
    public:
        explicit SaveFile(const std::string& saveTo_FileName); //throw std::runtime_error
        virtual ~SaveFile();

        virtual result takeData(const char* const startBuf, const char* const endBuf, std::uint64_t totaDataSize);

        inline const std::string& get_file_name() const;
    private:
        SaveFile(const SaveFile& orig);

        std::uint64_t alreadyBytesSaved;
        std::ofstream fout;
        std::string saveTo_FileName_;
    };
};

const std::string& LobKo::SaveFile::get_file_name() const {
    return saveTo_FileName_;
}
#endif	/* SAVEFILE_H */

