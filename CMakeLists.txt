cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Lightstreamer-lib-client-cpp VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
# Declared before extern/simple_color so this pin wins over the Catch2 it
# declares itself (FetchContent honours the first declaration).
FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.16.0
)

# Module scanning is only meant for this project's own sources. Catch2 (made
# available by extern/simple_color's tests) probes compiler flags with
# try_compile, and those probes fail to generate when scanning is inherited.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
add_subdirectory(extern/simple_color)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(LIGHTSTREAMER_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIGHTSTREAMER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


file(GLOB_RECURSE LIGHTSTREAMER_HEADER_FILES "include/*.hpp")
foreach(HEADER_FILE ${LIGHTSTREAMER_HEADER_FILES})
        message(STATUS "Found header file: ${HEADER_FILE}")
endforeach()

file(GLOB_RECURSE LIGHTSTREAMER_SRC_FILES "src/*.cpp")
foreach(SRC_FILE ${LIGHTSTREAMER_SRC_FILES})
        message(STATUS "Found src file: ${SRC_FILE}")
endforeach()


add_library(Lightstreamer SHARED
        ${LIGHTSTREAMER_SRC_FILES}
        ${LIGHTSTREAMER_HEADER_FILES}
#        include/lightstreamer/client/events/Event.hpp
#        include/lightstreamer/client/events/EventDispatcher.hpp
#        include/lightstreamer/client/events/EventsThread.hpp
)
#target_sources(Lightstreamer PRIVATE ${LIGHTSTREAMER_HEADER_FILES})

message(STATUS "LIGHTSTREAMER_INCLUDE_DIR: ${LIGHTSTREAMER_INCLUDE_DIR}")
target_include_directories(Lightstreamer
        PUBLIC
        ${LIGHTSTREAMER_HEADER_FILES}
        ${LIGHTSTREAMER_INCLUDE_DIR}/
        ${LIGHTSTREAMER_INCLUDE_DIR}/lightstreamer
        ${LIGHTSTREAMER_INCLUDE_DIR}/lightstreamer/client
        ${LIGHTSTREAMER_INCLUDE_DIR}/lightstreamer/util
        ${LIGHTSTREAMER_INCLUDE_DIR}/lightstreamer/events
        extern/simple_color/include
)
#include_directories(BEFORE
#        ${LIGHTSTREAMER_INCLUDE_DIR})


#target_compile_features(Lightstreamer PUBLIC cxx_std_20)
#target_compile_options(Lightstreamer PRIVATE -fmodules -std=gnu++20)

target_link_libraries(Lightstreamer PRIVATE simple_color)


option(LIGHTSTREAMER_BUILD_TESTS "Build the Lightstreamer tests" OFF)

if(LIGHTSTREAMER_BUILD_TESTS)
        FETCHCONTENT_GETPROPERTIES(Catch2)
        FETCHCONTENT_MAKEAVAILABLE(Catch2)
        add_subdirectory(tests)
endif()
