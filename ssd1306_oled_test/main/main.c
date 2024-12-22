#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c.h>
#include "ssd1306.h"
#include "esp_log.h"
#include "esp_err.h"

#define I2C_MASTER_SCL_IO 22 
#define I2C_MASTER_SDA_IO 21 
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000 //Toc do truyen i2c
//#define OLED_ADDR 0x3C  //Dia chi cua ssd1306 (i2c address)

void i2c_master_init(void) {
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    //Cau hinh i2c va cai dat driver
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, 0, 0, 0));
}

void app_main(void){
    //Khoi tao giao tiep i2c
    i2c_master_init();

    //Khoi tao man hinh oled
    ssd1306_128x64_i2c_init();

    //Xoa man hinh 
    ssd1306_clearScreen();

    //Hien thi chu len man hinh 
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_printFixed(18, 18, "Hello ESP32 !", STYLE_BOLD);
    ssd1306_negativeMode();
    ssd1306_printFixed(0, 18, "Hello Phuc !", STYLE_NORMAL);
    ssd1306_positiveMode();

    //delay de man hinh hien thi chu 
    vTaskDelay(pdMS_TO_TICKS(5000));

    //Xoa man hinh 
    ssd1306_clearScreen();
    ssd1306_displayOn();
}
