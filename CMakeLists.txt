cmake_minimum_required(VERSION 3.16)
project(MyQtApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Qt6_DIR: $ENV{Qt6_DIR}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

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

qt_policy(SET QTP0001 NEW)
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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

function(copy_qt_dependencies TARGET_NAME)
    if(WIN32)
        get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
        
        # 添加调试信息
        message(STATUS "正在配置Qt依赖部署...")
        message(STATUS "Qt Bin目录: ${QT_BIN_DIR}")
        message(STATUS "目标文件目录: $<TARGET_FILE_DIR:${TARGET_NAME}>")
        
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "开始部署Qt依赖..."
            # 确保QML目录存在
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/qml"
            # 运行windeployqt
            COMMAND "${QT_BIN_DIR}/windeployqt.exe"
                    --verbose 2
                    --qmldir "${CMAKE_SOURCE_DIR}/qml"
                    --no-compiler-runtime
                    --no-opengl-sw
                    --dir "$<TARGET_FILE_DIR:${TARGET_NAME}>"
                    "$<TARGET_FILE:${TARGET_NAME}>"
            COMMENT "正在部署Qt依赖..."
        )
    endif()
endfunction()

# 添加QML模块复制函数
function(copy_qml_files TARGET_NAME)
    # 复制QML文件到输出目录
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/qml"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/qml"
        COMMENT "正在复制QML文件..."
    )
endfunction()

# 在主目标后调用这些函数
copy_qt_dependencies(${PROJECT_NAME})
copy_qml_files(${PROJECT_NAME})