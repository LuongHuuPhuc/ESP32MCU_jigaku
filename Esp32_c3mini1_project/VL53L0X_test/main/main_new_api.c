// #include"vl53l0x.h"
// #include<stdio.h>
// #include"freertos/FreeRTOS.h"
// #include"freertos/task.h"
// #include"esp_err.h"
// #include"driver/i2c_master.h"
// #include"driver/i2c_types.h"
// #include"esp_log.h"

// #define SDA_PIN 8
// #define SCL_PIN 9
// #define I2C_PORT I2C_NUM_0
// #define VL53L0X_ADDR 0x52 //Dia chi mac dinh

// static const char *TAG = "VL53L0X";
// TaskHandle_t vl53l0x_handle = NULL;
// vl53l0x_t *sensor = NULL;

// void i2c_master_init(){

//   i2c_master_bus_config_t i2c_config = {
//     .clk_source = I2C_CLK_SRC_DEFAULT,
//     .i2c_port = I2C_PORT,
//     .scl_io_num = SCL_PIN,
//     .sda_io_num = SDA_PIN,
//     .glitch_ignore_cnt = 7,
//     .flags.enable_internal_pullup = true,
//   };

//   i2c_master_bus_handle_t i2c_bus_handle;
//   ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &i2c_bus_handle));

//   i2c_device_config_t i2c_dev_conf = {
//     .dev_addr_length = I2C_ADDR_BIT_LEN_7,
//     .device_address = VL53L0X_ADDR,
//     .scl_speed_hz = 100000,
//   };

//   i2c_master_dev_handle_t dev_handle;
//   ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_bus_handle, &i2c_dev_conf, &dev_handle));
// }

// /**
//  * @brief Task thuc hien do khoang cach
//  * @param pvParameter tham so task (khong su dung)
//  */
// void vl53l0x_task(void *pvParameter){
//   ESP_LOGI(TAG,"Dang khoi tao cam bien...");
//   sensor = vl53l0x_config(I2C_PORT, SCL_PIN, SDA_PIN, -1, VL53L0X_ADDR, 0);

//   if(!sensor){
//     ESP_LOGE(TAG, "Khong tim thay cam bien vl53l0x...");
//     vTaskDelete(NULL); 
//     return;
//   }

//   const char *err = vl53l0x_init(sensor);
//   if(err){
//     ESP_LOGE(TAG, "Loi khoi tao: %s", err);
//     vTaskDelete(NULL); //Dung task neu co loi 
//     return;
//   }

//   ESP_LOGI(TAG, "Khoi tao thanh cong cam bien vl53l0x !");

//   while(1){
//     uint16_t distance = vl53l0x_readRangeSingleMillimeters(sensor);
//     printf("\n%umm", distance);
//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// }

// /**
//  * @brief Ham chinh
//  */

// void app_main(void){
//   ESP_LOGI(TAG, "Khoi tao I2C...");
//   i2c_master_init();
//   xTaskCreatePinnedToCore(&vl53l0x_task, "Read_vl53l0x_task", 4096, NULL, 5, &vl53l0x_handle, 0);
// }
