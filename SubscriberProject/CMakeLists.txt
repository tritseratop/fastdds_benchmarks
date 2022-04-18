# CMakeList.txt : CMake project for SubscriberProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(SubscriberProject)

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

file(GLOB SUBSCRIBER_PROJECT_CPP
        "main.cpp"
        "DdsService/*.cpp" 
        "DdsService/*.h"
        "../include/*.cpp"
        "../include/TimeConverter/*.cpp"
        "../include/logger/*.cpp"
        "ThreadSafeQueue/*.h" 
        "../ConfigTopic/*.cxx" 
        "../TypeTopicsDDS/DDSAlarm/*.cxx" 
        "../TypeTopicsDDS/DDSAlarmEx/*.cxx" 
        "../TypeTopicsDDS/DDSData/*.cxx" 
        "../BenchmarkTopics/*.cxx" 
        "../TypeTopicsDDS/DDSDataEx/*.cxx" 
)

# Add source to this project's executable.
add_executable (SubscriberProject  ${SUBSCRIBER_PROJECT_CPP})
target_link_libraries(SubscriberProject 
        PUBLIC fastrtps 
        PUBLIC fastcdr
)

# TODO: Add tests and install targets if needed.
