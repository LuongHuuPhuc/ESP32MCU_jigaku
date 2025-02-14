cmake_minimum_required(VERSION 3.24.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)\
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/Bitmap)
project(ssd1306_oled_test)