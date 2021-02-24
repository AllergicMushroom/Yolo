cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("Libraries/spdlog-1.8.2")
include_directories("Libraries/spdlog-1.8.2/include/")

add_subdirectory("Libraries/googletest-1.10.0")
include_directories("Libraries/googletest-1.10.0/include")

project("Yolo" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    set(CMAKE_CXX_FLAGS "/W4 /EHsc /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "/DEBUG:FULL /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif()

option(ENABLE_DEBUG "Debug option" OFF)
if (ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE DEBUG)
    add_definitions(-DYOLO_MODE_DEBUG)
else()
    set(CMAKE_BUILD_TYPE RELEASE)
    add_definitions(-DYOLO_MODE_RELEASE)
endif()

# Source files
set(SRC_ROOT "Yolo/Source")
include_directories("${SRC_ROOT}")

file(GLOB_RECURSE SRC_FILES
    "${SRC_ROOT}/Core/**.hpp"
    "${SRC_ROOT}/Core/**.cpp"

    "${SRC_ROOT}/Domain/**.cpp"
    "${SRC_ROOT}/Domain/**.cpp"

    "${SRC_ROOT}/Technical/**.cpp"
    "${SRC_ROOT}/Technical/**.cpp"
)

file(GLOB_RECURSE SANDBOX_FILES
    "${SRC_ROOT}/UserInterface/Sandbox.cpp"
)

file(GLOB_RECURSE TEST_FILES
    "${SRC_ROOT}/UnitTests/**.hpp"
    "${SRC_ROOT}/UnitTests/**.cpp"
)

# Executables
set(APP_NAME "Yolo")

add_executable(${APP_NAME} ${SRC_FILES} ${SANDBOX_FILES})
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES} ${SANDBOX_FILES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Instances
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(TEST_APP_NAME "YoloUnitTests")

add_executable(${TEST_APP_NAME} ${SRC_FILES} ${TEST_FILES})
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${TEST_APP_NAME})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES} ${TEST_FILES})
target_link_libraries(${TEST_APP_NAME} PUBLIC gtest)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Instances
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
