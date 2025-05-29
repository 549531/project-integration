#include <Adafruit_MPU6050.h>
#include <Arduino.h>

#include "buttons.hh"
#include "chart.hh"
#include "fft.hh"

// MPU 6050
Adafruit_MPU6050 mpu;

// global fft object
static fft g_fft;

void initMPU() {
	if (!mpu.begin()) {
		Serial.println("MPU6050 not found");
		while (true) delay(10);
	}
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);     // best LSB resolution
	mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);  // stop >64 Hz aliasing
}

static char const *TAG = "main";

extern "C" void app_main() {
	Serial.begin(921600);
	ESP_LOGI(TAG, "boot done");

	initMPU();
	ESP_LOGI(TAG, "mpu config done");

	lvgl_init();
	ESP_LOGI(TAG, "lvgl config done");

	lv_obj_t *chart = chart_init();
	ESP_LOGI(TAG, "chart config done");

	buttons_init(chart);
	ESP_LOGI(TAG, "buttons config done");

	lv_timer_create(fft::timer_cb, 1000 / fft::FS, &g_fft);

	for (;;) {
		uint32_t delay = lv_timer_handler();
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}
