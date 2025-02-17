/**
 * @brief Chuong trinh dieu khien module LCD HD44780 su dung giao tiep I2C
 */
#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"esp_err.h"
#include"driver/i2c_types.h"
#include"driver/i2c.h"
#include"esp_lcd_io_i2c.h"
#include"esp_log.h"

//Cau hinh I2C
#define I2C_SCL_PIN 7
#define I2C_SDA_PIN 6
#define I2C_PORT    I2C_NUM_0
#define LCD_ADDR    0x27 //Dia chi driver HLF8574T

//Dia chi cac lenh dieu khien LCD
#define LCD_BACKLIGHT     0x08
#define LCD_ENABLE        0x04
#define LCD_CMD           0
#define LCD_DATA          1
#define LCD_CLEAR         0x01
#define LCD_HOME          0x02
#define LCD_ENTRY_MODE    0x06
#define LCD_DISPLAY_ON    0x0C
#define LCD_FUNCTION      0x28 //4-bit

static const char *TAG = "LCD";
/**
 * @brief Ham kiem tra xem co thiet bi nao tren bus I2C khong
 */
esp_err_t scan_i2c_device(uint8_t addr){
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  if(cmd == NULL) return ESP_FAIL;
  
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
  i2c_master_stop(cmd);

  esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(100));
  i2c_cmd_link_delete(cmd);

  return ret;
}

//Quet toan bo dia chi I2C
void scan_i2c_bus(){
  ESP_LOGI(TAG, "Dang quet i2c bus...");
  for(uint8_t addr = 0x03; addr <= 0x77; addr++){
    if(scan_i2c_device(addr) == ESP_OK){
      ESP_LOGI(TAG, "Tim thay thiet bi o dia chi: 0x%2X", addr);
    }
  }
  ESP_LOGI(TAG, "Quet hoan tat !");
}

/**
 * @brief Ham gui data LCD
 * 
 * @param value Dia chi can gui
 * @param mode Che do LCD
*/
void lcd_send_data(uint8_t value, uint8_t mode){
  uint8_t data_high = (value & 0xF0) | LCD_BACKLIGHT | mode;
  uint8_t data_low = ((value << 4) & 0xF0) | LCD_BACKLIGHT | mode;

  uint8_t data[] = {
    data_high | LCD_ENABLE, data_high, //Gui 4 bit cao
    data_low | LCD_ENABLE, data_low //Gui 4 bit thap
  };

  i2c_master_write_to_device(I2C_PORT, LCD_ADDR, data, sizeof(data), 1000 / portTICK_PERIOD_MS);
  vTaskDelay(pdMS_TO_TICKS(1));
}

//Gui lenh den LCD
esp_err_t lcd_command(uint8_t cmd){
  lcd_send_data(cmd, LCD_CMD);
  vTaskDelay(pdMS_TO_TICKS(2)); //Delay nho
  return ESP_OK;
}

//Gui ky tu ra LCD
void lcd_write(uint8_t chr){
  lcd_send_data(chr, LCD_DATA);
}

//In chuoi ra LCD
void lcd_print(const char *str){
  while(*str){
    lcd_write((uint8_t)*str++);
  }
}

//Dat con tro den vi tri (row, col)
void lcd_set_cursor(uint8_t row, uint8_t col){
  uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
  lcd_command(address); 
}

/**
 * @brief Ham khoi tao LCD
 * @details Khoi tao LCD duoc thuc hien theo thu tu sau (theo datasheet):
 * 1. Gui lenh 0x03 3 lan (cho che do 8-bit), moi lan cach nhau 5ms de chac chan LCD nhan duoc lenh
 * 2. Gui lenh 0x02 (chuyen qua che do 4-bit)
 * 3. Gui lenh 0x28 (khoi tao che do 4-bit, 2 hang, font 5x8)
 * 4. Gui lenh 0x0C (bat hien thi, tat con tro)
 * ....
 */
esp_err_t lcd_init(){
  esp_err_t err3, err4, err5, err6;
  vTaskDelay(pdMS_TO_TICKS(50)); //Doi LCD khoi dong

  //Trang thai 1 (cau hinh 8-bit, duoc gui 3 lan)
  lcd_command(0x03); //Che do 8-bit
  vTaskDelay(pdMS_TO_TICKS(5));

  lcd_command(0x03); //Che do 8-bit
  vTaskDelay(pdMS_TO_TICKS(5));

  lcd_command(0x03); //Che do 8-bit
  vTaskDelay(pdMS_TO_TICKS(1));

  //Trang thai 2 (cau hinh 4-bit, cho 4 bit dau tien)
  lcd_command(0x02);
  vTaskDelay(pdMS_TO_TICKS(1));

  err3 = lcd_command(LCD_FUNCTION);
  if(err3 != ESP_OK) return err3;

  err4 = lcd_command(LCD_DISPLAY_ON);
  if(err4 != ESP_OK) return err4;

  err5 = lcd_command(LCD_CLEAR);
  if(err5 != ESP_OK) return err5;

  err6 = lcd_command(LCD_ENTRY_MODE);
  if(err6 != ESP_OK) return err6;

  return ESP_OK; 
}

//Khoi tao I2C
void i2c_init(){
  i2c_config_t i2c_conf = {
    .mode = I2C_MODE_MASTER,
    .master.clk_speed = 100000,
    .sda_io_num = I2C_SDA_PIN,
    .scl_io_num = I2C_SCL_PIN,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
  };

  ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_conf));
  esp_err_t ret = i2c_driver_install(I2C_PORT, i2c_conf.mode, 0, 0, 0);
  if(ret != ESP_OK){
    ESP_LOGE(TAG, "Loi cai dat driver: %s", esp_err_to_name(ret));
  }else{
    ESP_LOGI(TAG, "Cai dat driver thanh cong !");
  }
  
}

/**
 * @brief Ham chinh
 */
void app_main(){
  i2c_init();
  scan_i2c_bus();
  esp_err_t err = lcd_init();
  if(err == ESP_OK){
    ESP_LOGI(TAG, "Khoi tao LCD thanh cong !");
  }else{
    ESP_LOGE(TAG, "Khoi tao LCD that bai !: %s", esp_err_to_name(err));
  }

  lcd_set_cursor(0, 0);
  lcd_print("Nagata Asahi !!!");
  lcd_set_cursor(1, 0);
  lcd_print("Que Hyogo !");
}