project(colorpick)
cmake_minimum_required(VERSION 3.0)

find_package(Qt5 CONFIG REQUIRED Core Widgets)
find_package(KF5GuiAddons CONFIG REQUIRED)
find_package(KF5WidgetsAddons CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")

add_subdirectory(src)
