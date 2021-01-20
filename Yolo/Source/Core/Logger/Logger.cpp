#pragma once

#include "Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Yolo
{
    std::shared_ptr<spdlog::logger> Logger::mLogger;

    void Logger::initialise()
    {
        spdlog::set_pattern("%^[%T] %n %l: %v%$");
        mLogger = spdlog::stdout_color_mt("Yolo");
        mLogger->set_level(spdlog::level::trace);

        YOLO_INFO("Initialised Loggers");
    }

    void Logger::shutdown()
    {
        spdlog::shutdown();
    }
}