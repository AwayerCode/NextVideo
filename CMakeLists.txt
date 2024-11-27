cmake_minimum_required(VERSION 3.16)
project(MyQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 Qt 路径
set(Qt6_DIR "D:/Qt/6.8.0/msvc2022_64")
set(CMAKE_PREFIX_PATH "D:/Qt/6.8.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Quick Core)

# Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 源文件
set(PROJECT_SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    resources.qrc
)

# 添加可执行文件
qt_add_executable(MyQtApp ${PROJECT_SOURCES})

# QML 模块
qt_add_qml_module(MyQtApp
    URI MyQtApp
    VERSION 1.0
    RESOURCE_PREFIX "/"  
    QML_FILES
        qml/MainWindow.qml
        qml/components/CustomButton.qml
)

# 链接库
target_link_libraries(MyQtApp PRIVATE
    Qt6::Core
    Qt6::Quick
)

# Windows 可执行文件设置
set(CMAKE_WIN32_EXECUTABLE OFF)


# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Qt 依赖部署（Windows）
if(WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    
    add_custom_command(TARGET MyQtApp POST_BUILD
        COMMAND "${QT_BIN_DIR}/windeployqt.exe"
                --qmldir "${CMAKE_SOURCE_DIR}/qml"
                "$<TARGET_FILE:MyQtApp>"
    )
endif() 