set(priv_req VL53L0X)
idf_component_register(SRCS "main_legacy_api.c"
                      INCLUDE_DIRS "."
                      PRIV_REQUIRES ${priv_req})