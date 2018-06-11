cmake_minimum_required(VERSION 2.6)

project(Evolution)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -funsafe-math-optimizations")

SET(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
# SET(CMAKE_BUILD_TYPE RELEASE)

# SET(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source)


## INCLUDE
include_directories("Include")

# include_directories("/home/rian/Code/Libraries/OpenBLAS/include")
# include_directories("/opt/AMDAPPSDK-3.0/include")
# include_directories("/home/rian/Code/Libraries/clBLAS-2.12.0-Linux-x64/include")
# 
# include_directories("/home/rian/Code/Projects/CoreNN/Include")
include_directories("/home/rian/Code/Libraries/Arcade-Learning-Environment-0.6.0/src")


## LINK
# link_directories("/home/rian/Code/Libraries/OpenBLAS/lib")
# link_directories("/opt/AMDAPPSDK-3.0/lib/x86_64/sdk")
# link_directories("/home/rian/Code/Libraries/clBLAS-2.12.0-Linux-x64/lib64")
# link_directories("/home/rian/Code/Projects/CoreNN/Lib")
link_directories("/home/rian/Code/Libraries/Arcade-Learning-Environment-0.6.0")


## TARGET
file(GLOB SOURCE_FILES
    Include/*.h
    Source/*.cpp
)

add_executable(Evolution ${SOURCE_FILES})
# target_link_libraries(ArcadeLearner libCoreNN.a)
# target_link_libraries(ArcadeLearner libopenblas.a)
# target_link_libraries(ArcadeLearner libOpenCL.so.1)
# target_link_libraries(ArcadeLearner libclBLAS2.so)
target_link_libraries(Evolution libale.so)
# # target_link_libraries(ArcadeLearner SDL2)
# # target_link_libraries(ArcadeLearner SDL2main)
# target_link_libraries(ArcadeLearner pthread)
