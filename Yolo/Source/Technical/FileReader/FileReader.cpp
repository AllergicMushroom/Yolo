#include "FileReader.hpp"

#include "Core/Core.hpp"

static constexpr char* EOL = "\n";

namespace Yolo
{
    FileReader::FileReader(std::string filepath):
        mCurrentPosition(0)
    {
        std::ifstream fileStream(filepath, std::ios::in);
        if (fileStream)
        {
            fileStream.seekg(0, std::ios::end);
            mFileContent.resize(fileStream.tellg());

            fileStream.seekg(0, std::ios::beg);
            fileStream.read(&mFileContent[0], mFileContent.size());

            fileStream.close();

            mHasFile = true;
        }
        else
        {
            YOLO_ERROR("File {0} not found!", filepath);
            mHasFile = false;
        }
    }

    std::optional<std::string> FileReader::getNextLine()
    {
        size_t endOfLine = mFileContent.find_first_of(EOL, mCurrentPosition); /* Doesn't include new line character. */
        
        if (endOfLine != std::string::npos)
        {
            std::string line = mFileContent.substr(mCurrentPosition, endOfLine - mCurrentPosition);

            mCurrentPosition = endOfLine + strlen(EOL);
            return line;
        }
     
        return std::nullopt;
    }
}