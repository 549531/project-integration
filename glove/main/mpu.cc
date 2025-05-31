#include "mpu.hh"

static char const *TAG = "mpu";

Adafruit_MPU6050 mpu;

void mpu_init() {
	if (!mpu.begin()) {
		ESP_LOGE(TAG, "MPU6050 not found");
		abort();
	}
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_250_DEG);     // best LSB resolution
	mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);  // stop >64 Hz aliasing
}

void mpu_get(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t) {
	mpu.getEvent(a, g, t);
}
