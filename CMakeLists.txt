cmake_minimum_required(VERSION 3.15)
project(Colorpick
    VERSION 0.1.0
    LANGUAGES CXX
)
string(TOLOWER ${PROJECT_NAME} PROJECT_ID)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual")
set(CMAKE_CXX_STANDARD 17) #Qt6 needs C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#===============================================================================
# Qt
#===============================================================================
set(PROJECT_QT_VERSION 5 CACHE STRING "Qt version to use: 5 (default) or 6")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt${PROJECT_QT_VERSION})
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED Core LinguistTools Widgets)
#===============================================================================
# Other dependencies
#===============================================================================
find_package(KF${QT_VERSION_MAJOR}GuiAddons CONFIG REQUIRED)
find_package(KF${QT_VERSION_MAJOR}WidgetsAddons CONFIG REQUIRED)
find_package(ECM REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
#===============================================================================
# Project files
#===============================================================================
set(PROJECT_HEADERS
    src/coloreditor.h
    src/colorpicker.h
    src/colorspace.h
    src/componenteditor.h
    src/contrastpreview.h
    src/hsvcolorspace.h
    src/imagegradientselector.h
    src/rgbcolorspace.h
    src/window.h
)
set(PROJECT_SOURCES
    src/main.cpp
    src/coloreditor.cpp
    src/colorpicker.cpp
    src/colorspace.cpp
    src/componenteditor.cpp
    src/contrastpreview.cpp
    src/hsvcolorspace.cpp
    src/imagegradientselector.cpp
    src/rgbcolorspace.cpp
    src/window.cpp
)
set(PROJECT_RESOURCES resources/app.qrc)
#===============================================================================
# Resources
#===============================================================================
include(ECMAddAppIcon)
ecm_add_app_icon(PROJECT_SOURCES ICONS
    resources/icons/1024-apps-${PROJECT_ID}.png
    resources/icons/256-apps-${PROJECT_ID}.png
    resources/icons/32-apps-${PROJECT_ID}.png
    resources/icons/sc-apps-${PROJECT_ID}.svg
)
source_group("Resource Files" FILES
    ${PROJECT_RESOURCES}
)
#===============================================================================
# Build
#===============================================================================
add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Widgets
    KF${QT_VERSION_MAJOR}::GuiAddons
    KF${QT_VERSION_MAJOR}::WidgetsAddons
)
if (UNIX AND NOT APPLE)
    include(GNUInstallDirs)
    set(PROJECT_DATA_DIR "${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_ID}")
else()
    #TODO: macOS and Windows
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_ID}")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DATA_DIR="${PROJECT_DATA_DIR}")
#===============================================================================
# Install
#===============================================================================
include(ECMInstallIcons)
if (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(FILES "resources/freedesktop/${PROJECT_ID}.desktop"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
    )
    ecm_install_icons(ICONS "sc-apps-${PROJECT_ID}.svg"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/icons"
    )
endif()
#===============================================================================
# Project information
#===============================================================================
message(STATUS "
Project name:                ${PROJECT_NAME}
Version:                     ${PROJECT_VERSION}
Qt version:                  ${QT_VERSION}
Build type:                  ${CMAKE_BUILD_TYPE}
Install prefix:              ${CMAKE_INSTALL_PREFIX}

CXX Debug flags:             ${CMAKE_CXX_FLAGS_DEBUG}
CXX Release flags:           ${CMAKE_CXX_FLAGS_RELEASE}
CXX MinSize flags:           ${CMAKE_CXX_FLAGS_MINSIZEREL}
CXX RelWithDebInfo flags:    ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}
")
