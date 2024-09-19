#include "servo_tools.h"

esp_err_t servo_init(ServoConfig *config) {
  if (!config) {
    return ESP_FAIL;
  }

  // Initialize the hardware for PWM
  return hw_servo_init(config);
}


esp_err_t servo_set_angle(ServoConfig *config, int angle) {
  if (!config || angle < 0 || angle > 181) {
    return ESP_FAIL;
  }

  uint32_t pulse_width_us = SERVO_MIN_PULSE_WIDTH_US + (angle * (SERVO_MAX_PULSE_WIDTH_US - SERVO_MIN_PULSE_WIDTH_US ) / 181);
  esp_err_t ret = hw_servo_set_pulse_width(config, pulse_width_us);
  if (ret == ESP_OK) {
    config->current_angle = angle;
  }
  return ret;
}

esp_err_t servo_get_angle(const ServoConfig *config, int *angle) {
  if (!config || !angle) {
    return ESP_FAIL;
  }

  *angle = config->current_angle;
  return ESP_OK;
}