cmake_minimum_required (VERSION 3.5)
project (Stakken)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

#find_package(SDL2 REQUIRED)
include_directories("${source_dir}/include/")

file (GLOB source_files "${source_dir}/*.cpp")

add_executable (Stakken ${source_files})
target_link_libraries(Stakken SDL2main SDL2)