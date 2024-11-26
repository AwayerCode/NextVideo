cmake_minimum_required(VERSION 3.14)
project(ffmpeg_test_tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Quick QuickControls2 Qml QuickDialogs2 QuickDialogs2QuickImpl)
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

# 添加QML资源文件
set(QML_RESOURCES
    qml.qrc
)

# 明确指定源文件路径
add_executable(ffmpeg_test_tool
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/videocontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ffmpeg_wrapper.cpp
    ${QML_RESOURCES}
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
    ${QT_HEADERS_DIR}/QtQuick
)

target_link_libraries(ffmpeg_test_tool PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::QuickDialogs2
    Qt6::QuickDialogs2QuickImpl
    ${FFMPEG_LIBRARIES}
) 