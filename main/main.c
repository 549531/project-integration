#include <esp_log.h>
#include <freertos/FreeRTOS.h>

static char const *TAG = "main";

void app_main(void) {
	ESP_LOGI(TAG, "Hello world");

	for (;;) {
		ESP_LOGI(TAG, "tick");
		vTaskDelay(pdMS_TO_TICKS(500));
		ESP_LOGI(TAG, "tock");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
