/**
 * @brief Bai test giam thieu key debounc(nay phim) su dung pullup resistor bang phan mem
 * @date 2025/04/27
 * @author Luong Huu Phuc
 */
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "freertos/FreeRTOS.h" //Phai de thu vien nay len truoc thi moi build duoc !!!
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "led_button.h"

#define BUTTON1    GPIO_NUM_14
#define BUTTON2    GPIO_NUM_13
#define GREEN_LED  GPIO_NUM_12
#define RED_LED    GPIO_NUM_11
#define YELLOW_LED GPIO_NUM_10

static int button1_state = 0; //0->1->2->3->0
static int button2_state = 0; //
TaskHandle_t led_button_handle = NULL;

/***** FUNCTION DEFINITION ******/

/**
 * @brief Phep chia modulo (lay du)
 * \note Phep toan khong phai chia kieu so thuc
 * \note Phep chia chi lay thuong la so nguyen (khong phai thap phan) + so du
 */
int next_state(int x){
  x = (x + 1) % 4; 
  return x;
}

void button1_active(void){
  button1_state = next_state(button1_state); //Tang trang thai 

  switch(button1_state){
    case 0: 
    gpio_set_level(RED_LED, 1);
    gpio_set_level(YELLOW_LED, 0);
    gpio_set_level(GREEN_LED, 0);
    break;

    case 1:
    gpio_set_level(RED_LED, 0);
    gpio_set_level(YELLOW_LED, 1);
    gpio_set_level(GREEN_LED, 0);
    break;

    case 2: 
    gpio_set_level(RED_LED, 0);
    gpio_set_level(YELLOW_LED, 0);
    gpio_set_level(GREEN_LED, 1);
    break;

    case 3: 
    gpio_set_level(RED_LED, 1);
    gpio_set_level(YELLOW_LED, 1);
    gpio_set_level(GREEN_LED, 1);
    break;
    
  }
}

void button2_active(void){
  button2_state = next_state(button2_state);
  
  switch(button2_state){
    case 0: 
    gpio_set_level(RED_LED, 1);
    gpio_set_level(YELLOW_LED, 1);
    gpio_set_level(GREEN_LED, 0);
    break;

    case 1:
    gpio_set_level(RED_LED, 1);
    gpio_set_level(YELLOW_LED, 0);
    gpio_set_level(GREEN_LED, 1);
    break;

    case 2: 
    gpio_set_level(RED_LED, 0);
    gpio_set_level(YELLOW_LED, 1);
    gpio_set_level(GREEN_LED, 1);
    break;

    case 3: 
    gpio_set_level(RED_LED, 1);
    gpio_set_level(YELLOW_LED, 1);
    gpio_set_level(GREEN_LED, 1);
    break;
  }
}

void rm_keyDebounce(int button){
  if(gpio_get_level(button) == 0){ //Neu button duoc nhan (active low)
    vTaskDelay(pdMS_TO_TICKS(20)); //Delay 20ms (khu nhieu)
    if(gpio_get_level(button) == 0){ //Kiem tra lai lan nua
      if(button == BUTTON1){
        button1_active();
      }else if(button == BUTTON2){
        button2_active();
      }

      //Wait to release button 
      while(gpio_get_level(button) == 0){ //Neu van dang giu nut
        vTaskDelay(pdMS_TO_TICKS(10));
      }
    }
  }
}

void gpio_init(void){
  gpio_config_t io_config = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL << BUTTON1) | (1ULL << BUTTON2),
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE
  };
  gpio_config(&io_config);
  ESP_LOGI(pcTaskGetName(NULL), "Button initialized OK !");

  io_config.mode = GPIO_MODE_OUTPUT;
  io_config.pin_bit_mask = (1ULL << GREEN_LED) | (1ULL << RED_LED) | (1ULL << YELLOW_LED);
  gpio_config(&io_config);
  ESP_LOGI(pcTaskGetName(NULL), "Led pin initialized OK !");

  //Tat het led ban dau
  gpio_set_level(RED_LED, 0);
  gpio_set_level(YELLOW_LED, 0);
  gpio_set_level(GREEN_LED, 0);
}

void led_button_task(void *pvParameter){
  vTaskDelay(pdMS_TO_TICKS(1));
  while(1){
    rm_keyDebounce(BUTTON1);
    rm_keyDebounce(BUTTON2);
    vTaskDelay(pdMS_TO_TICKS(10)); //Delay 10ms
  }
}

/******* MAIN *******/
void app_main(void){
  gpio_init();
  xTaskCreate(led_button_task, "led button task", 2048 * 2, NULL, 5, &led_button_handle);
}

