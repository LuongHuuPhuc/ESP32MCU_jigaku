# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Esp-idf/Espressif/frameworks/esp-idf-v5.2.3/components/bootloader/subproject"
  "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader"
  "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix"
  "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix/tmp"
  "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix/src"
  "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Esp-idf/Esp32MCU_jigaku/i2c_scan/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
