#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include "button.hh"

static char const *TAG = "main";

uint32_t my_lv_draw_buf[TFT_WIDTH * TFT_HEIGHT / 4];

static void btn1_click(void *);
static void btn2_click(void *);

static button_t buttons[]{
    {GPIO_NUM_0, btn1_click},
    {GPIO_NUM_35, btn2_click},
};

static void buttons_init() {
	gpio_config_t buttons_gpio_config{
	    .mode = GPIO_MODE_INPUT,
	    .pull_up_en = GPIO_PULLUP_ENABLE,
	};
	for (auto &btn : buttons) {
		buttons_gpio_config.pin_bit_mask |= 1llu << btn.m_pin;
	}
	ESP_ERROR_CHECK(gpio_config(&buttons_gpio_config));
}

static uint32_t my_lv_tick_get() { return millis(); }

static void chart_update(lv_timer_t *t) {
	auto chart = (lv_obj_t *)lv_timer_get_user_data(t);
	auto series = lv_chart_get_series_next(chart, nullptr);
	lv_chart_set_next_value(chart, series, (int32_t)lv_rand(0, 100));
	lv_chart_refresh(chart);
}

static void buttons_update(lv_timer_t *t) {
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

extern "C" void app_main() {
	ESP_LOGI(TAG, "boot done");

	buttons_init();
	ESP_LOGI(TAG, "buttons config done");

	lv_init();
	lv_tick_set_cb(my_lv_tick_get);

	auto disp = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, my_lv_draw_buf,
				       sizeof my_lv_draw_buf);
	lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

	int32_t col_desc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
	int32_t row_desc[] = {LV_GRID_CONTENT, LV_GRID_FR(1),
			      LV_GRID_TEMPLATE_LAST};

	auto grid = lv_obj_create(lv_screen_active());
	lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
	lv_obj_set_layout(grid, LV_LAYOUT_GRID);
	lv_obj_set_grid_dsc_array(grid, col_desc, row_desc);

	auto labl = lv_label_create(grid);
	lv_obj_set_grid_cell(labl, LV_GRID_ALIGN_CENTER, 0, 1,
			     LV_GRID_ALIGN_CENTER, 0, 1);
	lv_label_set_text(labl, "Hello World");

	auto chart = lv_chart_create(grid);
	lv_chart_set_point_count(chart, 50);
	lv_obj_set_grid_cell(chart, LV_GRID_ALIGN_STRETCH, 0, 1,
			     LV_GRID_ALIGN_STRETCH, 1, 1);

	lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED),
			    LV_CHART_AXIS_PRIMARY_Y);

	lv_timer_create(chart_update, 100, chart);
	lv_timer_create(buttons_update, 33, chart);

	ESP_LOGI(TAG, "lvgl config done");

	for (;;) {
		uint32_t delay = lv_timer_handler();
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}
