#pragma once

#include <Adafruit_MPU6050.h>

void mpu_init();
void mpu_get(sensors_event_t *a, sensors_event_t *g, sensors_event_t *t);
