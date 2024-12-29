#include <stdio.h>
#include <driver/i2c_master.h>
#include <esp_log.h>
#include <esp_err.h>

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define I2C_MASTER_FREQ_HZ 100000 //Toc do bus i2c 100kHz
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_TIMEOUT_MS 1000 //Thoi gian choi i2c 

#define TAG "I2C_SCANNER"

static i2c_master_bus_handle_t i2c_bus_handle = NULL;

//Cau hinh i2c master (API moi cho I2C )
void i2c_master_init(){
    ESP_LOGI(TAG, "Dang khoi tao I2C Master...");
    i2c_master_bus_config_t i2c_master_conf = {
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .i2c_port = I2C_MASTER_NUM,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7, //Bo loc nhieu voi nguong 7 chu ky xung clock
        .intr_priority = 1, //Do uu tien ngat la 1
        .trans_queue_depth = 10, //Hang doi truyen 10
        .flags = {
            .enable_internal_pullup = 1 //Bat pull up noi bo
        }
    };

    //Khoi tao driver i2c voi cau hinh 
    esp_err_t ret = i2c_new_master_bus(&i2c_master_conf, &i2c_bus_handle);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "Khoi tao bus i2c that bai: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Khoi tao bus i2c master thanh cong !");
}

//Ham quet qua thiet bi i2c
void i2c_scan(){
    ESP_LOGI(TAG, "Bat dau quet thiet bi i2c...");
    int device_count = 0; //Dem so thiet bi

    for(uint8_t address = 1; address < 127; address++){
        //Tao cau hinh thiet bi i2c 
        i2c_device_config_t dev_conf = {
            .device_address = address,
            .scl_speed_hz = I2C_MASTER_FREQ_HZ  
        }; 

        //Xu ly thiet bi tren bus
        i2c_master_dev_handle_t dev_handle = NULL;
        esp_err_t ret = i2c_master_bus_add_device(i2c_bus_handle, &dev_conf, &dev_handle);

        if(ret == ESP_OK){
            ESP_LOGI(TAG, "Thiet bi duoc tim thay tai dia chi: 0x%02x", address);
            device_count++;

            //Xoa thiet bi sau khi da tim thay
            esp_err_t err = i2c_master_bus_rm_device(dev_handle);
            if(err != ESP_OK){
                ESP_LOGE(TAG, "Khong the xoa thiet bi tai dia chi: 0x%02x, loi: %s", address, esp_err_to_name(err));
            }
        }else if(ret == ESP_ERR_TIMEOUT){
            ESP_LOGW(TAG, "Timeout tai dia chi: 0x%02x", address);
        }else if(ret == ESP_ERR_INVALID_ARG || ret == ESP_ERR_INVALID_STATE){
            ESP_LOGE(TAG, "Cau hinh khong hop le tai dia chi: 0x%02x, loi: %s", address, esp_err_to_name(ret));
        }else if(ret == ESP_ERR_INVALID_RESPONSE){
            ESP_LOGE(TAG, "Thiet bi tai dia chi: 0x%02x khong phan hoi ", address);
        }
    }
    if(device_count > 0){
        ESP_LOGI(TAG, "Da tim thay %d thiet bi tren bus I2C.", device_count);
    }else{
        ESP_LOGW(TAG, "Khong tim thay thiet bi nao tren bus I2C.");
    }
    ESP_LOGI(TAG, "Quet i2c hoan thanh.");
}

void app_main(void){
    //Khoi tao i2xc master
    i2c_master_init();

    //Quet thi bi i2c
    i2c_scan(); 
}
