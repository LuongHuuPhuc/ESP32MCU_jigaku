set(req esp_timer driver Filter_master_ecg)
idf_component_register(SRCS "ad8232_adc.c"
                       INCLUDE_DIRS "."
                       REQUIRES ${req})