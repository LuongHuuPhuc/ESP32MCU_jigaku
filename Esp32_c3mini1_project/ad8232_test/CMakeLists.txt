cmake_minimum_required(VERSION 3.30)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(file_path "D:/Esp-idf/Mysource")

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/../../../Mysource/inmp441_test/Filter
                         ${CMAKE_CURRENT_LIST_DIR}/../../../Mysource/ad8232_ecg_test/Lib)
                         
project(ad8232_test)
