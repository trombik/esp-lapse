#if !defined(__SDCARD__H__)

#include <esp_err.h>
#include <driver/sdmmc_host.h>

/*
 * @brief Initialize SD card, and mount it
 * #return sdmmc_card_t, a ointer to card information, or NULL on error.
 */

sdmmc_card_t *sdcard_start();

/*
 * @brief Unmount SD card.
 * #return ESP_OK or ESP_FAIL on error
 */

esp_err_t sdcard_stop(sdmmc_card_t *card);

#endif
