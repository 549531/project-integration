#pragma once

#include <Arduino.h>

/// This class polls a button, debounces it, and invokes a callback on each
/// click.
struct button_t {
	using callback_t = void (*)(void *);

	/// GPIO pin connected to the button.
	gpio_num_t m_pin;
	/// Called on every button click.
	callback_t m_click_cb;

	/// Create a button poller.
	/// @param pin      The GPIO pin that's connected to the button.
	/// @param click_cb Called on every click.
	button_t(gpio_num_t pin, callback_t click_cb);

	/// This function must be called periodically to poll the button.
	/// @param arg This parameter will be passed, unchanged, to the
	///            callback.
	void update(void *arg);

	/// Debounced state of the button.
	bool m_value{};
	/// Timestamp, in microseconds, when m_value was last updated.
	int64_t m_time{};

	/// Raw state of the button.
	bool m_last_value{};
	/// Timestamp, in microseconds, when m_last_value was last updated.
	int64_t m_last_time{};
};
