#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace Yolo
{
    class FileReader
    {
    public:
        FileReader(std::string filepath);
        
        inline bool hasFile() const { return mHasFile; }

        inline std::string getContent() const { return mFileContent; }

        std::optional<std::string> getNextLine();

    private:
        bool mHasFile;

        std::string mFileContent;

        size_t mCurrentPosition;
    };
}