#pragma once

#include "PreProcessor.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    class Core
    {
    public:
        inline static void initialise()
        {
            Logger::initialise();

            YOLO_INFO("Initialised Yolo's Core. Don't forget to shut it down!");
        }

        inline static void shutdown()
        {
            YOLO_INFO("Shut down Yolo's Core.");
            Logger::shutdown();
        }
    };
}