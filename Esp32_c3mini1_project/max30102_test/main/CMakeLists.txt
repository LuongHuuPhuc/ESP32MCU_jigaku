set(req driver freertos max30102)
idf_component_register(SRCS "max30102_main.c"
                      INCLUDE_DIRS "."
                      REQUIRES ${req})