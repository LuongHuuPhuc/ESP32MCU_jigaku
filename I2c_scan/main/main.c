#include <stdio.h>
#include <driver/i2c_master.h>
#include <esp_log.h>
#include <esp_err.h>

#define I2C_SDA_PIN 8
#define I2C_SCL_PIN 9
#define I2C_MASTER_FREQ_HZ 100000 // Tốc độ bus I2C 100kHz
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_TIMEOUT_MS 1000 // Thời gian timeout cho I2C

#define TAG "I2C_SCANNER"

static i2c_master_bus_handle_t i2c_bus_handle = NULL;

// Cấu hình i2c master (API mới cho I2C)
void i2c_master_init(){
    ESP_LOGI(TAG, "Đang khởi tạo I2C Master...");
    i2c_master_bus_config_t i2c_master_conf = {
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .i2c_port = I2C_MASTER_NUM,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7, // Bộ lọc nhiễu với ngưỡng 7 chu kỳ xung clock
        .intr_priority = 1, // Độ ưu tiên ngắt là 1
        .trans_queue_depth = 10, // Hàng đợi truyền 10
        .flags = {
            .enable_internal_pullup = 1 // Bật pull-up nội bộ
        }
    };

    // Khởi tạo driver I2C với cấu hình
    esp_err_t ret = i2c_new_master_bus(&i2c_master_conf, &i2c_bus_handle);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "Khởi tạo bus I2C thất bại: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "Khởi tạo bus I2C master thành công!");
}

// Hàm quét qua thiết bị I2C
void i2c_scan(){
    ESP_LOGI(TAG, "Bắt đầu quét thiết bị I2C...");
    int device_count = 0; // Đếm số thiết bị

    for(uint8_t address = 1; address < 127; address++){
        // Tạo cấu hình thiết bị I2C
        i2c_device_config_t dev_conf = {
            .device_address = address,
            .scl_speed_hz = I2C_MASTER_FREQ_HZ  
        }; 

        // Xử lý thiết bị trên bus
        i2c_master_dev_handle_t dev_handle = NULL;
        esp_err_t ret = i2c_master_bus_add_device(i2c_bus_handle, &dev_conf, &dev_handle);

        if(ret == ESP_OK){
            ESP_LOGI(TAG, "Thiết bị được tìm thấy tại địa chỉ: 0x%02x", address);
            device_count++;

            // Xóa thiết bị sau khi đã tìm thấy
            esp_err_t err = i2c_master_bus_rm_device(dev_handle);
            if(err != ESP_OK){
                ESP_LOGE(TAG, "Không thể xóa thiết bị tại địa chỉ: 0x%02x, lỗi: %s", address, esp_err_to_name(err));
            }
        }else if(ret == ESP_ERR_TIMEOUT){
            ESP_LOGW(TAG, "Timeout tại địa chỉ: 0x%02x", address);
        }else if(ret == ESP_ERR_INVALID_ARG || ret == ESP_ERR_INVALID_STATE){
            ESP_LOGE(TAG, "Cấu hình không hợp lệ tại địa chỉ: 0x%02x, lỗi: %s", address, esp_err_to_name(ret));
        }else if(ret == ESP_ERR_INVALID_RESPONSE){
            ESP_LOGE(TAG, "Thiết bị tại địa chỉ: 0x%02x không phản hồi", address);
        }
    }
    if(device_count > 0){
        ESP_LOGI(TAG, "Đã tìm thấy %d thiết bị trên bus I2C.", device_count);
    }else{
        ESP_LOGE(TAG, "Không tìm thấy thiết bị nào trên bus I2C.");
    }
    ESP_LOGI(TAG, "Quét I2C hoàn thành.");
}

void app_main(void){
    // Khởi tạo I2C master
    i2c_master_init();

    // Quét thiết bị I2C
    i2c_scan(); 
}
