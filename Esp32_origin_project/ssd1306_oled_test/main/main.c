#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/projdefs.h"

//Thu vien ssd1306
#include "lcd/oled_ssd1306.h"
#include "ssd1306_generic.h"
#include "ssd1306_fonts.h"
#include "ssd1306_1bit.h"

//Cau hinh chan spi     
#define SPI_MOSI_PIN 23 
#define SPI_SCK_PIN 18
#define SPI_CS_PIN 5
#define SPI_DC_PIN 2 //Data/command 
#define SPI_RST_PIN 4

#define TAG "ssd1306"

void spi_master_init(void){
    //Cau hinh giao tiep spi
    spi_bus_config_t bus_config = {
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = 4096, //Kich thuoc du lieu truyen toi da
        .mosi_io_num = SPI_MOSI_PIN,
        .miso_io_num = -1, //Khong dung
        .sclk_io_num = SPI_SCK_PIN,
        .flags = SPICOMMON_BUSFLAG_MASTER, //Them co nay chi dinh la master 
        
    };

    //Khoi tao SPI bus 
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_CH_AUTO));
}

void display_task(void *pvParameters){
    while(1){

    ssd1306_fillScreen(0x00); //Xoa man hinh

    ssd1306_setFixedFont(ssd1306xled_font6x8); //Set font chu 6x8

    //Hien thi chu len man hinh 
    ssd1306_printFixed(0, 8, "HELLO ESP32 !", STYLE_BOLD);
    vTaskDelay(pdMS_TO_TICKS(1000));

    vTaskDelay(1000);
    ssd1306_fillScreen(0x00); //Xoa man hinh 
    ssd1306_printFixed(0, 16, "HELLO PHUC !", STYLE_BOLD);

    ssd1306_negativeMode(); //Chuyen sang che do am ban
    ssd1306_printFixed(0, 8, "Cu bi lua 9tr", STYLE_BOLD);
    vTaskDelay(pdMS_TO_TICKS(5000));        

    //Chuyen lai che do binh thuong 
    ssd1306_positiveMode();

    //Giu man hinh trong 5s
    vTaskDelay(pdMS_TO_TICKS(5000));
    }    
}

void app_main(void){
    //Khoi tao giao tiep spi
    ESP_LOGI(TAG, "Dang khoi tao SPI...");
    spi_master_init();
    ESP_LOGI(TAG, "Khoi tao thanh cong SPI !");


    ESP_LOGI(TAG, "Dang khoi tao man hinh oled...");
    ssd1306_128x64_spi_init(SPI_RST_PIN, SPI_CS_PIN, SPI_DC_PIN);

    ESP_LOGI(TAG, "Khoi tao man hinh oled thanh cong !");
    ESP_LOGI(TAG, "Dang bat man hinh oled...");

    //Reset man hinh neu can 
    /*gpio_set_direction(SPI_RST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(SPI_RST_PIN, 0); //Dua chan rst xuong thap
    vTaskDelay(pdMS_TO_TICKS(100)); //Cho 100ms
    gpio_set_level(SPI_RST_PIN, 1); */ //Dua chan rst lai len cao

    //Bat man hinh hien thi
    ssd1306_displayOn();

    //Task hien thi man hinh 
    xTaskCreate(display_task, "Display Task", 4096, NULL, 5, NULL);

}