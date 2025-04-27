#ifndef LED_BUTTON_H
#define LED_BUTTON_H

#include <stdio.h>
#include <stdint.h>

#pragma once 

#ifdef __cplusplus
extern "C" {
#endif 

int next_state(int x);
void button1_active(void);
void button2_active(void);
void rm_keyDebounce(int button);
void gpio_init(void);
void led_button_task(void *pvParameter);

#ifdef __cplusplus
}
#endif 

#endif //LED_BUTTON_H