cmake_minimum_required(VERSION 3.0)

project(zordzman)

option(GATSAN "You are gatsan" FALSE)

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

include_directories(extlib/cJSON)
include_directories(extlib/cppformat)

add_library(cJSON extlib/cJSON/cJSON.c)
add_library(cppformat extlib/cppformat/format.cc)

# For GCC and Clang, enable C++11 support and add some other flags
if(USING_GCC OR USING_CLANG OR GATSAN)
  add_compile_options(-std=c++11 -pedantic -Wall -Wextra
                      -Wmissing-declarations -Wnon-virtual-dtor)
  if(SANITIZE)
    add_compile_options(-fsanitize=${SANITIZE})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${SANITIZE}")
  endif()
endif()

include_directories(zordzmanpp-server/lib)
add_subdirectory(zordzmanpp-server)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
include_directories(${SDLIMAGE_INCLUDE_DIR})
find_package(SDL2_net REQUIRED)
include_directories(${SDLNET_INCLUDE_DIR})

if(GATSAN)
	set(SDLIMAGE_LIBRARY /Library/Frameworks/SDL2_image.framework)
	set(SDLIMAGE_INCLUDE_DIR /Library/Frameworks/SDL2_image.framework/Headers)
	set(SDLNET_LIBRARY /Library/Frameworks/SDL2_net.framework)
	set(SDLNET_INCLUDE_DIR /Library/Frameworks/SDL2_net.framework/Headers)
        add_compile_options(-stdlib=libc++)
endif()

include_directories(src)

file(GLOB_RECURSE SOURCE_FILES src/*.*pp)

add_executable(zordzman ${SOURCE_FILES})

target_link_libraries(zordzman
    ${SDL2_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLNET_LIBRARY}
    ${OPENGL_LIBRARY} cJSON cppformat)
