# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

#编译目标环境、编译器及其路径设置
#SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER   /usr/local/Cellar/gcc@4.8/4.8.5/bin/gcc-4.8)
SET(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc@4.8/4.8.5/bin/g++-4.8)
set(CMAKE_FIND_ROOT_PATH /usr/local/Cellar/gcc@4.8/4.8.5)
# 项目信息
project(cdn)

# include路径
include_directories(${PROJECT_SOURCE_DIR}/lib)

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

# 生成debug版本
SET(CMAKE_BUILD_TYPE "release")
if (CMAKE_BUILD_TYPE STREQUAL debug)
    add_definitions(-D_DEBUG)
endif ()
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(cdn ${DIR_SRCS} MCMF.cpp MCMF.h global.h global.cpp io.cpp cdn.cpp deploy.cpp deploy.h )
