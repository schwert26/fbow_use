cmake_minimum_required( VERSION 2.8 )
project( loop_closure )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -O3 -march=native")

# Check C++17 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)

if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    add_definitions(-DCOMPILEDWITHC11)
    message(STATUS "Using flag -std=c++17.")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()


# opencv
find_package( OpenCV 4.2 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# fbow
# fbow is a simple lib so I assume you installed it in default directory
set( FBoW_INCLUDE_DIRS "/home/lijiahao/vocabulary/fbow/src" )
set( FBoW_LIBS "/home/lijiahao/vocabulary/fbow/build/src/libfbow.so" )

# 设置 pybind11 的 CMake 配置目录
set(pybind11_DIR "/home/lijiahao/anaconda3/envs/py38xfeat/lib/python3.8/site-packages/pybind11/share/cmake/pybind11")


# 添加 zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

# 查找 pybind11
find_package(pybind11 REQUIRED)

# 查找 Python3，包含 Interpreter 和 Development 组件
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(${PROJECT_SOURCE_DIR}/cnpy) 
add_library(cnpy STATIC ${PROJECT_SOURCE_DIR}/cnpy/cnpy.cpp)

# 添加 DBoW3 的头文件路径
include_directories( 
${FBoW_INCLUDE_DIRS}
${pybind11_INCLUDE_DIR} 
${Python3_INCLUDE_DIRS}  

 )

add_executable( ceshi ceshi2.cpp )
target_link_libraries( ceshi ${OpenCV_LIBS} ${FBoW_LIBS} cnpy  ${ZLIB_LIBRARIES})


