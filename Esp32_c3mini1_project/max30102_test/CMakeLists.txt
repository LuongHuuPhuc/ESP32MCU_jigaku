cmake_minimum_required(VERSION 3.30)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENTS_DIRS ${CMAKE_CURRENT_LIST_DIR}/../../components/SSD1306
                          ${CMAKE_CURRENT_LIST_DIR}/../../Mysource/max30102_test/component)

project(MAX30102_test)