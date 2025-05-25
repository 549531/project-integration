#include "buttons.hh"

#include "button.hh"

static void btn1_click(void *);
static void btn2_click(void *);

static button_t buttons[]{
    {GPIO_NUM_0, btn1_click},
    {GPIO_NUM_35, btn2_click},
};

void buttons_init(lv_obj_t *chart) {
	// set up GPIO
	gpio_config_t buttons_gpio_config{
	    .mode = GPIO_MODE_INPUT,
	    .pull_up_en = GPIO_PULLUP_ENABLE,
	};
	for (auto &btn : buttons) {
		buttons_gpio_config.pin_bit_mask |= 1llu << btn.m_pin;
	}
	ESP_ERROR_CHECK(gpio_config(&buttons_gpio_config));

	// register button click handler
	lv_timer_create(buttons_update, 33, chart);
}

void buttons_update(lv_timer_t *t) {
	void *arg = lv_timer_get_user_data(t);
	for (auto &btn : buttons) {
		btn.update(arg);
	}
}

static void btn1_click(void *arg) {
	auto chart = (lv_obj_t *)arg;
	auto series = lv_chart_get_series_next(chart, nullptr);
	lv_chart_set_series_color(chart, series,
				  lv_palette_main(LV_PALETTE_BLUE));
}

static void btn2_click(void *arg) {
	auto chart = (lv_obj_t *)arg;
	auto series = lv_chart_get_series_next(chart, nullptr);
	lv_chart_set_series_color(chart, series,
				  lv_palette_main(LV_PALETTE_GREEN));
}
