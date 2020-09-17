#include <esp_err.h>
#include <esp_log.h>
#include <esp_camera.h>

#include "camera.h"

#define TAG "lapse_camera"

static esp_err_t camera_set_default()
{
    esp_err_t err;
    sensor_t *sensor;

    err = ESP_FAIL;
    sensor = esp_camera_sensor_get();
    if (sensor == NULL) {
        ESP_LOGE(TAG, "esp_camera_sensor_get()");
        goto fail;
    }
    sensor->set_framesize(sensor, FRAMESIZE_CIF);
    sensor->set_quality(sensor, 14);
    sensor->set_brightness(sensor, 0);
    sensor->set_contrast(sensor, 0);
    sensor->set_saturation(sensor, 0);
    sensor->set_sharpness(sensor, 0);
    sensor->set_denoise(sensor, 1);
    sensor->set_special_effect(sensor, 0);
    sensor->set_wb_mode(sensor, 0);
    sensor->set_whitebal(sensor, 1);
    sensor->set_awb_gain(sensor, 1);
    sensor->set_exposure_ctrl(sensor, 1);
    sensor->set_aec2(sensor, 1);
    sensor->set_ae_level(sensor, 1);
    sensor->set_aec_value(sensor, 600);
    sensor->set_gain_ctrl(sensor, 1);
    sensor->set_agc_gain(sensor, 15);
    sensor->set_gainceiling(sensor, GAINCEILING_16X);
    sensor->set_bpc(sensor, 1);
    sensor->set_wpc(sensor, 1);
    sensor->set_raw_gma(sensor, 0);
    sensor->set_lenc(sensor, 1);
    sensor->set_hmirror(sensor, 0);
    sensor->set_vflip(sensor, 0);
    sensor->set_dcw(sensor, 0);
    sensor->set_colorbar(sensor, 0);
    err = ESP_OK;
fail:
    return err;
}

static esp_err_t init()
{
    esp_err_t err;
    camera_config_t config;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;
    config.pin_d0       = Y2_GPIO_NUM;
    config.pin_d1       = Y3_GPIO_NUM;
    config.pin_d2       = Y4_GPIO_NUM;
    config.pin_d3       = Y5_GPIO_NUM;
    config.pin_d4       = Y6_GPIO_NUM;
    config.pin_d5       = Y7_GPIO_NUM;
    config.pin_d6       = Y8_GPIO_NUM;
    config.pin_d7       = Y9_GPIO_NUM;
    config.pin_xclk     = XCLK_GPIO_NUM;
    config.pin_pclk     = PCLK_GPIO_NUM;
    config.pin_vsync    = VSYNC_GPIO_NUM;
    config.pin_href     = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn     = PWDN_GPIO_NUM;
    config.pin_reset    = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size   = FRAMESIZE_SVGA;
    config.jpeg_quality = 16;
    config.fb_count     = 2;

    if ((err = esp_camera_init(&config)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_camera_init(): %s", esp_err_to_name(err));
        goto fail;
    }
    if ((err = camera_set_default()) != ESP_OK) {
        ESP_LOGE(TAG, "camera_set_default(): %s", esp_err_to_name(err));
        goto fail;
    }
fail:
    return err;
}

esp_err_t camera_start()
{
    esp_err_t err;

    err = init();
    return err;
}

camera_fb_t *get_picture()
{
    camera_fb_t *fb;

    if ((fb = esp_camera_fb_get()) == NULL) {
        ESP_LOGE(TAG, "esp_camera_fb_get()");
        goto fail;
    }
fail:
    return fb;
}
