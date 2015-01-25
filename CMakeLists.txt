cmake_minimum_required(VERSION 3.0)

project(zordzman)

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

include_directories(common/extlib/json11)
include_directories(common/extlib/cppformat)
include_directories(common/extlib/base64)

add_library(cppformat common/extlib/cppformat/format.cc)

# For GCC and Clang, enable C++11
if(USING_GCC OR USING_CLANG OR OSX)
  add_compile_options(-std=c++11)
  if(SANITIZE)
    add_compile_options(-fsanitize=${SANITIZE})
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${SANITIZE}")
  endif()
endif()

# Build external libraries
add_library(json11 common/extlib/json11/json11.cpp)

# Add a bunch of warnings for our own stuff
if(USING_GCC OR USING_CLANG OR OSX)
    add_compile_options(-pedantic -Wall -Wextra -Wmissing-declarations
                        -Wnon-virtual-dtor)
endif()

add_library(hash-library common/extlib/hash-library/md5.cpp)
add_library(base64
            common/extlib/base64/base64.hpp common/extlib/base64/base64.cpp)

add_library(server server/lib/Server.cpp server/lib/Client.cpp server/lib/Map.cpp server/lib/physicsSystem.cpp)
add_library(logger common/logger/Logger.hpp common/logger/Logger.cpp)
file(GLOB_RECURSE COMMON_NET_SOURCES common/net/*.*pp)
add_library(common_net ${COMMON_NET_SOURCES})
file(GLOB_RECURSE COMMON_ENTITY_SOURCES
     common/entity/*.*pp common/entity/components/*.*pp)
add_library(common_entity ${COMMON_ENTITY_SOURCES})
file(GLOB_RECURSE COMMON_UTIL_SOURCES common/util/*.*pp)
add_library(common_util ${COMMON_UTIL_SOURCES})
add_library(zjson common/zjson/zjson.hpp common/zjson/zjson.cpp)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
include_directories(${SDLIMAGE_INCLUDE_DIR})
find_package(SDL2_mixer REQUIRED)
include_directories(${SDLMIXER_INCLUDE_DIR})

if(OSX)
    set(SDLIMAGE_LIBRARY /Library/Frameworks/SDL2_image.framework)
    set(SDLIMAGE_INCLUDE_DIR /Library/Frameworks/SDL2_image.framework/Headers)
    set(SDLMIXER_LIBRARY /Library/Frameworks/SDL2_mixer.framework)
    set(SDLMIXER_INCLUDE_DIR /Library/Frameworks/SDL2_mixer.framework/Headers)
    add_compile_options(-stdlib=libc++)
    add_compile_options(-std=c++11)
endif()

include_directories(.)
include_directories(client)

file(GLOB_RECURSE CLIENT_SOURCES client/*.*pp)
add_executable(zordzman ${CLIENT_SOURCES})

file(GLOB_RECURSE SERVER_SOURCES server/*.*pp)
add_executable(zordzman-server ${SERVER_SOURCES})

target_link_libraries(common_entity json11)
target_link_libraries(common_util cppformat)
target_link_libraries(server common_entity hash-library common_util logger base64)
target_link_libraries(zjson cppformat json11)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(common_util ws2_32)
    target_link_libraries(zordzman ws2_32)
    target_link_libraries(server ws2_32)
endif()

target_link_libraries(zordzman
    ${SDL2_LIBRARY}
    ${SDLIMAGE_LIBRARY}
    ${OPENGL_LIBRARY}
    ${SDLMIXER_LIBRARY}
    json11
    cppformat
    server
    zjson
    base64
    hash-library
    common_util
    common_net
    common_entity
)

target_link_libraries(zordzman-server
    ${SDL2_LIBRARY}
    server
    json11
    cppformat
    logger
    zjson
    base64
    hash-library
    common_util
    common_net
    common_entity
)
