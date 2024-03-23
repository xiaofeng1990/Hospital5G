cmake_minimum_required(VERSION 3.10)

project(hospital5g CXX C)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH "output directory")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE PATH "output directory")
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(UNIX)
    include(CheckFunctionExists)
    include(CheckIncludeFile)
    include(CheckSymbolExists)
    CHECK_INCLUDE_FILE(pthread.h HAVE_PTHREAD)
    CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC)
    set(LINKS_LIBS ${LINKS_LIBS} m pthread stdc++fs)
endif(UNIX)

set(DEBUG ON)
if (DEBUG)
    set(XT_COMPILE_FLAGS "-g -O0")
else()
    set(XT_COMPILE_FLAGS "-g -O2")
endif()

set(XT_COMPILE_FLAGS "${XT_COMPILE_FLAGS} -ffunction-sections -fdata-sections -funwind-tables")
set(XT_COMPILE_FLAGS "${XT_COMPILE_FLAGS} -fstack-protector-strong")

set(CMAKE_C_FLAGS    "${XT_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS  "${XT_COMPILE_FLAGS}" )


add_subdirectory(master)