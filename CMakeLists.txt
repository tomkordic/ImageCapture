cmake_minimum_required(VERSION 3.15)
project(ImageCapturer)

find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)

include_directories(${})
link_directories(${FFMPEG_LIBRARY_DIRS})

message(FFMPEG_INCLUDE_DIRS: ${FFMPEG_INCLUDE_DIRS})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_definitions(-D__STDC_CONSTANT_MACROS)

set(SRC 
    src/main.cpp
    src/encoder.cpp
    src/decoder.cpp
    src/avframequeue.cpp
)

# Add the executable
add_executable(ImageCapturer ${SRC})
target_link_libraries(ImageCapturer ${FFMPEG_LIBRARIES})
target_include_directories(ImageCapturer PRIVATE ${FFMPEG_INCLUDE_DIRS})