#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"

//Oled hien thi thong so
#include "lcd/oled_ssd1306.h"
#include "i2c/ssd1306_i2c.h"
#include "ssd1306_generic.h"
#include "ssd1306_fonts.h"
#include "ssd1306_1bit.h"

#define C3_SDA_PIN      GPIO_NUM_4
#define C3_SCL_PIN      GPIO_NUM_5
#define SSD1306_ADDR    INT8_C(0x3C)
#define NORMAL_CONTRAST UINT8_C(0x7F)

TaskHandle_t ssd1306_handle = NULL;

esp_err_t scan_i2c_device(uint8_t addr){
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  if(cmd == NULL) return ESP_FAIL;
  
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
  i2c_master_stop(cmd);

  esp_err_t ret = i2c_master_cmd_begin(GPIO_NUM_0, cmd, pdMS_TO_TICKS(100));
  i2c_cmd_link_delete(cmd);

  return ret;
}

void scan_i2c_bus(){
  ESP_LOGI(pcTaskGetName(NULL), "Dang quet i2c bus...");
  for(uint8_t addr = 0x03; addr <= 0x77; addr++){
    if(scan_i2c_device(addr) == ESP_OK){
      ESP_LOGI(pcTaskGetName(NULL), "Tim thay thiet bi o dia chi: 0x%2X", addr);
    }
  }
  ESP_LOGI(pcTaskGetName(NULL), "Quet hoan tat !");
}

const char *display_init(void){ 
  ssd1306_i2cInitEx(C3_SCL_PIN, C3_SDA_PIN, SSD1306_ADDR);
  ssd1306_128x32_init();
  ssd1306_setContrast(NORMAL_CONTRAST);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_displayOn();
  vTaskDelay(pdMS_TO_TICKS(10));

  return NULL; //Tra ve rong neu khoi tao thanh cong
}

void display_task(void *args){
  ssd1306_clearScreen();
  
  int x = 50, y = 40;
  int num = 0;
  int dir = 1; //1: tang, -1: giam
  char buffer[2];

  ssd1306_printFixedN(10, 20, "Hello World !", STYLE_BOLD, 0);
  ssd1306_printFixedN(10, 30, "Huu Phuc !", STYLE_NORMAL, 0);
  vTaskDelay(pdMS_TO_TICKS(100));

  while(1){ 
    //Xoa dung vung hien thi cu (o day ta xoa 1 block co chieu cao font la 8 pixel)
    ssd1306_clearBlock(x, y, x + 12, y + 8); //12 chieu rong vi so chiem tam do 

    snprintf(buffer, sizeof(buffer), "%d", num);
    ssd1306_printFixed(x, y, buffer, STYLE_BOLD);

    num += dir;
    if(num > 9){
      num = 9;
      dir = -1;
    }else if(num < 0){
      num = 0;
      dir = 1;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void app_main(void){
  const char *err = display_init();
  if(err){
    ESP_LOGE(pcTaskGetName(NULL), "Loi khoi tao man hinh");
    vTaskDelete(NULL);
  }
  else{
    scan_i2c_bus();
    ESP_LOGI(pcTaskGetName(NULL), "Cam bien khoi tao thanh cong !");
  } 

  xTaskCreate(display_task, "display task", 2048, NULL, 5, &ssd1306_handle);
}