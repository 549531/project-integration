#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

static char const *TAG = "main";

uint32_t my_lv_tick_get() { return millis(); }

uint32_t my_lv_draw_buf[TFT_WIDTH * TFT_HEIGHT / 4];

extern "C" void app_main() {
	lv_init();
	lv_tick_set_cb(my_lv_tick_get);

	lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, my_lv_draw_buf,
			   sizeof my_lv_draw_buf);

	lv_obj_t *labl = lv_label_create(lv_screen_active());
	lv_label_set_text(labl, "Hello World");
	lv_obj_align(labl, LV_ALIGN_CENTER, 0, 0);

	for (;;) {
		ESP_LOGI(TAG, "tick");
		uint32_t delay = lv_timer_handler();
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}
