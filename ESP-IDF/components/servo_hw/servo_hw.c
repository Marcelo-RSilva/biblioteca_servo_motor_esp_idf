#include "servo_hw.h"

esp_err_t hw_servo_init(ServoConfig *config) {
  // Configure the LEDC timer
  ledc_timer_config_t ledc_timer = {
    .speed_mode = config->mode,
    .timer_num = config->timer,
    .duty_resolution = config->duty_resolution,
    .freq_hz = config->frequency,
    .clk_cfg = LEDC_AUTO_CLK
  };
  esp_err_t ret = ledc_timer_config(&ledc_timer);
  if (ret != ESP_OK) {
    return ESP_FAIL;
  }

  // Configure the LEDC channel
  ledc_channel_config_t ledc_channel = {
    .gpio_num = config->gpio_num,
    .speed_mode = config->mode,
    .channel = config->channel,
    .timer_sel = config->timer,
    .duty = 0,
    .hpoint = 0
  };
  ret = ledc_channel_config(&ledc_channel);
  if (ret != ESP_OK) {
    return ESP_FAIL;
  }

  // Move the servo from 0 to 180 degrees and back to 0 for verification
  for (int angle = 0; angle <= 180; angle += 10) {
    hw_servo_set_pulse_width(config, 500 + (angle * 2000 / 180));
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    hw_servo_set_pulse_width(config, 500 + (angle * 2000 / 180));
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  return ESP_OK;
}

esp_err_t hw_servo_set_pulse_width(ServoConfig *config, uint32_t pulse_width_us) {
  if (pulse_width_us < SERVO_MIN_PULSE_WIDTH_US || pulse_width_us > SERVO_MAX_PULSE_WIDTH_US) {
    return ESP_FAIL;
  }

  uint32_t duty_cycle = (pulse_width_us * (1 << SERVO_PWM_RESOLUTION)) / (1000000 / SERVO_PWM_FREQUENCY);
  esp_err_t ret = ledc_set_duty(config->mode, config->channel, duty_cycle);
  if (ret == ESP_OK) {
    ledc_update_duty(config->mode, config->channel);
  }
  return ret;
}


esp_err_t hw_servo_deinit(uint8_t gpio_num) {
  esp_err_t ret;

  // Stop the PWM
  ret = ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0);
  if (ret != ESP_OK) {
    return ESP_FAIL;
  }

  // Pause the timer
  ret = ledc_timer_pause(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0);
  if (ret != ESP_OK) {
    return ESP_FAIL;
  }

  return ESP_OK;
}