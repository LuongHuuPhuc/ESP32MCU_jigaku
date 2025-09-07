#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "I2C_dev.h"
#include "mpu6050_ll.h"
#include "mpu6050.h"
#include "mpu6050_log.h"

#define TAG      "MPU6050"
#define SDA_PIN  GPIO_NUM_8
#define SCL_PIN  GPIO_NUM_9

TaskHandle_t mpu6050_TaskHandle = NULL;
mpu6050_config_t mpu6050_cfg;
I2C_dev_init_t dev; 

esp_err_t mpu6050_init_sensor(void){
  esp_err_t ret = ESP_OK; 
  memset(&mpu6050_cfg, 0, sizeof(mpu6050_cfg)); //Set het ve false
  memset(&dev, 0, sizeof(I2C_dev_init_t));

  ret = mpu6050_I2C_dev_config(&dev, I2C_NUM, SDA_PIN, SCL_PIN);
  if(ret != ESP_OK){
    ESP_LOGE(TAG, "MPU6050 I2C_dev_config failed !\r\n");
    return ret;
  }

  ESP_LOGI(TAG, "I2C installing...\r\n");
  ret = i2c_dev_install_device(&dev);
  if(ret != ESP_OK){
    ESP_LOGE(TAG, "i2c_dev_install_device failed !\r\n");
    return ret;
  }

  ESP_LOGI(TAG, "I2C installed !\r\n");
  ret = mpu6050_verify_whoami(&dev);
  if(ret != ESP_OK){
    ESP_LOGE(TAG, "Verify WHO_AM_I_REG failed !\r\n");
    return ret;
  }
  ESP_LOGI(TAG, "Verify WHO_AM_I_REG OK !\r\n");
  return ESP_OK;
}

esp_err_t mpu6050_set_config(mpu6050_config_t *config){
  mpu6050_config_t cfg = {
    .clock_source = MPU6050_CLK_PPL_ZGYRO, //Cau hinh clock source
    .reset = MPU6050_RESET_ALL, //Cau hinh soft reset
    .custom_mask = 0, //Custom mask trong soft reset neu chon MPU6050_RESET_CUSTOM
    .sleep_mode = MPU6050_SLEEP_MODE_DISABLE, //Khong Sleep Mode
    .cycle_mode = MPU6050_CYCLE_MODE_DISABLE, //Khong Low Poer Cycle Mode
    .temperature = false, //Khong dung cam bien nhiet do
    .dlfp_cfg = MPU6050_DLPF_44HZ, //Digital Low Pass Filter bandwidth cang thap -> tre cang cao -> nhieu thap hon
    .freq_hz = 200.0f, //Tan so lay mau 200Hz (Div = 4)
    .accel_range = MPU6050_ACCEL_FS_4G, //Dai do full-scale Accel
    .gyro_range = MPU6050_GYRO_FS_500DPS, //Dai do full-scale gyro
    .standby_mode.disable_standby = true, //Disable standby mode
    .int_cfg.disable = true, //Khong cau hinh tin hieu ngat 
    .interrupts.disable_int = true, //Disable ngat
    .fifo_enable = true, //Bat FIFO
    .accel_en = true, //FIFO nhan Accel data 
    .gyro_en = true, //FIFO nhan Gyro data
    .temp_en = false
  };
  memcpy(config, &cfg, sizeof(mpu6050_config_t));
  esp_err_t ret = mpu6050_sensor_config(&dev, config);
  return ret;
}

void mpu6050_readData_task(void *pvParameter){
  size_t samples = 5;
  mpu6050_scaled_data_t scaled_data[samples] ;
  while(1){
    mpu6050_read_scaled_multi_samples(&dev, scaled_data, MPU6050_READ_FIFO, mpu6050_cfg.sens, samples);
    for(size_t i = 0; i < samples; i++){
      printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", scaled_data[i].accel_x_g, scaled_data[i].accel_y_g, scaled_data[i].accel_z_g,
                                              scaled_data[i].gyro_x_dps, scaled_data[i].gyro_y_dps, scaled_data[i].gyro_z_dps);
    }
    vTaskDelay(pdMS_TO_TICKS(15)); //Tranh chiem CPU
  }
}

void app_main(void){
  ESP_ERROR_CHECK(i2c_dev_initialize());
  ESP_ERROR_CHECK(mpu6050_init_sensor());
  esp_err_t ret = mpu6050_set_config(&mpu6050_cfg);
  if(ret != ESP_OK){
    ESP_LOGE(TAG, "Config set failed !\r\n");
    for(;;){
      vTaskDelay(pdMS_TO_TICKS(5));
    };
  }
  xTaskCreate(mpu6050_readData_task, "MPU6050 TASK", 2048 * 2, NULL, 5, &mpu6050_TaskHandle);
}