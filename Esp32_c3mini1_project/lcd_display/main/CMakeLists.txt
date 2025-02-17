set(req esp_lcd)
idf_component_register(SRCS "lcd_display.c"
                      INCLUDE_DIRS "."
                      REQUIRES ${req})