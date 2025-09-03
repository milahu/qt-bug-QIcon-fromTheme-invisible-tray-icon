cmake_minimum_required(VERSION 3.16)
project(qt_invisible_system_tray_icon_project LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Svg)

message(STATUS "Found Qt6 Core version: ${Qt6Core_VERSION}")
message(STATUS "Found Qt6 Widgets version: ${Qt6Widgets_VERSION}")
message(STATUS "Found Qt6 Svg version: ${Qt6Svg_VERSION}")

add_executable(qt_invisible_system_tray_icon
    src/qt_invisible_system_tray_icon.cpp
    src/resources/resources.qrc
)

target_link_libraries(qt_invisible_system_tray_icon PRIVATE Qt6::Widgets)

add_executable(qt_find_icons
    src/qt_find_icons.cpp
    src/resources/resources.qrc
)

target_link_libraries(qt_find_icons PRIVATE Qt6::Widgets Qt6::Svg)

# --- Install rules ---
include(GNUInstallDirs)

# Install the binary into the standard "bin" dir
install(TARGETS qt_invisible_system_tray_icon qt_find_icons
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    FILES
        src/resources/icons/koi_tray.svg
        src/resources/icons/koi_tray.png
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps
)
