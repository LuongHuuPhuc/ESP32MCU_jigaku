cmake_minimum_required(VERSION 3.24)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../components/VL53L0X
                         ${CMAKE_CURRENT_SOURCE_DIR}/../../components/SSD1306) #Lui ve 2 cap
project(vl53l0x_test)