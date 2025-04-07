set(req driver freertos VL53L0X)
idf_component_register(SRCS "main.c"
                      INCLUDE_DIRS "."
                      REQUIRES ${req})