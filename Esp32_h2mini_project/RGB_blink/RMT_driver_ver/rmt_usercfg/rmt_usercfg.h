/**
 * @file rmt_usercfg.h
 * @author Luong Huu Phuc 
 * @brief Cau hinh va cai dat rmt cho chuong trinh thong qua driver/rmt.h
 */

#ifndef RMT_USERCONFIG__
#define RMT_USERCONFIG__

#ifdef __cplusplus
  #define EXTERN_C "C"
#else 
  #define EXTERN_C
#endif //__cplusplus

#pragma once 

#include "stdio.h"
#include "driver/rmt.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_encoder.h"
#include "esp_err.h"
#include "esp_log.h"

/****************** CONFIG & INSTALL FUCNTION ****************/

esp_err_t rmt_setup_config();

/****************** RECEIVER FUCNTION ****************/

/****************** TRANSMITTER FUCNTION ****************/

/****************** ENCODER FUCNTION ****************/

#endif //__RMT_USERCONFIG__

