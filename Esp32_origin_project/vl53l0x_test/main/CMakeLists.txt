set(req driver freertos SSD1306 VL53L0X)
idf_component_register(SRCS "main.c"
                      INCLUDE_DIRS "." "D:/Esp-idf/Esp32MCU_jigaku/components/SSD1306/src/intf"
                      REQUIRES ${req})