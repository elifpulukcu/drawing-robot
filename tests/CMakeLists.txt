cmake_minimum_required(VERSION 3.10)
project(DrawingRobot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "/opt/homebrew/lib")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(TGUI REQUIRED)

add_library(drawing-robot-lib
    src/parser.cpp
)
target_include_directories(drawing-robot-lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(drawing-robot-lib
    PRIVATE
    sfml-graphics sfml-window sfml-system
)

add_executable(drawing-robot
    src/main.cpp
)
target_link_libraries(drawing-robot
    PRIVATE
    drawing-robot-lib
    sfml-graphics sfml-window sfml-system
    TGUI
)

enable_testing()
