cmake_minimum_required(VERSION 3.21)

project(GeodeRPCTest VERSION 0.1.0)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GEODE_SDK_INCLUDE}
)

target_link_libraries(${PROJECT_NAME}
    ${GEODE_SDK_LIBRARY}
)
