#include QMK_KEYBOARD_H
#include "nrf_gpio.h"
#include "eeprom.h"
#include "string.h"
#include "eeconfig.h"
#include "action.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "protocol_mode.h"
#include "rgb_matrix.h"
#include "power_manage_qf.h"
#include "qf.h"

extern rgb_config_t rgb_matrix_config;

#define LAYOUT_42(\
	K00, K01, K02, K03, \
	K10, K11, K12, K13, \
	K20, K21, K22, K23, \
	K30, K31, K32,      \
	K40, K41, K42, K43, \
	K50,      K52  \
) { \
	{ K00,   K01,   K02,   K03 }, \
	{ K10,   K11,   K12,   K13 }, \
	{ K20,   K21,   K22,   K23 }, \
	{ K30,   K31,   K32,   KC_NO }, \
	{ K40,   K41,   K42,   K43 }, \
	{ K50,   KC_NO, K52,   KC_NO }  \
}



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = {
    {KC_ESC,    KC_Q,    KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,     KC_BSPC},
    {KC_TAB,    KC_A,    KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_ENTER, KC_NO},
    {KC_LSFT,   KC_Z,    KC_X, KC_C, KC_V, KC_B, KC_B, KC_N, KC_M, KC_COMMA, KC_DOT, TD(1)},
    {KC_LCTL,   KC_LGUI, KC_LALT, LT(13,KC_BSPC), MO(3), KC_NO, KC_NO, KC_NO, KC_SPACE, MO(3), KC_SLASH, MO(2)}
},
    [1] = {
    {KC_ESCAPE, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC},
    {KC_TAB, KC_S, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_ENTER, KC_NO},
    {KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_B, KC_N, KC_M, KC_COMMA, KC_UP, TD(0)},
    {KC_LCTL, KC_LGUI, KC_LALT, LT(13,KC_BSPC), MO(3), KC_NO, KC_NO, KC_NO, KC_SPACE, KC_LEFT, KC_DOWN, KC_RIGHT}
},
    [2] = {
    {KC_ESCAPE, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_BSPC},
    {KC_TRNS, KC_F11, LSFT(KC_F12), KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_TRNS, KC_NO},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(14), MO(14), KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_MUTE},
    {KC_MPLY, KC_TRNS, KC_TRNS, KC_MPRV, KC_MSTP, KC_NO, KC_NO, KC_NO, KC_MNXT, MO(15), KC_VOLD, KC_TRNS}
},
    [3] = {
    {LSFT(KC_GRAVE), LSFT(KC_4), LSFT(KC_SLASH), KC_E, KC_R, KC_T, LSFT(KC_QUOTE), KC_U, KC_I, LSFT(KC_9), LSFT(KC_0), KC_BSPC},
    {KC_TRNS, LSFT(KC_2), LSFT(KC_6), LSFT(KC_4), KC_SCLN, LSFT(KC_1), LSFT(KC_7), LSFT(KC_3), KC_LBRC, KC_RBRC, KC_ENTER, KC_NO},
    {KC_TRNS, KC_Z, LSFT(KC_MINUS), KC_C, KC_V, LSFT(KC_5), LSFT(KC_5), KC_N, LSFT(KC_SCLN), LSFT(KC_COMMA), LSFT(KC_DOT), KC_TRNS},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_SLASH, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_BSLS, KC_TRNS, KC_TRNS, KC_TRNS}
},
    [13] = {
    {KC_TRNS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_O, KC_BSPC},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, KC_KP_PLUS, KC_EQUAL, KC_NO},
    {KC_CAPS, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_TRNS},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, LCA(KC_A), KC_LEFT, KC_DOWN, KC_RIGHT}
},
    [14] = {
    {KC_TRNS, BL_SW_1, BL_SW_2, BL_SW_3, RADIO_ESB , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS},
    {KC_TRNS, BL_SW_1, BL_SW_2, BL_SW_3, RADIO_ESB , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS}
},
    [15] = {
    {RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_TOG},
    {KC_TRNS, RM_VALD, RGB_HUI, RGB_HUD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RM_SPDU, RGB_VAI, KC_TRNS, KC_NO},
    {KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_SAI, RGB_SAD, KC_TRNS, RM_SPDD, KC_TRNS, KC_TRNS, KC_TRNS},
    {KC_TRNS, KC_TRNS, KC_TRNS, RGB_RMOD, KC_TRNS, KC_NO, KC_NO, KC_NO, RGB_MOD, KC_TRNS, KC_TRNS, KC_TRNS}
},
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
		{ 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, NO_LED },
		{ 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34 },
		{ 43, 42, 41, 40, 39, NO_LED, NO_LED, NO_LED, 38, 37, 36, 35 }
	},
{
	{18,4},{36,4},{56,4},{74,4},{94,4},{112,4},{132,4},{150,4},{170,4},{188,4},{208,4},{226,4},
	{22,23},{46,23},{65,23},{84,23},{103,23},{122,23},{141,23},{160,23},{179,23},{198,23},{224,23},
	{27,42},{56,42},{74,42},{94,42},{112,42},{132,42},{150,42},{170,42},{188,42},{208,42},{226,42},{248,42},
	{18,61},{36,61},{56,61},{86,61},{117,61},{153,61},{188,61},{208,61},{226,61}
},
	{
		4,4,4,4,4,4,4,4,4,4,4,4,    // LED  0~11  (Row 0)
		4,4,4,4,4,4,4,4,4,4,4,       // LED 12~22  (Row 1)
		4,4,4,4,4,4,4,4,4,4,4,4,    // LED 23~34  (Row 2)
		4,4,4,4,4,4,4,4,4             // LED 35~43  (Row 3)
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
