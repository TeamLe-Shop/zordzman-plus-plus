cmake_minimum_required(VERSION 3.0)

project(zordzman)

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

# For GCC and Clang, enable C++11 support and add some other flags
if(USING_GCC OR USING_CLANG)
  add_compile_options(-std=c++11 -pedantic -Wall -Wextra
                      -Wshadow -Wmissing-declarations -Wnon-virtual-dtor)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(zordzman
src/main.cpp
)

target_link_libraries(zordzman ${SDL2_LIBRARY} ${SDLIMAGE_LIBRARY})
