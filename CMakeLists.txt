cmake_minimum_required(VERSION 3.0.0)
project(SDL2-stb-image-loading-example)

find_package(SDL2 REQUIRED)

add_executable(example example.c stb_image.h)
target_link_libraries(example ${SDL2_LIBRARIES})
file(COPY "${CMAKE_CURRENT_LIST_DIR}/sdl2_logo_trans.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")