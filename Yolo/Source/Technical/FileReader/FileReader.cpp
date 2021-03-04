#include "FileReader.hpp"

#include "Core/Core.hpp"

static constexpr char* EOL = "\n";

/* Todo: Create method isFinishedReading() */
namespace Yolo
{
    FileReader::FileReader(std::string fileContent)
        : mCurrentPosition(0), mFileContent(fileContent)
    {
    }

    std::optional<FileReader> FileReader::create(std::string filepath)
    {
        std::string fileContent;

        std::ifstream fileStream(filepath, std::ios::in);
        if (fileStream)
        {
            fileStream.seekg(0, std::ios::end);
            fileContent.resize(fileStream.tellg());

            fileStream.seekg(0, std::ios::beg);
            fileStream.read(&fileContent[0], fileContent.size());

            fileStream.close();

            return FileReader(fileContent);
        }

        YOLO_ERROR("File {0} not found!", filepath);

        return std::nullopt;
    }

    std::string FileReader::getNextLine()
    {
        size_t endOfLine = mFileContent.find_first_of(EOL, mCurrentPosition); /* Doesn't include new line character. */

        if (endOfLine != std::string::npos)
        {
            std::string line = mFileContent.substr(mCurrentPosition, endOfLine - mCurrentPosition);

            mCurrentPosition = endOfLine + strlen(EOL);
            return line;
        }

        return std::string();
    }
} // namespace Yolo
