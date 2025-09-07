cmake_minimum_required(VERSION 3.24.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(IDF_TARGET esp32c3)

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../Test_source/lsm6ds3_test/components
                          ${CMAKE_CURRENT_SOURCE_DIR}/components)

project(mpu6050_test)