set(DOWNLOAD_URL_SDK "http://wiki.telink-semi.cn/tools_and_sdk/Zigbee/Zigbee_SDK.zip")
set(DOWNLOAD_URL_TOOLCHAIN "http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2")
include(${CMAKE_SOURCE_DIR}/lib/cmake/TelinkSDK.cmake)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/lib/cmake/Toolchain.cmake")

cmake_minimum_required(VERSION 3.25)

project(dce_zigbee VERSION 0.1.0 LANGUAGES ASM C)

set(CMAKE_VERBOSE_MAKEFILE ON)

ADD_SUBDIRECTORY(lib/cmake_timestamp timestamp)
ADD_SUBDIRECTORY(bootloader)
ADD_SUBDIRECTORY(switch)
