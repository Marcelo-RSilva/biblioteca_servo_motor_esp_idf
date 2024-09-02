#ifndef SERVO_TOOLS_H
#define SERVO_TOOLS_H

#include "esp_err.h"
#include "servo_hw.h"  // Inclui a camada de abstração de hardware

typedef struct {
  uint8_t gpio_num;          // GPIO number for PWM
  ledc_mode_t mode;          // PWM mode
  ledc_channel_t channel;    // PWM channel
  ledc_timer_t timer;        // PWM timer
  uint32_t frequency;        // PWM frequency in Hz
  uint32_t duty_resolution;  // PWM duty resolution
  int current_angle; // Adiciona um campo para armazenar o ângulo atual

} ServoConfig;

typedef int ServoAngle;

esp_err_t servo_init(ServoConfig *config);
esp_err_t servo_set_angle(ServoConfig *config, int angle);
esp_err_t servo_get_angle(const ServoConfig *config, int *angle);


#endif // SERVO_TOOLS_H
