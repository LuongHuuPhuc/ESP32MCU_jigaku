set (req Bitmap driver)
idf_component_register(SRCS "main.c"
                      INCLUDE_DIRS "."
                      REQUIRES ${req})