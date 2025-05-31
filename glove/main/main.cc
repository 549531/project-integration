#include <Adafruit_MPU6050.h>
#include <Arduino.h>

#include "buttons.hh"
#include "chart.hh"
#include "fft.hh"
#include "mpu.hh"

static char const *TAG = "main";

extern "C" void app_main() {
	ESP_LOGI(TAG, "boot done");

	mpu_init();
	ESP_LOGI(TAG, "mpu config done");

	fft g_fft;
	ESP_LOGI(TAG, "fft config done");

	lvgl_init();
	ESP_LOGI(TAG, "lvgl config done");

	lv_obj_t *chart = chart_init();
	ESP_LOGI(TAG, "chart config done");

	buttons_init(chart);
	ESP_LOGI(TAG, "buttons config done");

	for (;;) {
		uint32_t delay = lv_timer_handler();
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}
