#pragma once

#include <spdlog/spdlog.h>

namespace Yolo
{
    class Logger
    {
    public:
        static void initialise();
        static void shutdown();

        inline static std::shared_ptr<spdlog::logger>& getLogger()
        {
            return mLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> mLogger;
    };
}

#define YOLO_TRACE(...) ::Yolo::Logger::getLogger()->trace(__VA_ARGS__)
#define YOLO_INFO(...) ::Yolo::Logger::getLogger()->info(__VA_ARGS__)
#define YOLO_WARN(...) ::Yolo::Logger::getLogger()->warn(__VA_ARGS__)
#define YOLO_ERROR(...) ::Yolo::Logger::getLogger()->error(__VA_ARGS__)

#define YOLO_ASSERT(x, ...)                                     \
    {                                                           \
        if (!x)                                                 \
        {                                                       \
            YOLO_ERROR("Assertion failed: {0}", __VA_ARGS__);   \
        }                                                       \
    }