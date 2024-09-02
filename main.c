#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_random.h"

// Macros para legibilidade
#define PORTA_GPIO 13
#define SERVO_PWM_FREQUENCIA 50 // Frequência PWM em Hz
#define SERVO_PWM_RESOLUCAO LEDC_TIMER_13_BIT // Resolução PWM 
#define SERVO_LARGURA_MIN_PULSO_US 500 // Largura minima de pulso em microsegundos
#define SERVO_LARGURA_MAX_PULSO_US 2500 // Largura maxima de pulso em microsegundos

static const char *TAG = "servo";


void app_main(void) {
  ServoConfig servo_config = {
    .gpio_num = PORTA_GPIO,
    .mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer = LEDC_TIMER_0,
    .frequency = SERVO_PWM_FREQUENCIA,
    .duty_resolution = SERVO_PWM_RESOLUCAO,
  };

  if (servo_init(&servo_config) != ESP_OK) {
    printf("Falha ao inicializar o servo.\n");
    return;
  }

  int angle;

  while (1) {
    // Gera um número aleatório entre 0 e 180
    int random_angle = esp_random() % 181;
    ESP_LOGI(TAG, "Setting servo angle to %d degrees", random_angle);

    // Define o ângulo do servo
    if (servo_set_angle(&servo_config, random_angle) != ESP_OK) {
      printf("Falha ao definir o ângulo do servo.\n");
    }

    if (servo_get_angle(&servo_config, &angle) == ESP_OK) {
      printf("O ângulo atual do servo é: %d\n", angle);
    } else {
      printf("Falha ao obter o ângulo do servo.\n");
    }

    // Aguarda 2 segundos antes de definir um novo ângulo
    vTaskDelay(pdMS_TO_TICKS(2000));
  }

  // Limpeza
  if (hw_servo_deinit(servo_config.gpio_num) != ESP_OK) {
    printf("Falha ao desativar o servo.\n");
  }

}