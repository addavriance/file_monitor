cmake_minimum_required(VERSION 3.25)
project(file_monitor)

set(CMAKE_CXX_STANDARD 17)

add_executable(file_monitor main.cpp FileCriteria.cpp FileCriteria.h FileMonitor.cpp FileMonitor.h)
