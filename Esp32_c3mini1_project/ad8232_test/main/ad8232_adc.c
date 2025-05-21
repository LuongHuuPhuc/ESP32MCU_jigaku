/**
 * @name ECG su dung bo timer trong esp32
 * @note Bo timer la phan cung chuyen biet trong MCU esp32 dung de tao ra cac su kien 
 * dinh ky doc lap voi CPU. Day la ly do no khong bi WDT reset, trong khi vTaskDelay()
 * thi co the bi
 */
#include <stdio.h>
#include <stdint.h>
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "hal/adc_types.h"
#include "filter_master_ecg.h"
#include "esp_timer.h"

#define ADC_CHANNEL ADC_CHANNEL_1 //GPIO1 (tren Esp32c3), ADC_CHANNEL_6 (Esp32 - GPIO34)
#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_12 //Tang pham vi do 
#define ADC_WIDTH ADC_WIDTH_BIT_12
#define ADC_SAMPLE_RATE 500

static const char *TAG = "ADC";
TaskHandle_t readADTask_handle = NULL;

void adc_configure(){
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN); //Suy hao
  ESP_LOGI(TAG, "ADC Configured: Channel: %d, Attenuation: %d", ADC_CHANNEL, ADC_ATTEN);
}

void adc_sampling_callback(void *pvParameter){
  int sample = adc1_get_raw(ADC_CHANNEL);
  printf("%d\n", sample);
}

void esp_timer_setup(void){
  const esp_timer_create_args_t adc_timer_args = {
    .callback = &adc_sampling_callback,
    .arg = NULL,
    .name = "adc_timer",
    .dispatch_method = ESP_TIMER_TASK,
  };

  esp_timer_handle_t adc_timer;
  ESP_ERROR_CHECK(esp_timer_create(&adc_timer_args, &adc_timer));
  
  int64_t interval_us = 1000000 / ADC_SAMPLE_RATE;  //Microseconds
  ESP_LOGI(TAG, "Bat dau lay mau ADC moi %lld us (~%d Hz)", interval_us, ADC_SAMPLE_RATE);
  
  ESP_ERROR_CHECK(esp_timer_start_periodic(adc_timer, interval_us)); //Start timer lap
}

void app_main(void){
  adc_configure();
  esp_timer_setup();
  while(1){
    vTaskDelay(pdMS_TO_TICKS(1000)); //Khong lam gi ca, cho thoi gian callback
  }
}