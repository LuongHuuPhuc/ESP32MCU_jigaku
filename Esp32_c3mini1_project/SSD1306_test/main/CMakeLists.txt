set(req driver freertos SSD1306)
idf_component_register(SRCS "ssd1306_main.c" 
                      INCLUDE_DIRS "." "D:/Esp-idf/Esp32MCU_jigaku/components/SSD1306/src/intf"
                      REQUIRES ${req})