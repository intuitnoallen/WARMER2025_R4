# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.12)

# sdkconfig is generated into "build" folder
set(SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig)

# Include main IDF cmake file and define the project.
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(fw_esp32_mini_warmer_R4)
