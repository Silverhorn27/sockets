cmake_minimum_required(VERSION 3.1)
############################################################################################
# Project settings
set(APP_NAME sockets)
project(${APP_NAME})

set(PREFIX "src")

set(SOURCES 
    ${PREFIX}/main.cpp
    ${PREFIX}/Logger/Logger.h
    ${PREFIX}/Logger/Logger.cpp
    ${PREFIX}/Server/Server.h
    ${PREFIX}/Server/Server.cpp
    ${PREFIX}/Utils/StringUtils.h
    ${PREFIX}/Utils/StringUtils.cpp
    ${PREFIX}/definions.h
    ${PREFIX}/Application.h
    ${PREFIX}/Application.cpp
    ${PREFIX}/Mutlithreading/All.h
    ${PREFIX}/Mutlithreading/MultithreadingConstant.h
    ${PREFIX}/Mutlithreading/Runnable.h
    ${PREFIX}/Mutlithreading/Runnable.cpp
    ${PREFIX}/Mutlithreading/Thread.h
    ${PREFIX}/Mutlithreading/Thread.cpp
    ${PREFIX}/Mutlithreading/ThreadPool.h
    ${PREFIX}/Mutlithreading/ThreadPool.cpp)

add_executable(${PROJECT_NAME} "${SOURCES}")

add_definitions()

set(CMAKE_CXX_FLAGS "-std=c++17 -g -O0 -pthread -Wall -Wno-address -Wno-nonnull-compare -Wno-padded -Wreturn-type -Wno-sign-compare -Wno-unused-function")

include_directories(${PREFIX})

