#include "servo_hw.h"

esp_err_t hw_servo_init(uint8_t gpio_num) {
  // Configure the LEDC timer
  ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .duty_resolution = LEDC_TIMER_13_BIT,
    .freq_hz = SERVO_PWM_FREQUENCY,
    .clk_cfg = LEDC_AUTO_CLK
  };
  esp_err_t ret = ledc_timer_config(&ledc_timer);
  if (ret != ESP_OK) {
    return ret;
  }

  // Configure the LEDC channel
  ledc_channel_config_t ledc_channel = {
    .gpio_num = gpio_num,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  };
  ret = ledc_channel_config(&ledc_channel);
  if (ret != ESP_OK) {
    return ret;
  }

  // Initialize the servo by sweeping
  for (int angle = 0; angle <= 180; angle += 10) {
    hw_servo_set_pulse_width(gpio_num, 500 + (angle * 2000 / 180));
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    hw_servo_set_pulse_width(gpio_num, 500 + (angle * 2000 / 180));
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  return ESP_OK;
}

esp_err_t hw_servo_set_pulse_width(uint8_t gpio_num, uint32_t pulse_width_us) {
  if (pulse_width_us < SERVO_MIN_PULSE_WIDTH_US || pulse_width_us > SERVO_MAX_PULSE_WIDTH_US) {
    ESP_LOGE(TAG, "Pulse width out of range");
    return ESP_ERR_INVALID_ARG;
  }

  uint32_t duty_cycle = (pulse_width_us * (1 << SERVO_PWM_RESOLUTION)) / (1000000 / SERVO_PWM_FREQUENCY);
  return ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty_cycle);
}

esp_err_t hw_servo_deinit(uint8_t gpio_num) {
  // Stop the PWM and deinitialize
  ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
  ledc_timer_pause(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0);
  return ESP_OK;
}