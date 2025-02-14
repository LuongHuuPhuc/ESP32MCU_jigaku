#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "led_strip_rmt_encoder.h"

#define LED_STRIP_GPIO 8
#define LED_STRIP_RES_HS (uint32_t)1e7  //10MHz
#define LED_STRIP_NUM_PIXELS 1 //So luong LED (chi 1 LED tren board)
#define TAG "WS2812_RGB"

static rmt_channel_handle_t led_chan = NULL;
static rmt_encoder_handle_t led_encoder = NULL;

// Cấu hình RMT transmit không lặp vô hạn
rmt_transmit_config_t tx_config = {
    .loop_count = 0 // Chạy 1 lần mỗi màu
};

//Rainbow Gradient Effect using HSV formula
void hsv_to_rgb(int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b){
    float hh, p, q, t, ff;
    long i;

    if(s <= 0.0){ //Khi do bao hoa = 0 thi chi co mau xam
        *r = v;
        *g = v;
        *b = v;
        return;
    }

    hh = h / 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - (s / 255.0));
    q = v * (1.0 - (s / 255.0) * ff);
    t = v * (1.0 - (s / 255.0) * (1.0 - ff));

    switch(i){
        case 0: *r = v; *g = t; *b = p; break;
        case 1: *r = q; *g = v; *b = p; break;
        case 2: *r = p; *g = v; *b = t; break;
        case 3: *r = p; *g = q; *b = v; break;
        case 4: *r = t; *g = p; *b = v; break;
        case 5: default: *r = v; *g = p; *b = q; break;

    }
}

void rainbowEffect_task(int duration_ms){
    int hue = 0;
    uint8_t r, g, b;
    uint8_t led_color[3];
    int iterations = duration_ms / 50; //So lan lap (50ms moi lan)

    for(int i = 0; i < iterations; i++){
        hsv_to_rgb(hue, 255, 255, &r, &g, &b);
        led_color[0] = r;
        led_color[1] = g;
        led_color[2] = b;
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_color, sizeof(led_color), &tx_config));

        hue += 5; //Tang hue de chuyen doi mau
        if(hue >= 360){
            hue = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void app_main(void) {
  ESP_LOGI(TAG, "Configuring WS2812 LED...");
  
  // Cấu hình RMT
  rmt_tx_channel_config_t tx_chan_config = {
      .gpio_num = LED_STRIP_GPIO,
      .clk_src = RMT_CLK_SRC_DEFAULT,
      .resolution_hz = LED_STRIP_RES_HS,
      .mem_block_symbols = 64,
      .trans_queue_depth = 4,
  };
  ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

  // Cấu hình encoder cho LED WS2812
  led_strip_encoder_config_t encoder_config = {
      .resolution = LED_STRIP_RES_HS,
      .led_model = LED_MODEL_WS2812,
  };
  ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

  // Kích hoạt RMT
  ESP_ERROR_CHECK(rmt_enable(led_chan));

  uint8_t led_data[3];

  while (true) {
      ESP_LOGI(TAG, "Setting LED to RED...");
      led_data[0] = 255; 
      led_data[1] = 0; 
      led_data[2] = 0;
      ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_data, sizeof(led_data), &tx_config));
      vTaskDelay(pdMS_TO_TICKS(1000));

      ESP_LOGI(TAG, "Setting LED to Green...");
      led_data[0] = 0; 
      led_data[1] = 255; 
      led_data[2] = 0;
      ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_data, sizeof(led_data), &tx_config));
      vTaskDelay(pdMS_TO_TICKS(1000));

      ESP_LOGI(TAG, "Setting LED to Blue...");
      led_data[0] = 0;
      led_data[1] = 0; 
      led_data[2] = 255;
      ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_data, sizeof(led_data), &tx_config));
      vTaskDelay(pdMS_TO_TICKS(1000));

      ESP_LOGI(TAG, "Setting LED to Rainbow...");
      rainbowEffect_task(10000);
  }
}
