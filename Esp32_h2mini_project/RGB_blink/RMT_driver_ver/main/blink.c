/**
 * @file blink.c
 * @date 2025/09/29
 * @author Luong Huu Phuc
 */

#ifdef __cplusplus
  #define EXTERN_C "C"
#else
  #define EXTERN_C
#endif //__cplusplus

#include "stdio.h"
#include "stdlib.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "driver/rmt_rx.h"
#include "esp_err.h"
#include "esp_log.h"
#include "rmt_usercfg.h"

static const char *TAG = "__main";
 
void app_main(void){

}