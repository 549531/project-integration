#include <Arduino.h>
#include <TFT_eSPI.h>

#include "heart.h"

TFT_eSPI tft;

static char const *TAG = "main";

#define coro_labl_(l) co_##l
#define coro_labl(l) coro_labl_(l)

#define co_init                                   \
	{                                         \
		if (m_position) goto *m_position; \
	}

#define co_fini                                                           \
	{                                                                 \
		coro_labl(__LINE__) : m_position = &&coro_labl(__LINE__); \
		return;                                                   \
	}

#define co_yield                                    \
	{                                           \
		m_position = &&coro_labl(__LINE__); \
		return;                             \
		coro_labl(__LINE__) : {}            \
	}

#define co_delay(d)                     \
	{                               \
		m_timer = millis() + d; \
		co_yield;               \
	}

class Coro {
       protected:
	void *m_position = nullptr;

       public:
	size_t m_timer = 0;
	virtual void loop() = 0;
};

class Anim : public Coro {
	size_t m_period, m_frame = 0;

       public:
	Anim(size_t period = 50) : m_period{period} {}

	void loop() override {
		co_init;
		m_frame = 0;
		for (;;) {
			tft.pushImage(
			    (TFT_WIDTH - heart_width) / 2,
			    (TFT_HEIGHT - heart_height) / 2, heart_width,
			    heart_height,
			    &heart[m_frame * heart_width * heart_height]);
			m_frame = (m_frame + 1) % heart_frames;

			co_delay(m_period);
		}
		co_fini;
	}
};

struct TickTock : public Coro {
       public:
	void loop() override {
		co_init;
		for (;;) {
			ESP_LOGI(TAG, "tick");
			co_delay(500);
			ESP_LOGI(TAG, "tock");
			co_delay(500);
		}
		co_fini;
	}
};

extern "C" void app_main() {
	ESP_LOGI(TAG, "Hello world");

	tft.init();

	Anim a;
	TickTock t;

	Coro *coros[]{&a, &t};

	Coro *timer_coro = nullptr;
	for (;;) {
		size_t now = millis();

		for (auto c : coros) {
			if (c->m_timer <= now) c->loop();
			if (c->m_timer &&
			    (!timer_coro || c->m_timer < timer_coro->m_timer)) {
				timer_coro = c;
			}
		}

		if (timer_coro) {
			if (timer_coro->m_timer > now) {
				delay(timer_coro->m_timer - now);
			}
			timer_coro->m_timer = 0;
			timer_coro = nullptr;
		}
	}
}
