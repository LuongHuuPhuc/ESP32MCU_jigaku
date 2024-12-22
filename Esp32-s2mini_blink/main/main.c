#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#define BLINK_LED_GPIO 15 //Led nhay tren board
#define TAG "LED"
void app_main(){
    //Set gpio as output
    gpio_reset_pin(BLINK_LED_GPIO); //Reset trang thai pin 
    gpio_set_direction(BLINK_LED_GPIO, GPIO_MODE_OUTPUT); //Dat che do output GPIO

    while(1){
        //Bat led
        gpio_set_level(BLINK_LED_GPIO, 1);
        ESP_LOGI(TAG, "Led is on");
        vTaskDelay(200 / portTICK_PERIOD_MS); //Delay 0.5s

        gpio_set_level(BLINK_LED_GPIO, 0);
        ESP_LOGI(TAG, "Led is off");
        vTaskDelay(200 /portTICK_PERIOD_MS); 
    }
}