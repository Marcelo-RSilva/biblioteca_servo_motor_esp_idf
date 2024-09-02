#ifndef SERVO_HW_H
#define SERVO_HW_H

#include "esp_err.h"
#include "esp_log.h"
#include "driver/ledc.h"

esp_err_t hw_servo_init(uint8_t gpio_num);
esp_err_t hw_servo_set_pulse_width(uint8_t gpio_num, uint32_t pulse_width_us);
esp_err_t hw_servo_deinit(uint8_t gpio_num);

#endif // SERVO_HW_H
