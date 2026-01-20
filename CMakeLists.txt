cmake_minimum_required(VERSION 3.28)
project(SNAKH)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_NAME "snakh")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main
    src/irrationoid.hpp
    src/geoh.hpp
    src/game.hpp

    src/gridlines.hpp
    src/graphical_utils.hpp
    src/render.hpp

    src/platform.hpp
    src/main.cpp
)
set_target_properties(main PROPERTIES OUTPUT_NAME ${EXECUTABLE_OUTPUT_NAME})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
