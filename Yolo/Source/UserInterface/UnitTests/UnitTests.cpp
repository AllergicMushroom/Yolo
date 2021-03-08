#include "Core/Core.hpp"

#include "gtest/gtest.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    Yolo::Core::initialize();

    auto returnValue = RUN_ALL_TESTS();

    Yolo::Core::shutdown();

    return returnValue;
}
