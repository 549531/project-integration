#pragma once

#include <Adafruit_MPU6050.h>
#include <Arduino.h>
#include <arduinoFFT.h>

#include <cstdint>

#include "network.hh"

struct fft {
	// Constants
	static constexpr uint16_t SAMPLES = 32;  // FFT length (power-of-2)
	static constexpr float FS = 64.0f;       // sample rate  [Hz]

	// Object data
	float vReal[SAMPLES];
	float vImag[SAMPLES];

	ArduinoFFT<float> fftEngine;

	// State that used to be local-static
	uint16_t idx = 0;
	float fDrive = 0.0f;    // Hz   – dominant tremor frequency
	float ampDrive = 0.0f;  // same unit as gyro (deg/s)
	float phaseInv = 0.0f;  // rad  – inverted phase (φ+π)
	float phaseAcc = 0.0f;  // rad  – phase accumulator for sine

	// Constructor
	fft();

	// Public API
	void invert_signal(Network* net);
	void compute_fft();
	void update(Network* net);
};

struct TimerCtx {
	Network* net;
	fft* fft_obj;
};
