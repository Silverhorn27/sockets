cmake_minimum_required(VERSION 3.1)
############################################################################################
# Project settings
set(APP_NAME client)
project(${APP_NAME})

set(PREFIX "src")
set(PREFIXSOURCE "../Common")


set(SOURCES 
    ${PREFIX}/main.cpp
    ${PREFIX}/AppClient.h
    ${PREFIX}/AppClient.cpp
    ${PREFIX}/Client/Client.h
    ${PREFIX}/Client/Client.cpp
    ${PREFIXSOURCE}/ConnectionHandler.h
    ${PREFIXSOURCE}/ConnectionHandler.cpp
    ${PREFIXSOURCE}/definions.h
    ${PREFIXSOURCE}/Filesystem/FilesystemInteractor.h
    ${PREFIXSOURCE}/Filesystem/FilesystemInteractor.cpp
    ${PREFIXSOURCE}/Filesystem/FilesystemInteractorFactory.h
    ${PREFIXSOURCE}/Filesystem/FilesystemInteractorFactory.cpp
    ${PREFIXSOURCE}/Logger/Logger.h
    ${PREFIXSOURCE}/Logger/Logger.cpp
    ${PREFIXSOURCE}/Utils/StringUtils.h
    ${PREFIXSOURCE}/Utils/StringUtils.cpp
    ${PREFIXSOURCE}/Utils/ConfigReader.h
    ${PREFIXSOURCE}/Utils/ConfigReader.cpp
    ${PREFIXSOURCE}/Application/Application.h
    ${PREFIXSOURCE}/Application/Application.cpp
    ${PREFIXSOURCE}/Mutlithreading/All.h
    ${PREFIXSOURCE}/Mutlithreading/MultithreadingConstant.h
    ${PREFIXSOURCE}/Mutlithreading/Runnable.h
    ${PREFIXSOURCE}/Mutlithreading/Runnable.cpp
    ${PREFIXSOURCE}/Mutlithreading/Thread.h
    ${PREFIXSOURCE}/Mutlithreading/Thread.cpp
    ${PREFIXSOURCE}/Mutlithreading/ThreadPool.h
    ${PREFIXSOURCE}/Mutlithreading/ThreadPool.cpp)

add_executable(${PROJECT_NAME} "${SOURCES}")

add_definitions()

set(CMAKE_CXX_FLAGS "-std=c++17 -g -O0 -pthread -Wall -Wno-address -Wno-nonnull-compare -Wno-padded -Wreturn-type -Wno-sign-compare -Wno-unused-function")

include_directories(${PREFIX})
include_directories(${PREFIXSOURCE})

