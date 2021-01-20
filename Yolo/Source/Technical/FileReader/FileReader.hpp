#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace Yolo
{
    class FileReader
    {
    private:
        FileReader(std::string content);

    public:
        static std::optional<FileReader> create(std::string filepath);

        inline std::string getContent() const { return mFileContent; }

        std::optional<std::string> getNextLine();

    private:
        std::string mFileContent;

        size_t mCurrentPosition;
    };
}