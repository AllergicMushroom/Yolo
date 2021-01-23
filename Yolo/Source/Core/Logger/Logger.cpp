#pragma once

#include "Logger.hpp"

#include "Core/PreProcessor.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Yolo
{
    std::shared_ptr<spdlog::logger> Logger::mLogger;

    void Logger::initialize()
    {
        spdlog::set_pattern("%^[%T] %n %l: %v%$");
        mLogger = spdlog::stdout_color_mt("Yolo");

        if constexpr(sCompileMode == CompileMode::Debug || sCompileMode == CompileMode::DebugOptOn)
        {
            mLogger->set_level(spdlog::level::trace);
        }
        else
        {
            mLogger->set_level(spdlog::level::info);
        }

        YOLO_DEBUG("Initialised Logger.");
    }

    void Logger::shutdown()
    {
        YOLO_DEBUG("Shut down Logger.");
        spdlog::shutdown();
    }
}