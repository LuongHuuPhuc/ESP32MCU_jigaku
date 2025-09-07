# REQUIRES: Khai bao rang dependency la mot phan "public API" ma ban muon export cho cac component khac thay duoc
# // component cua ban phu thuoc vao component kia va tat ca cac component khac phu thuoc vao ban cung se tu dong thay duoc component kia
# PRIV_REQUIRES: Khai bao rang dependency la rieng tu, chi phuc vu noi bo 
# // component cua ban phu thuoc vao component kia, nhung dependency nay khong duoc lan truyen ra ngoai

set(req driver esp_timer freertos I2C_dev)
set(priv MPU6050_lib)
idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES ${req}
                       PRIV_REQUIRES ${priv})