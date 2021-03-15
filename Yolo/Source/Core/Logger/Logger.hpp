#pragma once

#include <spdlog/spdlog.h>

namespace Yolo
{
    class Logger
    {
    private:
        Logger() = delete;

    public:
        static void initialize();
        static void shutdown();

        inline static std::shared_ptr<spdlog::logger>& getLogger()
        {
            return mLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> mLogger;
    };
} // namespace Yolo

#ifndef YOLO_DISABLE_LOGGING
    #define YOLO_DEBUG(...) ::Yolo::Logger::getLogger()->debug(__VA_ARGS__)
    #define YOLO_TRACE(...) ::Yolo::Logger::getLogger()->trace(__VA_ARGS__)
    #define YOLO_INFO(...) ::Yolo::Logger::getLogger()->info(__VA_ARGS__)
    #define YOLO_WARN(...) ::Yolo::Logger::getLogger()->warn(__VA_ARGS__)
    #define YOLO_ERROR(...) ::Yolo::Logger::getLogger()->error(__VA_ARGS__)
#endif
