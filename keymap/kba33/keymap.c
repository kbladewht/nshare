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

#define LAYOUT_58( \
	K00, K01, K02, K03, K04, K05, K06, K07, \
	K10, K11, K12, K13, K14, K15, K16, K17, \
	K20, K21, K22, K23, K24, K25, K26, K27, \
	K30, K31, K32, K33, K34, K35, K36, K37, \
	K40, K41, K42, K43, K44, K45, K46, K47  \
) { \
	{ K00, K01, K02, K03, K04, K05, K06, K07 }, \
	{ K10, K11, K12, K13, K14, K15, K16, K17 }, \
	{ K20, K21, K22, K23, K24, K25, K26, K27 }, \
	{ K30, K31, K32, K33, K34, K35, K36, K37 }, \
	{ K40, K41, K42, K43, K44, K45, K46, K47 }  \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_58(
        KC_A,  KC_B,  KC_C,  KC_D,  KC_E,  KC_F,  KC_NO,  KC_NO,
        KC_G,  KC_H,  KC_I,  KC_J,  KC_K,  KC_L,  KC_M,  KC_N,
        KC_O,  KC_P,  KC_Q,  KC_R,  KC_S,  KC_T,  KC_U,  KC_V,
        KC_W,  KC_X,  KC_Y,  KC_Z,  KC_1,  KC_2,  KC_3,  KC_4,
        KC_5,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_6,  KC_7
    ),
};

#ifdef RGB_MATRIX_ENABLE

led_config_t g_led_config = {
    {
        {  0,  1,  2,  3,  4,  5, NO_LED, NO_LED },
        { 13, 12, 11, 10,  9,  8,  7,  6 },
        { 14, 15, 16, 17, 18, 19, 20, 21 },
        { 29, 28, 27, 26, 25, 24, 23, 22 },
        { 30, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, 31, 32 }
    },
    {
        {18,4},   {36,4},   {54,4},   {72,4},   {90,4},   {108,4},
        {144,23}, {126,23}, {108,23}, {90,23},  {72,23},  {54,23},  {36,23}, {9,23},
        {4,42},   {36,42},  {54,42},  {72,42},  {90,42},  {108,42}, {126,42}, {144,42},
        {180,61}, {162,61}, {144,61}, {126,61}, {108,61}, {72,61}, {36,61}, {0,61},
        {14,80},  {108,80}, {122,80}
    },
    {
        4,4,4,4,4,4,
        4,4,4,4,4,4,4,4,
        4,4,4,4,4,4,4,4,
        4,4,4,4,4,4,4,4,
        4,4,4
    }
};

bool rgb_matrix_indicators_kb(void) {
    return true;
}

#endif

extern bool process_record_user_cb(uint16_t keycode, keyrecord_t *record);
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return process_record_user_cb(keycode, record);
}

extern void rtc_config(void);
void keyboard_post_init_user(void) {
    void synk_kb_lock(void);
    synk_kb_lock();
    rtc_config();
}

extern bool is_temporary_breathing;
extern uint32_t breathing_timer;
extern void restore_previous_state(void);
void matrix_scan_user_qf(void) {
    if (is_temporary_breathing && timer_expired32(timer_read32(), breathing_timer)) {
        restore_previous_state();
    }
}