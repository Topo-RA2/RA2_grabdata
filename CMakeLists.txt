cmake_minimum_required(VERSION 3.5)

project(RA2_grabdata VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets WebEngineWidgets WebEngine REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets WebEngineWidgets WebEngine REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        ra2mem.ui
        setting.ui
        constant.h
        echart.h echart.cpp
        player.h player.cpp
        ra2mem.h ra2mem.cpp
        config.h config.cpp
        setting.h setting.cpp
        resource.qrc
        ra2mem.rc
)

add_executable(RA2_grabdata WIN32
    ${PROJECT_SOURCES}
)

target_link_libraries(RA2_grabdata PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
)
