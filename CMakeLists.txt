cmake_minimum_required(VERSION 3.0)

project(zordzman)

set(PYTHON_COMMAND python3)

# The Python version needed. This is the version of Python that will be used
# to run additional build steps and the version that is embedded into the
# zordzman and zordzman-server binaries. Any patch number will be allowed.
set(PYTHON_VERSION_MAJOR 3)
set(PYTHON_VERSION_MINOR 4)


# Check Python executable version
execute_process(
    COMMAND "${PYTHON_COMMAND}" --version
    OUTPUT_VARIABLE PYTHON_VERSION_FULL
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(
    REGEX MATCH
    "[0-9]+\\.[0-9]+\\.[0-9]+"
    PYTHON_VERSION
    ${PYTHON_VERSION_FULL}
)
math(EXPR PYTHON_VERSION_MINOR_MAX "${PYTHON_VERSION_MINOR} + 2")
set(PYTHON_VERSION_MIN "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
set(PYTHON_VERSION_MAX "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR_MAX}")
if(PYTHON_VERSION STREQUAL "")
    message(FATAL_ERROR "Couldn't determine Python version")
endif()
if((${PYTHON_VERSION} VERSION_LESS ${PYTHON_VERSION_MIN})
    OR (${PYTHON_VERSION} VERSION_EQUAL ${PYTHON_VERSION_MAX})
    OR (${PYTHON_VERSION} VERSION_GREATER ${PYTHON_VERSION_MAX}))
    message(FATAL_ERROR "Need Python ${PYTHON_VERSION_MAJOR}."
            "${PYTHON_VERSION_MINOR}.x but found ${PYTHON_VERSION}")
endif()
message(STATUS "Python version: ${PYTHON_VERSION}")


# Check that the 'zm' package is installed
execute_process(
    COMMAND ${PYTHON_COMMAND} -c "import zm"
    RESULT_VARIABLE ZM_STATUS
)
if(${ZM_STATUS})
    message(FATAL_ERROR "No 'zm' Python package found; try pip install -e .")
endif()
message(STATUS "Found 'zm' Python package")


# Generate files from Python definitions
file(GLOB_RECURSE PYTHON_SOURCES python/*.py)
set(TEMPLATE_MESSAGES ${CMAKE_SOURCE_DIR}/client/net/messages.hpp.in)
set(GEN_EGRESS_MESSAGES ${CMAKE_SOURCE_DIR}/client/net/egress.hpp)
set(GEN_INGRESS_MESSAGES ${CMAKE_SOURCE_DIR}/client/net/ingress.hpp)

set(TEMPLATE_CLIENT ${CMAKE_SOURCE_DIR}/client/net/Client.hpp.in)
set(GEN_CLIENT ${CMAKE_SOURCE_DIR}/client/net/Client.hpp)
add_custom_command(
    OUTPUT ${GEN_CLIENT}
    COMMAND ${PYTHON_COMMAND} -m zm.cgen Egress Ingress ${TEMPLATE_CLIENT} >
            ${GEN_CLIENT}
    DEPENDS ${PYTHON_SOURCES} ${TEMPLATE_CLIENT}
    COMMENT "Generating ${GEN_CLIENT}"
)
add_custom_command(
    OUTPUT ${GEN_EGRESS_MESSAGES}
    COMMAND ${PYTHON_COMMAND} -m zm.cgen Egress ${TEMPLATE_MESSAGES} >
            ${GEN_EGRESS_MESSAGES}
    DEPENDS ${PYTHON_SOURCES} ${TEMPLATE_MESSAGES}
    COMMENT "Generating ${GEN_EGRESS_MESSAGES}"
)
add_custom_command(
    OUTPUT ${GEN_INGRESS_MESSAGES}
    COMMAND ${PYTHON_COMMAND} -m zm.cgen Ingress ${TEMPLATE_MESSAGES} >
            ${GEN_INGRESS_MESSAGES}
    DEPENDS ${PYTHON_SOURCES} ${TEMPLATE_MESSAGES}
    COMMENT "Generating ${GEN_INGRESS_MESSAGES}"
)


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

add_library(server server/lib/Server.cpp server/lib/Client.cpp
                   server/lib/Level.cpp
                   server/lib/systems/physicsSystem.cpp)
add_library(logger common/logger/Logger.hpp common/logger/Logger.cpp)
file(GLOB_RECURSE COMMON_NET_SOURCES common/net/*.*pp)
add_library(common_net ${COMMON_NET_SOURCES})
file(GLOB_RECURSE COMMON_ENTITY_SOURCES
     common/entity/*.*pp common/entity/components/*.*pp)
add_library(common_entity ${COMMON_ENTITY_SOURCES})
file(GLOB_RECURSE COMMON_UTIL_SOURCES common/util/*.*pp)
add_library(common_util ${COMMON_UTIL_SOURCES})
add_library(zjson common/zjson/zjson.hpp common/zjson/zjson.cpp)
add_library(star common/star/tarlib.hpp common/star/tarlib.cpp)

file(GLOB_RECURSE COMMON_RESOURCES_SOURCES common/resources/*.*pp)
add_library(resources ${COMMON_RESOURCES_SOURCES})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# OpenGL & SDL Libraries

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
include_directories(${SDLIMAGE_INCLUDE_DIR})
find_package(SDL2_mixer REQUIRED)
include_directories(${SDLMIXER_INCLUDE_DIR})
find_package(SDL2_ttf REQUIRED)
include_directories(${SDLTTF_INCLUDE_DIR})

if(OSX)
    message(STATUS "OSX building enabled.")
    set(SDLIMAGE_LIBRARY /Library/Frameworks/SDL2_image.framework)
    set(SDLIMAGE_INCLUDE_DIR /Library/Frameworks/SDL2_image.framework/Headers)
    set(SDLMIXER_LIBRARY /Library/Frameworks/SDL2_mixer.framework)
    set(SDLMIXER_INCLUDE_DIR /Library/Frameworks/SDL2_mixer.framework/Headers)
    set(SDLTTF /Library/Frameworks/SDL2_ttf.framework)
    set(SDLTTF_INCLUDE_DIR /Library/Frameworks/SDL2_ttf.framework/Headers)
endif()

# Python

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIR})

include_directories(.)
include_directories(client)

file(GLOB_RECURSE CLIENT_SOURCES client/*.*pp)
message(STATUS ${GEN_CLIENT})
add_executable(
    zordzman
    ${CLIENT_SOURCES}
    ${GEN_CLIENT}
    ${GEN_EGRESS_MESSAGES}
    ${GEN_INGRESS_MESSAGES}
)

file(GLOB_RECURSE SERVER_SOURCES server/*.*pp)
add_executable(zordzman-server ${SERVER_SOURCES})

# Install binaries
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS zordzman DESTINATION bin)

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
    ${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDLIMAGE_LIBRARY}
    ${SDLMIXER_LIBRARY}
    ${SDLTTF_LIBRARY}
    ${PYTHON_LIBRARIES}
    ${CURSES_LIBRARIES}
    ${LIBLZMA_LIBRARIES}
    json11
    cppformat
    server
    zjson
    base64
    hash-library
    common_util
    common_net
    common_entity
    star
    resources
)

target_link_libraries(zordzman-server
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
    star
)

# Build resource packages
file(GLOB_RECURSE SPRITESHEET_METAS
    resources/graphics/tiles/*_meta.json
    resources/graphics/player/*_meta.json
    resources/ui/*_meta.json
)

file(GLOB_RECURSE RESOURCE_METAS
    resources/music/music_meta.json
    resources/fonts/fonts_meta.json
    resources/sound/sound_meta.json
)


set(RESOURCES_DIRECTORY ${CMAKE_BINARY_DIR}/resources)
message(STATUS "Making resources build directory ${RESOURCES_DIRECTORY}")
file(MAKE_DIRECTORY ${RESOURCES_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})

add_custom_command(
    TARGET zordzman POST_BUILD
    COMMAND
        ${PYTHON_COMMAND}
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/make-spritesheet.py
        ${SPRITESHEET_METAS}
    COMMAND
        ${PYTHON_COMMAND}
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/make-resource-package.py
        ${RESOURCE_METAS}
        sprites.manifest
    DEPENDS ${SPRITESHEET_METAS} ${RESOURCE_METAS}
    WORKING_DIRECTORY ${RESOURCES_DIRECTORY}
    COMMENT "Building resources tarball"
)
