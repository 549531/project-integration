#include "button.hh"

button_t::button_t(gpio_num_t pin, callback_t click_cb)
    : m_pin{pin}, m_click_cb{click_cb} {}

void button_t::update(void *arg) {
	bool now_value = !gpio_get_level(m_pin);
	int64_t now = esp_timer_get_time();
	if (now_value != m_last_value) {
		m_last_value = now_value;
		m_last_time = now;
	}
	if (m_value != m_last_value && m_last_time - m_time >= 50) {
		m_value = m_last_value;
		m_time = m_last_time;
		if (m_value) {
			assert(m_click_cb);
			m_click_cb(arg);
		}
	}
}
