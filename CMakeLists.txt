cmake_minimum_required(VERSION 3.14)
project(ffmpeg_test_tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(PkgConfig REQUIRED)

execute_process(
    COMMAND qmake6 -query QT_INSTALL_HEADERS
    OUTPUT_VARIABLE QT_HEADERS_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

pkg_check_modules(FFMPEG REQUIRED
    libavcodec
    libavformat
    libavutil
    libswscale
    libavdevice
    libavfilter
)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(ffmpeg_test_tool
    src/main.cpp
    src/mainwindow.cpp
    src/ffmpeg_wrapper.cpp
    include/mainwindow.h
    include/ffmpeg_wrapper.h
)

target_include_directories(ffmpeg_test_tool PRIVATE
    ${INCLUDE_DIR}
    ${FFMPEG_INCLUDE_DIRS}
    ${QT_HEADERS_DIR}
    ${QT_HEADERS_DIR}/QtWidgets
    ${QT_HEADERS_DIR}/QtCore
    ${QT_HEADERS_DIR}/QtGui
    ${QT_HEADERS_DIR}/QtMultimedia
    ${QT_HEADERS_DIR}/QtMultimediaWidgets
)

target_link_libraries(ffmpeg_test_tool PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${FFMPEG_LIBRARIES}
) 