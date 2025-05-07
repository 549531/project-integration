#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

static char const *TAG = "main";

uint32_t my_lv_tick_get() { return millis(); }

uint32_t my_lv_draw_buf[TFT_WIDTH * TFT_HEIGHT / 4];

static void my_chart_update(lv_timer_t *t) {
	auto chart = (lv_obj_t *)lv_timer_get_user_data(t);
	auto series = lv_chart_get_series_next(chart, nullptr);
	lv_chart_set_next_value(chart, series, (int32_t)lv_rand(0, 100));
	lv_chart_refresh(chart);
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

	lv_timer_create(my_chart_update, 100, chart);

	for (;;) {
		ESP_LOGI(TAG, "tick");
		uint32_t delay = lv_timer_handler();
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}
