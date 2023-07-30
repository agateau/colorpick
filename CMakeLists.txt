cmake_minimum_required(VERSION 3.15)
project(Colorpick
    VERSION 0.1.0
    LANGUAGES CXX
)
string(TOLOWER ${PROJECT_NAME} PROJECT_ID)
set(APPSTREAM_ID "com.agateau.${PROJECT_ID}")

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
set(PROJECT_ALL_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
)
#===============================================================================
# Resources
#===============================================================================
if (UNIX AND NOT APPLE)
    include(ECMAddAppIcon)
    ecm_add_app_icon(PROJECT_SOURCES ICONS
        resources/icons/1024-apps-${PROJECT_ID}.png
        resources/icons/256-apps-${PROJECT_ID}.png
        resources/icons/32-apps-${PROJECT_ID}.png
        resources/icons/sc-apps-${PROJECT_ID}.svg
    )
endif()
source_group("Resource Files" FILES ${PROJECT_RESOURCES})
#===============================================================================
# Build
#===============================================================================
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_ALL_FILES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_ALL_FILES})
endif()

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
include(GNUInstallDirs)
if (UNIX AND NOT APPLE)
    set(PROJECT_DATA_DIR "${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_ID}") # Needed to find translations
else()
    #TODO: macOS and Windows translation system dirs or as bundled resource?
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_ID}")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DATA_DIR="${PROJECT_DATA_DIR}")
#===============================================================================
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
#
# NOTE: there is no need for if(WIN32 OR APPLE)
#===============================================================================
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER "${APPSTREAM_ID}")
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
#===============================================================================
# Install
#===============================================================================
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if (UNIX AND NOT APPLE)
    install(FILES "resources/freedesktop/${PROJECT_ID}.desktop"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
    )
    include(ECMInstallIcons)
    ecm_install_icons(ICONS "sc-apps-${PROJECT_ID}.svg"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/icons"
    )
else()
    # TODO: macOS and Windows icons?
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
