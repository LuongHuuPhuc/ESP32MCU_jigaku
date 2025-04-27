#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/i2c.h"
#include "driver/i2c_types.h"
#include "driver/gpio.h"
#include "max30102.h"

/*********  ESP32-C3  **********/
//Cau hinh chan cho MAX30102
#define I2C_SDA_GPIO  GPIO_NUM_5 
#define I2C_SCL_GPIO  GPIO_NUM_4
#define I2C_PORT      I2C_NUM_0
#define powerLed      UINT8_C(0x1F) //Cuong do led, tieu thu 6.4mA
#define sampleAverage 4
#define ledMode       2
#define sampleRate    500 //Tan so lay mau cao thi kich thuoc BUFFER_SIZE cung phai thay doi de co thoi gian thuat toan xu ly cac mau
#define pulseWidth    411 //Xung cang rong, dai thu duoc cang nhieu (18 bit)
#define adcRange      16384 //14 bit ADC tieu thu 65.2pA moi LSB

/**** MAX30102 ****/
i2c_dev_t dev;  
struct max30102_record record;
unsigned long red, ir;
TaskHandle_t readMAXTask_handle = NULL;

static const char *TAG = "MAX30102";

esp_err_t max30102_configure(void){
    memset(&dev, 0, sizeof(i2c_dev_t));
    ESP_ERROR_CHECK(max30102_initDesc(&dev, 0, I2C_SDA_GPIO, I2C_SCL_GPIO));
    if(max30102_readPartID(&dev) == ESP_OK) {
      ESP_LOGI(TAG, "Found MAX30102!");
    }
    else {
      ESP_LOGE(TAG, "Not found MAX30102");
    }
    ESP_ERROR_CHECK(max30102_init(powerLed, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange, &record, &dev));
    max30102_clearFIFO(&dev);
    return ESP_OK;
}

void readMAX30102_task(void *pvParameter){
    ESP_LOGI(TAG, "Bat dau doc cam bien AD8232");
  
    while(1){
        vTaskDelay(1);
        max30102_check(&record, &dev); //Check the sensor, read up to 3 samples
        while (max30102_available(&record)){
            red = max30102_getFIFORed(&record);
            ir = max30102_getFIFOIR(&record);
            printf("%ld,%ld\n", red, ir);
            max30102_nextSample(&record);
        }
    }
}

void app_main(void){
    ESP_ERROR_CHECK(i2cdev_init()); 
    if(max30102_configure() != ESP_OK){
        ESP_LOGE(pcTaskGetName(NULL), "Khoi tao cam bien khong thanh cong, loi I2C...");
    }else ESP_LOGI(pcTaskGetName(NULL), "Khoi tao cam bien thanh cong !");
    
    xTaskCreatePinnedToCore(readMAX30102_task, "max30102_test", 1024 * 5, &readMAXTask_handle, configMAX_PRIORITIES - 1, NULL, 0);
}