#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_random.h"
#include "servo_tools.h"


// Define constants for PWM
#define SERVO_PWM_FREQUENCY 50 // PWM frequency in Hz
#define SERVO_PWM_RESOLUTION LEDC_TIMER_13_BIT // PWM resolution
#define SERVO_MIN_PULSE_WIDTH_US 500 // Minimum pulse width in microseconds
#define SERVO_MAX_PULSE_WIDTH_US 2500 // Maximum pulse width in microseconds

static const char *TAG = "servo";


void app_main(void) {
  ServoConfig servo_config = {
    .gpio_num = PORTA_GPIO, // Defina o GPIO que você está usando
    .mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer = LEDC_TIMER_0,
    .frequency = SERVO_PWM_FREQUENCY,
    .duty_resolution = LEDC_TIMER_13_BIT,
  };

  ServoConfig servo_config_2 = {
    .gpio_num = 27, // Defina o GPIO que você está usando
    .mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_1,
    .timer = LEDC_TIMER_0,
    .frequency = SERVO_PWM_FREQUENCY,
    .duty_resolution = LEDC_TIMER_13_BIT,
  };

  if (servo_init(&servo_config) != ESP_OK) {
    printf("Falha ao inicializar o servo 1.\n");
    return;
  }

  if (servo_init(&servo_config_2) != ESP_OK) {
    printf("Falha ao inicializar o servo 2.\n");
    return;
  }

  int angle;

  while (1) {
    // Movimenta o servo 1 para 90 graus
    if (servo_set_angle(&servo_config, 90) != ESP_OK) {
      printf("Falha ao definir o ângulo do servo 1.\n");
    }

    // Movimenta o servo 2 para 45 graus
    if (servo_set_angle(&servo_config_2, 180) != ESP_OK) {
      printf("Falha ao definir o ângulo do servo 2.\n");
    }

    vTaskDelay(pdMS_TO_TICKS(2000));

    // Movimenta o servo 1 para 0 graus
    if (servo_set_angle(&servo_config, 0) != ESP_OK) {
      printf("Falha ao definir o ângulo do servo 1.\n");
    }

    // Movimenta o servo 2 para 179 graus
    if (servo_set_angle(&servo_config_2, 0) != ESP_OK) {
      printf("Falha ao definir o ângulo do servo 2.\n");
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }

  // Limpeza
  if (hw_servo_deinit(servo_config.gpio_num) != ESP_OK) {
    printf("Falha ao desativar o servo 1.\n");
  }

  if (hw_servo_deinit(servo_config_2.gpio_num) != ESP_OK) {
    printf("Falha ao desativar o servo 2.\n");
  }
}
