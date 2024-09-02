#include "servo_tools.h"

esp_err_t servo_init(ServoConfig *config) {
  if (!config) {
    return ESP_ERR_INVALID_ARG;
  }

  // Initialize the hardware for PWM
  return hw_servo_init(config->gpio_num);
}


esp_err_t servo_set_angle(ServoConfig *config, int angle) {
  if (!config || angle < 0 || angle > 180) {
    return ESP_ERR_INVALID_ARG;
  }

  uint32_t pulse_width_us = SERVO_MIN_PULSE_WIDTH_US + (angle * SERVO_MAX_PULSE_WIDTH_US  / 180);
  esp_err_t ret = hw_servo_set_pulse_width(config->gpio_num, pulse_width_us);
  if (ret == ESP_OK) {
    // Update the stored angle
    config->current_angle = angle;
  }
  return ret;
}

esp_err_t servo_get_angle(const ServoConfig *config, int *angle) {
  if (!config || !angle) {
    return ESP_ERR_INVALID_ARG;
  }

  *angle = config->current_angle;
  return ESP_OK;
}
