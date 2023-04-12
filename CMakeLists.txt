cmake_minimum_required(VERSION 3.0)
project(colorpick)

set(APP_NAME ${PROJECT_NAME})

find_package(Qt5 CONFIG REQUIRED Core Widgets)
find_package(KF5GuiAddons CONFIG REQUIRED)
find_package(KF5WidgetsAddons CONFIG REQUIRED)
find_package(ECM REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")

add_subdirectory(src)
add_subdirectory(desktop)
