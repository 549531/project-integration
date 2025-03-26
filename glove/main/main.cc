#include <Arduino.h>
#include <TFT_eSPI.h>

#include "heart.h"

TFT_eSPI tft;

static char const *TAG = "main";

extern "C" void app_main() {
	ESP_LOGI(TAG, "Hello world");

	tft.init();

	for (;;) {
		for (int i = 0; i < heart_frames; i++) {
			tft.pushImage((TFT_WIDTH - heart_width) / 2,
				      (TFT_HEIGHT - heart_height) / 2,
				      heart_width, heart_height,
				      &heart[i * heart_width * heart_height]);

			vTaskDelay(pdMS_TO_TICKS(50));
		}

		ESP_LOGI(TAG, "tick");
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}
