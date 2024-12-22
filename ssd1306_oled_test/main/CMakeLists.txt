set(EXTRA_COMPONENTS_DIRS "D:/Esp-idf/Esp32MCU_jigaku/ssd1306_oled_test/components/ssd1306")

idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES ssd1306 driver) 
                       