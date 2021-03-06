﻿# CMakeList.txt : CMake project for PublisherProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(PublisherProject)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

# Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

message(STATUS "Configuring HelloWorld example...")

file(GLOB DDS_PUBLISHER_CPP 
        "../BenchmarkTopics/*.cxx" 
        "../ConfigTopic/*.cxx" 
        "../TypeTopicsDDS/DDSAlarm/*.cxx" 
        "../TypeTopicsDDS/DDSAlarmEx/*.cxx" 
        "../TypeTopicsDDS/DDSData/*.cxx" 
        "../TypeTopicsDDS/DDSDataEx/*.cxx" 
        "PublisherFactory.cpp" 
        "*.cpp" 
        "../include/TimeConverter/*.cpp"
        "../include/*.cpp"
)

# Add source to this project's executable.
add_executable (PublisherProject  ${DDS_PUBLISHER_CPP})
target_link_libraries(PublisherProject fastrtps fastcdr)

# TODO: Add tests and install targets if needed.
