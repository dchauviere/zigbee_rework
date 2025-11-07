set(ZIGBEE_SDK_VERSION "3.7.2.0")
set(DOWNLOAD_URL_TOOLCHAIN "http://shyboy.oss-cn-shenzhen.aliyuncs.com/readonly/tc32_gcc_v2.0.tar.bz2")
set(TOOLCHAIN_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain")
set(DOWNLOAD_PATH_TOOLCHAIN "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/tc32.tar.bz2")
set(TOOLCHAIN_PREFIX_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/tc32")

# Download and extract Telink Zigbee SDK
if(NOT EXISTS ${TOOLCHAIN_INSTALL_DIR})
  message(STATUS "Get Zigbee SDK to ${TOOLCHAIN_INSTALL_DIR}")
  include(FetchContent)
  FetchContent_Declare(tl_zigbbe_sdk
    GIT_REPOSITORY "https://github.com/telink-semi/telink_zigbee_sdk"
    GIT_TAG "V${ZIGBEE_SDK_VERSION}"   # it's much better to use a specific Git revision or Git tag for reproducibility
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
  )
  FetchContent_MakeAvailable(tl_zigbbe_sdk)
endif()

if(NOT EXISTS ${TOOLCHAIN_INSTALL_DIR}/tc32)
  if(NOT EXISTS ${DOWNLOAD_PATH_TOOLCHAIN})
    message(STATUS "Downloading SDK from ${DOWNLOAD_URL_TOOLCHAIN} to ${DOWNLOAD_PATH_TOOLCHAIN}")
    file(DOWNLOAD ${DOWNLOAD_URL_TOOLCHAIN} ${DOWNLOAD_PATH_TOOLCHAIN} SHOW_PROGRESS)
  endif()

  message(STATUS "Extracting SDK to ${TOOLCHAIN_PREFIX_LOCATION}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${DOWNLOAD_PATH_TOOLCHAIN} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/)
  file(REMOVE ${DOWNLOAD_PATH_TOOLCHAIN})
endif()
