#include <Adafruit_MPU6050.h>
#include <Arduino.h>

#include "fft.hh"
#include "network.hh"

const char *ssid = "ACSlab";
const char *pwd = "lab@ACS24";
// const char *mqtt_host = "138.199.207.53";
const char *mqtt_host = "jarkad.net.eu.org";
uint16_t mqtt_port = 8883;
const char *mqtt_user = "pint";
const char *mqtt_pass = "pint";

// global network object
static Network network(ssid, pwd, mqtt_host, mqtt_port, mqtt_user, mqtt_pass);

// MPU 6050
Adafruit_MPU6050 mpu;

// global fft object
static fft g_fft;

static char const *TAG = "main";

void initMPU() {
	if (!mpu.begin()) {
		ESP_LOGE(TAG, "MPU6050 not found");
		abort();
	}
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);     // best LSB resolution
	mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);  // stop >64 Hz aliasing
}

extern "C" void app_main() {
	initArduino();
	ESP_LOGI(TAG, "boot done");

	initMPU();
	ESP_LOGI(TAG, "mpu config done");

	network.begin();
	ESP_LOGI(TAG, "network config done");

	for (;;) {
		uint32_t delay = 1000 / fft::FS;
		vTaskDelay(pdMS_TO_TICKS(delay));
		network.loop();
		g_fft.update(&network);
	}
}
