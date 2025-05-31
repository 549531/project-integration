#include "fft.hh"
extern Adafruit_MPU6050 mpu;

static char const *TAG = "fft";

/*----------------------------------------------------*/
/* 1. Static wrapper – never touches member data      */
/*----------------------------------------------------*/
void fft::timer_cb(lv_timer_t *t) {
	auto *self = (fft *)lv_timer_get_user_data(t);
	self->update();
}

/*----------------------------------------------------*/
/* 2. Being called 128 times every second             */
/*----------------------------------------------------*/
void fft::update() {
	sensors_event_t a, g, t;
	mpu.getEvent(&a, &g, &t);

	static float phase = 0.0f;            // keeps running
	phase += TWO_PI * 2.0f / FS;          // Δφ = 2 Hz / 128 Hz
	if (phase > TWO_PI) phase -= TWO_PI;  // wrap once per cycle
	vReal[idx] = sinf(phase);             // deg/s
	vImag[idx] = 0.0f;

	ESP_LOGI(TAG, "> Test sin: %f", vReal[idx]);

	if (fDrive > 0.0f) invert_signal();

	if (++idx >= SAMPLES) {
		compute_fft();
		idx = 0;
	}
}

void fft::invert_signal() {
	phaseAcc += TWO_PI * fDrive / FS;
	if (phaseAcc > TWO_PI) phaseAcc -= TWO_PI;

	float invSample = ampDrive * sinf(phaseAcc + phaseInv);
	ESP_LOGI(TAG, "> Inv: %f", invSample);
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
	phaseInv = phasePeak + PI;                                 // +180°
	ampDrive = 2.0f * sqrtf(peakMag2) / SAMPLES;               // ≃ RMS

	fftEngine.complexToMagnitude();
	char pkt[768];
	size_t len = sprintf(pkt, "> FFT:");
	for (uint8_t k = 0; k < SAMPLES / 2; ++k) {
		float freq = k * FS / SAMPLES;
		len += snprintf(pkt + len, sizeof(pkt) - len, "%u:%.3f%s",
				(unsigned)freq, vReal[k],
				(k < SAMPLES / 2 - 1) ? ";" : "");
	}
	strcat(pkt, "|xy,clr\n");
	ESP_LOGI(TAG, "%s", pkt);
}
