cmake_minimum_required (VERSION 3.5)
project (Stakken)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -ldl")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")
set (targetfile "${PROJECT_SOURCE_DIR}/build/Stakken")

#find_package(SDL2 REQUIRED)
include_directories("${PROJECT_SOURCE_DIR}/include")

file (GLOB_RECURSE source_files "${source_dir}/*.cpp")

add_executable (Stakken "${source_dir}/Systems/gl3w.c" ${source_files})
target_link_libraries(Stakken SDL2main SDL2 SDL2_mixer)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)