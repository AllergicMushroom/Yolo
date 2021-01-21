cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("Libraries/spdlog-1.8.2")
include_directories("Libraries/spdlog-1.8.2/include/")

project("Yolo" LANGUAGES CXX)
set(APP_NAME "Yolo")
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    set(CMAKE_CXX_FLAGS "/W4 /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
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
    "${SRC_ROOT}/**.hpp"
    "${SRC_ROOT}/**.cpp"
)

# Executables
add_executable(${APP_NAME} ${SRC_FILES})
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

# Coverage tests
# include(CTest)
# set(GLOBAl_PROPERTIES PROPERTIES TIMEOUT 30)
