cmake_minimum_required(VERSION 3.1)
############################################################################################
# Project settings
set(APP_NAME application)
project(${APP_NAME})

add_subdirectory(Client)
add_subdirectory(Server)

add_custom_target(application)
add_dependencies(application server client)
