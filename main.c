#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_random.h"

void app_main(void) {
  ServoConfig servo_config = {
    .gpio_num = 13, // Defina o GPIO que você está usando
    .mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer = LEDC_TIMER_0,
    .frequency = SERVO_PWM_FREQUENCY,
    .duty_resolution = LEDC_TIMER_13_BIT,
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