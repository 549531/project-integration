#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

static char const *TAG = "main";

struct button_t {
	gpio_num_t m_pin;
	void (*m_click_cb)();

	bool m_value{};
	int64_t m_time{};

	bool m_last_value{};
	int64_t m_last_time{};

	button_t(gpio_num_t pin, void (*click_cb)())
	    : m_pin{pin}, m_click_cb{click_cb} {}

	void update() {
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
				m_click_cb();
			}
		}
	}
};

uint32_t my_lv_draw_buf[TFT_WIDTH * TFT_HEIGHT / 4];

static void btn_click();

static uint32_t my_lv_tick_get() { return millis(); }

static void chart_update(lv_timer_t *t) {
	auto chart = (lv_obj_t *)lv_timer_get_user_data(t);
	auto series = lv_chart_get_series_next(chart, nullptr);
	lv_chart_set_next_value(chart, series, (int32_t)lv_rand(0, 100));
	lv_chart_refresh(chart);
}

static void btn_click() {
	ESP_LOGI(TAG, "button click");
	auto t = lv_timer_get_next(nullptr);
	lv_timer_delete(t);
}

extern "C" void app_main() {
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

	for (;;) {
		ESP_LOGI(TAG, "tick");
		uint32_t delay = lv_timer_handler();
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}
