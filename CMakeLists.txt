cmake_minimum_required(VERSION 3.1.0)

project(map)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_<LANG>_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

set(OpenCV_DIR /usr/local/lib/cmake/opencv4)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(map
        ./src/main.cpp
        ./src/Map.cpp
        ./src/Astar.cpp)

target_link_libraries(map
    ${OpenCV_LIBS}
    -lopencv_imgcodecs
    )
