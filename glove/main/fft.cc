#include "fft.hh"

#include <ESP32Servo.h>

#include "mpu.hh"

Servo servo;

fft::fft() : fftEngine(vReal, vImag, SAMPLES, FS) {
	servo.attach(17, 1000, 2000);
}

/*----------------------------------------------------*/
/* 1. Being called 64 times every second              */
/*----------------------------------------------------*/
void fft::update(Network *net) {
	sensors_event_t a, g, t;
	mpu_get(&a, &g, &t);

	vReal[idx] = g.gyro.x;  // deg/s
	vImag[idx] = 0.0f;

	// send values one by one
	Serial.print(F(">Amplitude:"));
	Serial.println(vReal[idx]);
	net->push(vReal[idx], "devices/12345678/input");

	if (fDrive > 0.0f) invert_signal(net);

	if (++idx >= SAMPLES) {
		compute_fft();
		idx = 0;
	}
}

void fft::invert_signal(Network *net) {
	phaseAcc += TWO_PI * fDrive / FS;
	if (phaseAcc > TWO_PI) phaseAcc -= TWO_PI;

	float invSample = ampDrive * sinf(phaseAcc + phaseInv);
	Serial.print(F(">Inv:"));
	Serial.println(invSample);
	const float range = 2;
	const int digits = 1000;

	auto angle =
	    map(invSample * digits, -range * digits, range * digits, 0, 180);

	servo.write(angle);
	Serial.print(F(">Angle:"));
	Serial.println(angle);
	net->push(invSample, "devices/12345678/inverted");
}

void fft::compute_fft() {
	fftEngine.windowing(FFTWindow::Hamming, FFTDirection::Forward);
	fftEngine.compute(FFTDirection::Forward);

	uint8_t peakIdx = 1;
	float peakMag2 = 0.0f;
	for (uint8_t k = 1; k < SAMPLES / 2; ++k) {
		float mag2 = vReal[k] * vReal[k] + vImag[k] * vImag[k];
		if (mag2 > peakMag2) {
			peakMag2 = mag2;
			peakIdx = k;
		}
	}
	fDrive = peakIdx * FS / SAMPLES;  // Hz

	float phasePeak = atan2f(vImag[peakIdx], vReal[peakIdx]);  // rad
	phaseInv = phasePeak - PI / 2;                             // +180°
	ampDrive = 4.0f * sqrtf(peakMag2) / SAMPLES;               // ≃ RMS
}
