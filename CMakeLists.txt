# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.18)

# c++17
set(CMAKE_CXX_STANDARD 17)
# 项目信息
project("T")

# include路径
include_directories(${PROJECT_SOURCE_DIR}/lib)

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

#生成debug版本
# SET(CMAKE_BUILD_TYPE "release")
# if (CMAKE_BUILD_TYPE STREQUAL debug)
#    add_definitions(-D_DEBUG)
# endif ()
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -march=native -Wall -std=c++11")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
# aux_source_directory(. DIR_SRCS)


# 指定生成目标
add_executable(${PROJECT_NAME}  mutiThread/Foods.cpp)
find_package(OpenMP)
find_package(Threads)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX Threads::Threads)