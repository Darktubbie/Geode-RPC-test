cmake_minimum_required(VERSION 3.21)

project(GeodeRPCTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK environment variable is not set")
endif()

include($ENV{GEODE_SDK}/cmake/Geode.cmake)

setup_geode_mod(${PROJECT_NAME})
