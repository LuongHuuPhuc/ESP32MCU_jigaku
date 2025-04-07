cmake_minimum_required(VERSION 3.24.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/Bitmap
                         ${CMAKE_CURRENT_SOURCE_DIR}/../../components/SSD1306) #Lui ve 2 cap
include_directories("D:/Esp-idf/Mysource/max30102_test/component/progmem")
project(ssd1306_oled_test)