# 设置 Qt 路径
set(Qt6_DIR "D:/Qt/6.8.0/msvc2022_64")
set(CMAKE_PREFIX_PATH "D:/Qt/6.8.0/msvc2022_64")

# 查找 Qt Core 组件
find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(deleter
    STATIC
    deleter.cpp
    deleter.h
)

target_include_directories(deleter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(deleter
    PRIVATE
        Qt6::Core
) 