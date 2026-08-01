
#include <stdint.h>
#include QMK_KEYBOARD_H
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_gzp.h"
#include "nrf_ecb.h"
#include "nrf_gzll_error.h"
#include "config.h"
#include "n40.h"
// #include "pl_gzll_dc.h"
#include "nrf_queue.h"
#include "app_timer.h"
#include "protocol_mode.h"
#include "app_scheduler.h"
#include "power_manage_qf.h"
#include "board_support.h"
#include "qfly_custom_animation.h"

void keyboard_pre_init_kb(void) {
    board_init_qf();
    NRF_LOG_INFO("keyboard_pre_init_kb");
}

void keyboard_post_init_kb(void) {
    NRF_LOG_INFO("keyboard_post_init_kb");
}
extern void battert_timer_start(void);
void main_post_init_qf(void) {
    NRF_LOG_INFO("main_post_init_qf");
    //  battert_timer_start();
    NRF_LOG_INFO("battert_timer_start *******************");
}
// #include "rgb_matrix_types.h"
extern rgb_config_t rgb_matrix_config;
void housekeeping_task_user(void) {
    //如果没有开启rgb ,那么手动控制blinky 效果
    if (!rgb_matrix_config.enable) {
        custom_animation_task();
    }

    // keyboard_tick_handler_qf();
}

void bootmagic_lite(void) {
    /* Do nothing */
}
void bootmagic_scan(void) {
    // We need multiple scans because debouncing can't be turned off.
    // matrix_scan();
    // wait_ms(BOOTMAGIC_DEBOUNCE);
    // matrix_scan();

    // if (bootmagic_should_reset()) {
    //     bootmagic_reset_eeprom();

    //     // Jump to bootloader.
    //     bootloader_jump();
    // }
}