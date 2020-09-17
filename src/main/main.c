#include <stdio.h>
#include <time.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_camera.h>
#include <esp_log.h>
#include <esp_sleep.h>

#include "camera.h"
#include "sdcard.h"

#define TAG "lapse_main"
#define TEN_SEC_IN_USEC (10 * 1000 * 1000)

static esp_err_t save_photo()
{
    esp_err_t err;
    camera_fb_t *fb;
    time_t now;
    size_t size;
    struct tm timeinfo;
    FILE *file = NULL;

    /* ISO 8601 2020-09-17T04:36:42+00:00 */
    char filename[] = "/sdcard/YYYY-MM-DDTHH:MM:SS+00:00.jpg";

    err = ESP_FAIL;
    localtime_r(&now, &timeinfo);
    size = strftime(filename, sizeof(filename), "/sdcard/%Y-%m-%dT%H:%M:%S%z.jpg", &timeinfo);
    if (size == 0) {
        ESP_LOGE(TAG, "strftime(): failed");
        goto fail;
    }
    ESP_LOGI(TAG, "Writing to file name: %s", filename);

    fb = get_picture();
    if (fb == NULL) {
        ESP_LOGE(TAG, "get_picture(): failed");
        goto fail;
    }

    if ((file = fopen(filename, "w")) == NULL) {
        ESP_LOGE(TAG, "fopen()");
        goto fail;
    }

    if ((size = fwrite(&fb->buf, fb->len, 1, file)) != fb->len) {
        ESP_LOGE(TAG, "fwrite(): expected: %d written: %d", fb->len, size);
        goto fail;
    }
    err = ESP_OK;
fail:
    if (file != NULL && fclose(file) != 0) {
        ESP_LOGE(TAG, "fclose()");
        err = ESP_FAIL;
    }
    return err;
}

void app_main()
{
    esp_err_t err;
    sdmmc_card_t *card;

    ESP_LOGI(TAG, "Starting camera");
    if ((err = camera_start()) != ESP_OK) {
        ESP_LOGE(TAG, "camera_start(): %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "Starting SD card");
    if ((card = sdcard_start()) == NULL) {
        ESP_LOGE(TAG, "sdcard_start(): failed to mount SD card");
    }

    ESP_LOGI(TAG, "Taking a picture");
    if (save_photo() != ESP_OK) {
        ESP_LOGE(TAG, "save_photo()");
    }

    ESP_LOGI(TAG, "Stopping SD card");
    if ((err = sdcard_stop(card)) != ESP_OK) {
        ESP_LOGE(TAG, "sdcard_stop(): %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "Sleeping");
    esp_sleep_enable_timer_wakeup(TEN_SEC_IN_USEC);
    esp_deep_sleep_start();

    vTaskDelay(100 / portTICK_PERIOD_MS);
}
