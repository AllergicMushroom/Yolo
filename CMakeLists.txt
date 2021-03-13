cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("Libraries/spdlog-1.8.2")
include_directories("Libraries/spdlog-1.8.2/include/")

add_subdirectory("Libraries/googletest-1.10.0")
include_directories("Libraries/googletest-1.10.0/include")

set_target_properties(spdlog gtest gtest_main PROPERTIES FOLDER Dependencies)

project("Yolo" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage -DYOLO_MODE_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DYOLO_MODE_RELEASE")
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    set(CMAKE_CXX_FLAGS "/W4 /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Od /DYOLO_MODE_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /DYOLO_MODE_RELEASE")
endif()

#### Source directories ####

set(SRC_ROOT "Yolo/Source")
include_directories("${SRC_ROOT}")

############################

#### Executables ####

#### Library ####

file(GLOB_RECURSE LIB_FILES
    "${SRC_ROOT}/Core/**.hpp"
    "${SRC_ROOT}/Core/**.cpp"

    "${SRC_ROOT}/Domain/**.cpp"
    "${SRC_ROOT}/Domain/**.cpp"

    "${SRC_ROOT}/Technical/**.cpp"
    "${SRC_ROOT}/Technical/**.cpp"
)

set(YOLO_LIBRARY "Yolo")
add_library(${YOLO_LIBRARY} STATIC ${LIB_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIB_FILES})

set(SANDBOX_APP_NAME "Sandbox")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${SANDBOX_APP_NAME})

#################

set(COMMON_LIBRARIES "spdlog")

#### Sandbox for developping purposes ####

file(GLOB_RECURSE SANDBOX_FILES
    "${SRC_ROOT}/UserInterface/Sandbox.cpp"
)

add_executable(${SANDBOX_APP_NAME} ${SANDBOX_FILES})
target_link_libraries(${SANDBOX_APP_NAME} ${YOLO_LIBRARY} ${COMMON_LIBRARIES})

set_target_properties(${SANDBOX_APP_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$(configuration))

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SANDBOX_FILES})

if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_custom_command(TARGET ${SANDBOX_APP_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Instances" "$<TARGET_FILE_DIR:${SANDBOX_APP_NAME}>/Instances")
else()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Instances" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

##########################################

#### Unit tests ####

file(GLOB_RECURSE TEST_FILES
    "${SRC_ROOT}/UserInterface/UnitTests/**.hpp"
    "${SRC_ROOT}/UserInterface/UnitTests/**.cpp"
)

set(UNIT_TESTS_APP_NAME "UnitTests")
add_executable(${UNIT_TESTS_APP_NAME} ${TEST_FILES})
target_link_libraries(${UNIT_TESTS_APP_NAME} ${YOLO_LIBRARY} ${COMMON_LIBRARIES} gtest)

set_target_properties(${UNIT_TESTS_APP_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$(configuration))

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_FILES})

if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_custom_command(TARGET ${UNIT_TESTS_APP_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E "copy_directory" "${CMAKE_CURRENT_SOURCE_DIR}/Instances" "$<TARGET_FILE_DIR:${UNIT_TESTS_APP_NAME}>/Instances")
else()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Instances" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

####################

#### Command line interface ####

file(GLOB_RECURSE CLI_FILES
    "${SRC_ROOT}/UserInterface/CLI/**.hpp"
    "${SRC_ROOT}/UserInterface/CLI/**.cpp"
)

set(CLI_APP_NAME "CLI")
add_executable(${CLI_APP_NAME} ${CLI_FILES})
target_link_libraries(${CLI_APP_NAME} ${YOLO_LIBRARY} ${COMMON_LIBRARIES})

set_target_properties(${CLI_APP_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$(configuration))

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CLI_FILES})

if (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_custom_command(TARGET ${CLI_APP_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Instances" "$<TARGET_FILE_DIR:${CLI_APP_NAME}>/Instances")
else()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Instances" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

################################

#####################
