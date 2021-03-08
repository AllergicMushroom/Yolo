#pragma once

#include "PreProcessor.hpp"

#include "Core/Logger/Logger.hpp"

// Todo: smart pointers
namespace Yolo
{
    class Core
    {
    public:
        inline static void initialize()
        {
            Logger::initialize();

            YOLO_DEBUG("Initialised Yolo's Core. Don't forget to shut it down!");
        }

        inline static void shutdown()
        {
            YOLO_DEBUG("Shut down Yolo's Core.");
            Logger::shutdown();
        }
    };
} // namespace Yolo
