cmake_minimum_required(VERSION 3.24.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENTS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../Mysource/lsm6ds3_test/components/I2C_dev
                          ${CMAKE_CURRENT_SOURCE_DIR}/components)

project(mpu6050_test)