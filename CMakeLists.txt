cmake_minimum_required(VERSION 3.16)
project(MyQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到所需的 Qt 组件
find_package(Qt6 REQUIRED COMPONENTS Quick Core Qml QmlWorkerScript)

# 设置自动处理 MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 项目源文件
set(PROJECT_SOURCES
    src/main.cpp
    src/backend.h
    src/backend.cpp
    qml/main.qml
    qml/components/CustomButton.qml
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加可执行文件
qt_add_executable(MyQtApp ${PROJECT_SOURCES})

# 添加 QML 模块
qt_add_qml_module(MyQtApp
    URI MyQtApp
    VERSION 1.0
    QML_FILES
        qml/main.qml
        qml/components/CustomButton.qml
)

# 设置包含目录
target_include_directories(MyQtApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Qt6Quick_INCLUDE_DIRS}
    ${Qt6Core_INCLUDE_DIRS}
)

# 链接 Qt 库
target_link_libraries(MyQtApp PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::QmlWorkerScript
)

# 设置应用程序属性
set_target_properties(MyQtApp PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
) 