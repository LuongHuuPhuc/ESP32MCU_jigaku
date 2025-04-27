set(req driver freertos)
set(IDF_TARGET "esp32h2")
idf_component_register(SRCS "led_button.c"
                      INCLUDE_DIRS "." "include"
                      REQUIRES ${req}) 