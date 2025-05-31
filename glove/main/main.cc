#include <Adafruit_MPU6050.h>
#include <Arduino.h>

#include "fft.hh"
#include "mpu.hh"
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

static char const *TAG = "main";

extern "C" void app_main() {
	initArduino();
	ESP_LOGI(TAG, "boot done");

	mpu_init();
	ESP_LOGI(TAG, "mpu config done");

	network.begin();
	ESP_LOGI(TAG, "network config done");

	fft g_fft;
	ESP_LOGI(TAG, "fft config done");

	for (;;) {
		uint32_t delay = 1000 / fft::FS;
		vTaskDelay(pdMS_TO_TICKS(delay));
		network.loop();
		g_fft.update(&network);
	}
}
