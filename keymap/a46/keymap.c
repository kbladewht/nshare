
#include QMK_KEYBOARD_H
#include "nrf_gpio.h"
#include "eeprom.h"
#include "keycodes.h"
#include "string.h"
#include "eeconfig.h"
#include "action.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "protocol_mode.h"
#include "power_manage_qf.h"
#include "qf.h"
#include "rgb_matrix.h"

extern rgb_config_t rgb_matrix_config;

#define LAYOUT_A46( \
    k00, k01, k02, k03, k04, k05,   k06, k07, k08, k09, k0a, k0b\
    k10, k11, k12, k13, k14, k15,   k16, k17, k18, k19, k1a, k1b\
    k20, k21, k22, k23, k24, k25,   k26, k27, k28, k29, k2a, k2b\
    k30, k31, k32, k33, k34,        k36,      k38, k39, k3a, k3b\
) { \
    { k00, k01, k02, k03, k04, k05,   k06, k07,   k08, k09, k0a, k0b }, \
    { k10, k11, k12, k13, k14, k15,   k16, k17,   k18, k19, k1a, k1b }, \
    { k20, k21, k22, k23, k24, k25,   k26, k27,   k28, k29, k2a, k2b }, \
    { k30, k31, k32, k33, k34, KC_NO, k36, KC_NO, k38, k39, k3a, k3b } \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [0] = LAYOUT_A46(
        KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_BSPC,
        KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_ENT,
        KC_LCTL, KC_LALT, KC_LGUI, MO(1),   KC_SPC,            LT(2, KC_SPC),    KC_UP,   KC_LEFT, KC_DOWN,  KC_RGHT
    ),
 [1] = LAYOUT_A46(
        RESET_KB,BL_SW_1, BL_SW_2, BL_SW_3, RADIO_ESB,USB_QF,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, 
        KC_TRNS, RGB_RMOD,RGB_MOD, RGB_TOG, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,   
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,   
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS
    ),
 [2] = LAYOUT_A46(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_BSPC,
        KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,  KC_BSLS,
        KC_TRNS, KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,   KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,          KC_TRNS, KC_HOME, KC_PGDN,  KC_PGUP
    ),
};

extern bool process_record_user_cb(uint16_t keycode, keyrecord_t *record) ;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_CAPS:
            if (record->event.pressed) {
                 NRF_LOG_INFO("caps unlocked\n");
            }
            break;
    }

    return process_record_user_cb(keycode, record);
}

#ifdef RGB_MATRIX_ENABLE
   
// clang-format off
led_config_t g_led_config = {
    {
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11 },
        { 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12 },
        { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 },
        { 45, 44, 43, 42, 41, NO_LED, 40, NO_LED, 39, 38, 37, 36 }
    },
    {
        // Row 0: LED 0~11（从左到右）
        {18,4},{36,4},{56,4},{74,4},{94,4},{112,4},{132,4},{150,4},{170,4},{188,4},{208,4},{226,4},
        // Row 1: LED 12~23（矩阵映射为从右到左递增，故坐标反序）
        {226,23},{208,23},{188,23},{170,23},{150,23},{132,23},{112,23},{94,23},{74,23},{56,23},{36,23},{18,23},
        // Row 2: LED 24~35（从左到右，保留原有 12 个坐标）
        {27,42},{56,42},{74,42},{94,42},{112,42},{132,42},{150,42},{170,42},{188,42},{208,42},{226,42},{248,42},
        // Row 3: LED 36~45（矩阵映射为从右到左递增，且底部双空格，2u 空格 LED 取合并列中心）
        {226,61},{208,61},{188,61},{160,61},{122,61},{94,61},{74,61},{56,61},{36,61},{18,61}
    },
    {
        4,4,4,4,4,4,4,4,4,4,4,4,    // LED  0~11  (Row 0)
        4,4,4,4,4,4,4,4,4,4,4,4,    // LED 12~23  (Row 1)
        4,4,4,4,4,4,4,4,4,4,4,4,    // LED 24~35  (Row 2)
        4,4,4,4,4,  4,  4,4,4,4     // LED 36~45  (Row 3)
    }
};

 bool rgb_matrix_indicators_kb(void) {
     if (host_keyboard_led_state().caps_lock) {  // Capslock = RED
        //  rgb_matrix_set_color(16, 0, 200, 0);
     }
     return true;
 }
#endif

extern void rtc_config(void);
void keyboard_post_init_user(void) {
//默认拉低中断接口,并且配置为输入,上升沿唤醒,回头休眠的代码加入唤醒
    void synk_kb_lock(void);
    synk_kb_lock();
     rtc_config();
}

extern  bool is_temporary_breathing;
extern  uint32_t breathing_timer; 
// ========== 矩阵扫描处理 ==========
extern void restore_previous_state(void);
void matrix_scan_user_qf(void) {
    if (is_temporary_breathing && timer_expired32(timer_read32(), breathing_timer)) {
        restore_previous_state();  // 5秒到，恢复原状
    }
}
