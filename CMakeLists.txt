cmake_minimum_required(VERSION 3.8.2)

project(Evolution)

enable_language(CUDA)

project(Evolution LANGUAGES CXX CUDA)
set(CUDA_SEPARABLE_COMPILATION ON)
find_package(CUDA QUIET REQUIRED)

if(NOT DEFINED CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD 11)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}; -O3 -gencode arch=compute_75,code=sm_75)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -funsafe-math-optimizations")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})
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

include_directories("/home/rian/Code/Projects/CoreNN/Include")
include_directories("/home/rian/Code/Libraries/Arcade-Learning-Environment-0.6.0/src")

include_directories("/home/rian/Code/Libraries/OpenBLAS/include")
include_directories("/opt/cuda/include")
# include_directories("/libraw/")
# include_directories("/usr/include/opencv4")
# include_directories(${OpenCV_INCLUDE_DIRS})
# include_directories("/opt/AMDAPPSDK-3.0/include")
# include_directories("/home/rian/Code/Libraries/clBLAS-2.12.0-Linux-x64/include")


## LINK
link_directories("/home/rian/Code/Libraries/OpenBLAS/lib")
link_directories("/opt/cuda/lib64")
# link_directories("/opt/AMDAPPSDK-3.0/lib/x86_64/sdk")
# link_directories("/home/rian/Code/Libraries/clBLAS-2.12.0-Linux-x64/lib64")
link_directories("/home/rian/Code/Projects/CoreNN/Lib")
link_directories("/home/rian/Code/Libraries/Arcade-Learning-Environment-0.6.0")


## TARGET
file(GLOB SOURCE_FILES
    Include/*.h
    Source/*.cpp
    /home/rian/Code/Projects/CoreNN/Include/*.h
    /home/rian/Code/Projects/CoreNN/Include/Neurons/*.h
    /home/rian/Code/Projects/CoreNN/Include/ErrorFunctions/*.h
    /home/rian/Code/Projects/CoreNN/Include/Optimizers/*.h
    /home/rian/Code/Projects/CoreNN/Include/Initializers/*.h
    /home/rian/Code/Projects/CoreNN/Source/*.cpp
    /home/rian/Code/Projects/CoreNN/Source/*.cu
    /home/rian/Code/Projects/CoreNN/Source/Neurons/*.cpp
    /home/rian/Code/Projects/CoreNN/Source/Neurons/*.cu
    /home/rian/Code/Projects/CoreNN/Source/ErrorFunctions/*.cpp
    /home/rian/Code/Projects/CoreNN/Source/ErrorFunctions/*.cu
    /home/rian/Code/Projects/CoreNN/Source/Optimizers/*.cpp
    /home/rian/Code/Projects/CoreNN/Source/Optimizers/*.cu
    /home/rian/Code/Projects/CoreNN/Source/Initializers/*.cpp
    /home/rian/Code/Projects/CoreNN/Source/Initializers/*.cu
)

add_executable(Evolution ${SOURCE_FILES})
target_link_libraries(Evolution libale.so)
target_link_libraries(Evolution lua5.4)
target_link_libraries(Evolution libopenblas.a)
target_link_libraries(Evolution libcudart.so)
target_link_libraries(Evolution libcudadevrt.so)
target_link_libraries(Evolution libcuda.so)
target_link_libraries(Evolution libcublas.so)
target_link_libraries(Evolution libcurand.so)
target_link_libraries(Evolution libcudnn.so)
target_link_libraries(Evolution libIL.so)
target_link_libraries(Evolution libILU.so)
target_link_libraries(Evolution libILUT.so)
target_link_libraries(Evolution pthread)
# target_link_libraries(Evolution libCoreNN.a)
# target_link_libraries(ArcadeLearner libopenblas.a)
# target_link_libraries(ArcadeLearner libOpenCL.so.1)
# target_link_libraries(ArcadeLearner libclBLAS2.so)
# # target_link_libraries(ArcadeLearner SDL2)
# # target_link_libraries(ArcadeLearner SDL2main)
# target_link_libraries(ArcadeLearner pthread)
# target_compile_options(Evolution PUBLIC "-rdc=true")
set_property(TARGET Evolution PROPERTY CUDA_STANDARD 11)
target_compile_features(Evolution PUBLIC cxx_std_11)
set_target_properties(Evolution PROPERTIES CUDA_SEPARABLE_COMPILATION ON)