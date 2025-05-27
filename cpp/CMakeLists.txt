cmake_minimum_required(VERSION 3.10)
project(preprocess)

set(CMAKE_CXX_STANDARD 17)

# === OpenCV Paths ===
set(OpenCV_INCLUDE_DIRS "C:/OpenCV/opencv/build/include")
set(OpenCV_LIB_DIR "C:/OpenCV/opencv/build/x64/vc15/lib")

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

# === Use monolithic OpenCV library ===
set(OpenCV_LIBS opencv_world453.lib)

# === Shared library to be used from Python ===
add_library(preprocess SHARED preprocess.cpp)
target_link_libraries(preprocess ${OpenCV_LIBS})

# === Standalone test executable ===
add_executable(preprocess_test main.cpp)
target_link_libraries(preprocess_test preprocess ${OpenCV_LIBS})

# === Output directory settings ===
set_target_properties(preprocess PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(preprocess_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)