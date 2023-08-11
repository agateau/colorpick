cmake_minimum_required(VERSION 3.15)
project(Colorpick
    VERSION 0.1.0
    LANGUAGES CXX
)
string(TOLOWER ${PROJECT_NAME} PROJECT_ID)

set(CMAKE_CXX_STANDARD 17) #Qt6 needs C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Qt
#
option(PROJECT_TRANSLATIONS_UPDATE "Update source translations [default: OFF]" OFF)
set(PROJECT_QT_VERSION 5 CACHE STRING "Qt version to use: 5 (default) or 6")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt${PROJECT_QT_VERSION})
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED Core LinguistTools Widgets)

#
# Other dependencies
#
find_package(KF${QT_VERSION_MAJOR}GuiAddons CONFIG REQUIRED)
find_package(KF${QT_VERSION_MAJOR}WidgetsAddons CONFIG REQUIRED)
find_package(ECM REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

#
# Project files
#
set(PROJECT_SOURCES
    src/coloreditor.h
    src/coloreditor.cpp
    src/colorpicker.h
    src/colorpicker.cpp
    src/colorspace.h
    src/colorspace.cpp
    src/componenteditor.h
    src/componenteditor.cpp
    src/contrastpreview.h
    src/contrastpreview.cpp
    src/hsvcolorspace.h
    src/hsvcolorspace.cpp
    src/imagegradientselector.h
    src/imagegradientselector.cpp
    src/rgbcolorspace.h
    src/rgbcolorspace.cpp
    src/window.h
    src/window.cpp
    src/main.cpp
)

#
# Resources
#
include(ECMAddAppIcon)
ecm_add_app_icon(PROJECT_SOURCES ICONS
    resources/icons/1024-apps-${PROJECT_ID}.png
    resources/icons/256-apps-${PROJECT_ID}.png
    resources/icons/32-apps-${PROJECT_ID}.png
    resources/icons/sc-apps-${PROJECT_ID}.svg
)
set(PROJECT_RESOURCES
    resources/app.qrc
    resources/freedesktop/${PROJECT_ID}.desktop
)
set(PROJECT_TRANSLATION_TEMPLATE resources/translations/${PROJECT_ID}.ts)
file(GLOB PROJECT_TRANSLATION_SOURCES "resources/translations/${PROJECT_ID}_*.ts")

source_group("Resource Files" FILES
    ${PROJECT_RESOURCES}
    ${PROJECT_TRANSLATION_TEMPLATE}
    ${PROJECT_TRANSLATION_SOURCES}
)

#
# Translations
#
if(${QT_VERSION_MAJOR} VERSION_LESS 6.2)
    if(PROJECT_TRANSLATIONS_UPDATE)
        # the first call creates/updates the <appname>.ts template file, it should not be compiled,
        # the other the actual <appname>_<country-code>.ts translation files to be installed later.
        qt_create_translation(QM_DUMMY
            ${PROJECT_SOURCES}
            ${PROJECT_TRANSLATION_TEMPLATE}
        )
        qt_create_translation(PROJECT_QM_FILES
            ${PROJECT_SOURCES}
            ${PROJECT_TRANSLATION_SOURCES}
        )
        add_custom_target(TemplateUpdate ALL DEPENDS ${QM_DUMMY})
    endif()
    qt_add_translation(PROJECT_QM_FILES ${PROJECT_TRANSLATION_SOURCES})
else()
    # FIXME: updating the .ts files seems not to work in Qt6, even by using the deprecated
    #        qt_create_translation() used with Qt5. It's not clear where qt_add_lupdate() call
    #        should be placed, qt_add_lrelease() wants to be called after add_executable() for sure.
    if(PROJECT_TRANSLATIONS_UPDATE)
        qt_add_lupdate(${PROJECT_NAME}
            TS_FILES ${PROJECT_TRANSLATION_TEMPLATE} ${PROJECT_TRANSLATION_SOURCES}
            SOURCES ${PROJECT_SOURCES}
        )
    endif()
endif()

#
# Build
#
add_executable(${PROJECT_NAME}
    ${PROJECT_QM_FILES}
    ${PROJECT_RESOURCES}
    ${PROJECT_SOURCES}
    ${PROJECT_TRANSLATION_SOURCES}
    ${PROJECT_TRANSLATION_TEMPLATE}
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
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Woverloaded-virtual)
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DATA_DIR="${PROJECT_DATA_DIR}")

if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL 6.2)
    qt_add_lrelease(${PROJECT_NAME}
        TS_FILES ${PROJECT_TRANSLATION_SOURCES}
        QM_FILES_OUTPUT_VARIABLE PROJECT_QM_FILES
    )
endif()

#
# Install
#
include(ECMInstallIcons)
if (UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(FILES "resources/freedesktop/${PROJECT_ID}.desktop"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
    )
    install(FILES "${PROJECT_QM_FILES}"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_ID}/translations"
    )
    ecm_install_icons(ICONS "sc-apps-${PROJECT_ID}.svg"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/icons"
    )
endif()

#
# Project information
#
message(STATUS "
Qt version:                       ${QT_VERSION}
Build type:                       ${CMAKE_BUILD_TYPE}
Install prefix:                   ${CMAKE_INSTALL_PREFIX}
Update translations before build: ${PROJECT_TRANSLATIONS_UPDATE}
")
