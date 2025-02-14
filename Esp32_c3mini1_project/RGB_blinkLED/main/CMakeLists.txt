set(req driver esp_timer)
set(priv_req espressif__led_strip)
idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "." "D:/Esp-idf/Esp32MCU_jigaku/Esp32_c3mini1_project/RGB_blinkLED/managed_components/espressif__led_strip/src"
                      REQUIRES ${req}
                      PRIV_REQUIRES ${priv_req})