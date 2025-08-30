#include"vl53l0x.h"
#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"esp_err.h"
#include"driver/i2c_types.h"
#include"driver/i2c.h"
#include"esp_log.h"

#define SDA_PIN GPIO_NUM_6
#define SCL_PIN GPIO_NUM_7
#define XSHUT_PIN GPIO_NUM_5
#define I2C_PORT I2C_NUM_0
#define I2C_MASTER_CLK 400000 
#define VL53L0X_ADDR 0x52 //Dia chi mac dinh
#define VL53L0X_ADDR2 0x29

static const char *TAG = "VL53L0X";
TaskHandle_t vl53l0x_handle = NULL;
vl53l0x_t *sensor = NULL;

/**
 * @brief Ham khoi tao I2C
 */
void i2c_legacy_init(){
  ESP_LOGI(TAG, "Dat lai chan...");
  gpio_reset_pin(SDA_PIN);
  gpio_reset_pin(SCL_PIN);  

  i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,
    .master.clk_speed = I2C_MASTER_CLK,
    .sda_io_num = SDA_PIN,
    .scl_io_num = SCL_PIN,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .clk_flags = 0,
  };

  ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_config));

  esp_err_t err = i2c_driver_install(I2C_PORT, i2c_config.mode, 0, 0, 0);
  if(err != ESP_OK){
    ESP_LOGE(TAG, "Loi cai dat driver: %s", esp_err_to_name(err));
  }else{
    ESP_LOGI(TAG, "Cai dat driver thanh cong !");
  }
}

/**
 * @brief Ham kiem tra xem co thiet bi tren bus I2C khong
 */
esp_err_t scan_i2c_device(uint8_t addr){
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
  i2c_master_stop(cmd);

  esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
  i2c_cmd_link_delete(cmd);

  return ret;
}

/**
 * @brief　Ham dieu khien chan xshut (chan dieu khien nguon)
 * xSHUT - GND => Cam bien tat hoan toan, khong phan hoi I2C
 * xSHUT - 3.3v => Cam bien bat va tat bat dau hoat dong
 */
void xshut_reset(){
  gpio_set_pull_mode(XSHUT_PIN, GPIO_PULLUP_ONLY);
  gpio_set_direction(XSHUT_PIN, GPIO_MODE_OUTPUT);
  
  gpio_set_level(XSHUT_PIN, 0);
  vTaskDelay(pdMS_TO_TICKS(10)); //Giu muc thap de reset
  gpio_set_level(XSHUT_PIN, 1);
  vTaskDelay(pdMS_TO_TICKS(10)); //Giu muc cao de bat cam bien 
}

/**
 * @brief Ham thuc hien task
 * @param pvParamter Khong co tham so dau vao
 */
void vl53l0x_task(void *pvParameter){
  ESP_LOGI(TAG, "Khoi tao cam bien...");
  
  sensor = vl53l0x_config(I2C_PORT, SCL_PIN, SDA_PIN, XSHUT_PIN, VL53L0X_ADDR, true);
  if (sensor == NULL) {
    ESP_LOGE(TAG, "Cau hinh cam bien vl53l0x that bai !");
    vTaskDelete(NULL);
    return;
  }
  ESP_LOGI(TAG, "Cau hinh cam bien vl53l0x thanh cong !");

  const char *err = vl53l0x_init(sensor);
  if(err){
    ESP_LOGE(TAG, "Loi khoi tao cam bien: %s", err);
    vTaskDelete(NULL);
    return;
  }
  ESP_LOGI(TAG, "Cam bien khoi tao thanh cong !");

  while(true){
    uint16_t distance = vl53l0x_readRangeSingleMillimeters(sensor);
    printf("\n%umm", distance);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/**
 * @brief Ham chinh
 */
void app_main(){
  ESP_LOGI(TAG, "Khoi tao i2c...");

  i2c_legacy_init();
  xshut_reset();
  if(scan_i2c_device(VL53L0X_ADDR >> 1) == ESP_OK){
    ESP_LOGI(TAG, "Phat hien cam bien vl53l0x tai dia chi: 0x%2X", VL53L0X_ADDR);
  }else if(scan_i2c_device(VL53L0X_ADDR2 >> 1) == ESP_OK){
    ESP_LOGI(TAG, "Phat hien cam bien vl53l0x tai dia chi 0x29");
  }else{
    ESP_LOGE(TAG, "Khong phat hien thay cam bien !");
  }

  xTaskCreatePinnedToCore(&vl53l0x_task, "Read_vl53l0x_task", 4096, NULL, configMAX_PRIORITIES -1, &vl53l0x_handle, 0);
}