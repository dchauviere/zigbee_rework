include(${CMAKE_SOURCE_DIR}/lib/cmake/TelinkSDK.cmake)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/lib/cmake/Toolchain.cmake")

cmake_minimum_required(VERSION 3.25)

project(dce_zigbee VERSION 0.1.0 LANGUAGES ASM C)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(NOT DEFINED BOARD)
    set(BOARD "TS0012")
endif()

ADD_SUBDIRECTORY(lib/cmake_timestamp timestamp)
ADD_SUBDIRECTORY(bootloader)
ADD_SUBDIRECTORY(switch)
