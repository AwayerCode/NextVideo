# 启用 Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 需要在使用 Qt6::Core 之前找到它
find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(deleter
    STATIC
    Deleter.cpp
    Deleter.h
)

target_include_directories(deleter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(deleter
    PRIVATE
        Qt6::Core
)

# 设置静态库输出目录
set_target_properties(deleter PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
) 