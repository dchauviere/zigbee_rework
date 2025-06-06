set(TOOLCHAIN_INSTALL_DIR)
set(SDK_EXPECTED_SHA256 "0d0859e2412e3a52e9c3ba012bbf5d5b7c30554b8a96632659bd500ea18f7aa3")
set(DOWNLOAD_PATH_SDK "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/Zigbee_SDK.zip")
set(SDK_PREFIX_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/tl_zigbee_sdk")

set(DOWNLOAD_PATH_TOOLCHAIN "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/tc32.tar.bz2")
set(TOOLCHAIN_PREFIX_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/tc32")

# Download and extract Telink Zigbee SDK
if(NOT EXISTS ${SDK_PREFIX_LOCATION})
  if(NOT EXISTS ${DOWNLOAD_PATH_SDK})
    message(STATUS "Downloading SDK from ${DOWNLOAD_URL_SDK} to ${DOWNLOAD_PATH_SDK}")
    file(DOWNLOAD ${DOWNLOAD_URL_SDK} ${DOWNLOAD_PATH_SDK} SHOW_PROGRESS
      EXPECTED_HASH SHA256=${SDK_EXPECTED_SHA256})
  endif()

  message(STATUS "Extracting SDK to ${SDK_PREFIX_LOCATION}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH_SDK} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/)
  file(REMOVE ${DOWNLOAD_PATH_SDK})
endif()

# Download and extract Telink toolchain
if(NOT EXISTS ${TOOLCHAIN_PREFIX_LOCATION})
  if(NOT EXISTS ${DOWNLOAD_PATH_TOOLCHAIN})
    message(STATUS "Downloading SDK from ${DOWNLOAD_URL_TOOLCHAIN} to ${DOWNLOAD_PATH_TOOLCHAIN}")
    file(DOWNLOAD ${DOWNLOAD_URL_TOOLCHAIN} ${DOWNLOAD_PATH_TOOLCHAIN} SHOW_PROGRESS)
  endif()

  message(STATUS "Extracting SDK to ${TOOLCHAIN_PREFIX_LOCATION}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${DOWNLOAD_PATH_TOOLCHAIN} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.toolchain/)
  file(REMOVE ${DOWNLOAD_PATH_TOOLCHAIN})
endif()
