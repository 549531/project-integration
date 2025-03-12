#include <Arduino.h>

static char const *TAG = "main";

extern "C" void app_main() {
	ESP_LOGI(TAG, "Hello world");

	for (;;) {
		ESP_LOGI(TAG, "tick");
		vTaskDelay(pdMS_TO_TICKS(500));
		ESP_LOGI(TAG, "tock");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
