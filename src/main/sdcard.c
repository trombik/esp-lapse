#include <esp_err.h>
#include <esp_log.h>
#include <esp_vfs_fat.h>
#include <driver/sdmmc_host.h>

#define TAG "lapse_sdcard"
#define NAME_SPACE "/sdcard"

static sdmmc_card_t *init()
{
    esp_err_t err;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 1
    };
    sdmmc_card_t *card = NULL;

    ESP_LOGI(TAG, "Initializing SD card");
    err = esp_vfs_fat_sdmmc_mount(
              NAME_SPACE,
              &host,
              &slot_config,
              &mount_config,
              &card
          );
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_vfs_fat_sdmmc_mount(): %s", esp_err_to_name(err));
        goto fail;
    }
fail:
    return card;
}

sdmmc_card_t *sdcard_start()
{
    return init();
}

esp_err_t sdcard_stop(sdmmc_card_t *card)
{
    return esp_vfs_fat_sdmmc_unmount(NAME_SPACE, card);
}
