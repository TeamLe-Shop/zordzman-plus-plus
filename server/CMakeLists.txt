cmake_minimum_required(VERSION 3.0)

project(zordz-server)

option(GATSAN "You are gatsan" FALSE)

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

# For GCC and Clang, enable C++11 support and add some other flags
if(USING_GCC OR USING_CLANG OR GATSAN)
  add_compile_options(-std=c++11 -pedantic -Wall -Wextra
                      -Wmissing-declarations -Wnon-virtual-dtor)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_net REQUIRED)
include_directories(${SDLNET_INCLUDE_DIR})
include_directories(zordzmanpp-common/extlib/cppformat)
if(NOT TARGET cppformat)
add_subdirectory(zordzmanpp-common)
endif()

if(GATSAN)
	set(SDLNET_LIBRARY /Library/Frameworks/SDL2_net.framework)
	set(SDLNET_INCLUDE_DIR /Library/Frameworks/SDL2_net.framework/Headers)
        add_compile_options(-stdlib=libc++)
endif()

include_directories(src)

file(GLOB_RECURSE SOURCE_FILES src/lib/*.*pp)

add_library(libzordz-server ${SOURCE_FILES})
add_executable(zordz-server src/main.cpp)

target_link_libraries(zordz-server ${SDL2_LIBRARY} ${SDLNET_LIBRARY} libzordz-server cppformat cJSON)
