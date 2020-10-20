include(ECMAddAppIcon)

add_subdirectory(icons)

# Sources
set(colorpick_SRCS
    colorspace.cpp
    coloreditor.cpp
    colorpicker.cpp
    contrastpreview.cpp
    hsvcolorspace.cpp
    imagegradientselector.cpp
    main.cpp
    rgbcolorspace.cpp
    componenteditor.cpp
    window.cpp
    )

# Building
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

ecm_add_app_icon(
    colorpick_SRCS
    ICONS
        icons/1024-apps-${APP_NAME}.png
        icons/256-apps-${APP_NAME}.png
        icons/32-apps-${APP_NAME}.png
        icons/sc-apps-${APP_NAME}.svg
)

qt5_add_resources(colorpick_SRCS app.qrc)

add_executable(colorpick ${colorpick_SRCS})

target_link_libraries(colorpick Qt5::Core Qt5::Widgets
    KF5::GuiAddons
    KF5::WidgetsAddons)

# Install
install(TARGETS colorpick
    DESTINATION bin
    )
