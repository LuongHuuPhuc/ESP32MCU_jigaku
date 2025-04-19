#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "driver/i2c_types.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "vl53l0x.h"

//Oled hien thi thong so
#include "lcd/oled_ssd1306.h"
#include "i2c/ssd1306_i2c.h"
#include "ssd1306_generic.h"
#include "ssd1306_fonts.h"
#include "ssd1306_1bit.h"

#define SDA_PIN            GPIO_NUM_21
#define SCL_PIN            GPIO_NUM_22
#define XSHUT_PIN          GPIO_NUM_5
#define I2C_PORT           I2C_NUM_0
#define I2C_MASTER_CLK     100000
#define VL53L0X_ADDR       UINT8_C(0x52)
#define VL53L0X_7BITS_ADDR UINT8_C(0x29)
#define SSD1306_ADDR       INT8_C(0x3C)

#define CONFIG_PLATFORM_I2C_AVAILABLE
#define CONFIG_PLATFORM_I2C_ENABLE 

static const char *TAG = "VL53L0X";
TaskHandle_t vl53l0x_handle = NULL, ssd1306_handle = NULL;
vl53l0x_t *sensor = NULL;
static volatile uint16_t distance = 0;
volatile bool isNewData = false;

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

/**
 * @brief　Ham dieu khien chan xshut (chan dieu khien nguon)
 * xSHUT - GND => Cam bien tat hoan toan, khong phan hoi I2C
 * xSHUT - 3.3v => Cam bien bat va tat bat dau hoat dong
 * \note Nen pullup chan nay voi tro 10k
 */
void xshut_reset(){
  gpio_set_direction(XSHUT_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(XSHUT_PIN, 0); //Keo xuong thap -> Tat cam bien
  vTaskDelay(pdMS_TO_TICKS(10));
  gpio_set_level(XSHUT_PIN, 1); //Keo len cao -> Bat cam bien
  vTaskDelay(pdMS_TO_TICKS(10));
}

void display_task(void *pvParameter){
  char buf[30];
  ssd1306_clearScreen();
  // ssd1306_printFixed(15, 10, "Time of Flight", STYLE_BOLD);
  // ssd1306_printFixed(45, 10, "VL53L0X", STYLE_NORMAL);
  
  uint16_t last_distance = 0xFFFF; //Gia tri ban dau khong hop le

  while(1){
    if(isNewData && distance != last_distance){
      last_distance = distance;
      isNewData = false;
      ssd1306_clearBlock(0, 40, 128, 48);
      snprintf(buf, sizeof(buf), "Distance:%umm", distance);
      ssd1306_printFixed(0, 40, buf, STYLE_NORMAL);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

const char *oled_display_init(void){
  ssd1306_i2cInitEx(SCL_PIN, SDA_PIN, SSD1306_ADDR);
  ssd1306_128x64_init();
  ssd1306_setContrast(UINT8_C(0x7F));
  ssd1306_setFixedFont(ssd1306xled_font8x16);
  ssd1306_displayOn();

  return NULL; //Tra ve rong neu khoi tao thanh cong
}

void sensor_init(void){
  ESP_LOGI(TAG, "Khoi tao cam bien...");

  xshut_reset();
  sensor = vl53l0x_config(I2C_PORT, SCL_PIN, SDA_PIN, XSHUT_PIN, VL53L0X_7BITS_ADDR, true);
  if(!sensor){
    ESP_LOGE(TAG, "Khong tim thay cam bien !");
    vTaskDelete(NULL);
    return;
  }
  ESP_LOGI(TAG, "Da tim thay cam bien !");
  scan_i2c_bus();
  vl53l0x_setAddress(sensor, VL53L0X_7BITS_ADDR);
  
  const char *err = vl53l0x_init(sensor); //Tra ve NULL neu khong init duoc 
  if(err){  
    ESP_LOGE(TAG, "Loi khoi tao cam bien: %s", err);
    vTaskDelete(NULL);
    for(;;) vTaskDelay(1);
  } 
  ESP_LOGI(TAG, "Cam bien khoi tao thanh cong !");
  vTaskDelay(pdMS_TO_TICKS(100));
}

void vl53l0x_task(void *pvParameter){
  while(true){
    distance = vl53l0x_readRangeSingleMillimeters(sensor);
    printf("\n%umm", distance);
    isNewData = true;
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void app_main(){
  sensor_init(); //Chi khoi tao I2c mot lan cho ca 2 thiet bi 
  const char *err = oled_display_init();
  if(err){
    ESP_LOGE(pcTaskGetName(NULL), "Loi khoi tao man hinh !");
  }
  ESP_LOGI(pcTaskGetName(NULL), "Khoi tao man hinh thanh cong !");

  xTaskCreatePinnedToCore(vl53l0x_task, "readVL53L0X_task", 1024 * 3, NULL, 1, &vl53l0x_handle, 0);
  xTaskCreatePinnedToCore(display_task, "display task", 1024 * 3, NULL, 1, &ssd1306_handle, 1);
}