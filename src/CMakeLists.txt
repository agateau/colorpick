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

add_executable(colorpick ${colorpick_SRCS})

target_link_libraries(colorpick Qt5::Core Qt5::Widgets
    KF5::GuiAddons
    KF5::WidgetsAddons)

# Install
install(TARGETS colorpick
    DESTINATION bin
    )
