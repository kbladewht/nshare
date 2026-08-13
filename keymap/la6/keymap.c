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
        { KC_ESC,  KC_Q,    KC_W,    KC_E,   KC_R,          KC_T,  KC_Y,   KC_U,  KC_I,       KC_O,     KC_P  ,  KC_BSPC  },
        { KC_TAB,  KC_A,    KC_S,    KC_D,   KC_F,          KC_G,  KC_H,   KC_J,  KC_K,       KC_L,     KC_QUOT, KC_ENTER},
        { KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,          KC_B,  KC_N,   KC_M,  KC_COMM,    KC_DOT,   KC_UP,   KC_RSFT },
        { KC_LCTL, KC_CAPS, KC_LALT, KC_SPC, MO(1),         MO(2), KC_NO,  KC_SPC,KC_NO,      KC_LEFT,  KC_DOWN, KC_RGHT}
    }   
	, [1] = {
        { RESET_KB,KC_Q,    KC_W,    KC_E,   KC_R,          KC_T,  KC_Y,   KC_U,  KC_I,       KC_O,     KC_P  ,  KC_BSPC  },
        { KC_TAB,  KC_A,    RGB_MOD, RGB_TOG, KC_F,         KC_G,  KC_H,   KC_J,  KC_K,       KC_L,     KC_QUOT, KC_ENTER},
        { KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,          KC_B,  KC_N,   KC_M,  KC_COMM,    KC_DOT,   KC_UP,   KC_RSFT },
        { KC_LCTL, KC_CAPS, KC_LALT, KC_SPC, MO(1),         MO(2), KC_NO,  KC_SPC,KC_NO,      KC_LEFT,  KC_DOWN, KC_RGHT}
    } 
	, [2] = {
        { RESET_KB,BL_SW_1, BL_SW_2, BL_SW_3,  RADIO_ESB,     KC_T,  KC_Y,   KC_U,  KC_I,       KC_O,     KC_P  ,  KC_BSPC  },
        { KC_TAB,  KC_A,    KC_NO,   DEL_BONDS,KC_F,          KC_G,  KC_H,   KC_J,  KC_K,       KC_L,     KC_QUOT, KC_ENTER},
        { KC_LSFT, KC_Z,    KC_X,    KC_C,     KC_V,          KC_B,  KC_N,   KC_M,  KC_COMM,    KC_DOT,   KC_UP,   KC_RSFT },
        { KC_LCTL, KC_CAPS, KC_LALT, KC_SPC,   KC_NO,         KC_NO, KC_NO,  KC_SPC,KC_NO,      KC_LEFT,  KC_DOWN, KC_RGHT}
    } 
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


led_config_t g_led_config = { {
    // 第0行: 从左到右 (偶数行)
    { 0,      1,      2,      3,      4,      5,      6,      7,      8,      9,      10,     11 },
    // 第1行: 从右到左 (奇数行) — 注意编号顺序反转
    { 23,     22,     21,     20,     19,     18,     17,     16,     15,     14,     13,     12 },
    // 第2行: 从左到右 (偶数行)
    { 24,     25,     26,     27,     28,     29,     30,     31,     32,     33,     34,     35 },
    // 第3行: 从右到左 (奇数行)，跳过两个KC_NO
    { 45,     44,     43,     42,     41,     40,     NO_LED, 39,     NO_LED, 38,     37,     36 }
}, {
    // LED物理坐标 (x, y) — 共46个灯
    // 第0行 (灯0-11) 从左到右
    { 10, 10 },  { 30, 10 },  { 50, 10 },  { 72, 10 },  { 94, 10 },  { 116, 10 },
    { 138, 10 }, { 158, 10 }, { 178, 10 }, { 198, 10 }, { 216, 10 }, { 234, 10 },
    
    // 第1行 (灯12-23) 物理上从左到右，但编号从右到左
    // 物理最左边键 → 灯23，物理最右边键 → 灯12
    { 246, 30 },  // 灯12 (物理最右)
    { 226, 30 },  // 灯13
    { 204, 30 },  // 灯14
    { 182, 30 },  // 灯15
    { 156, 30 },  // 灯16
    { 134, 30 },  // 灯17
    { 112, 30 },  // 灯18
    { 102, 30 },  // 灯19
    { 82, 30 },   // 灯20
    { 62, 30 },   // 灯21
    { 42, 30 },   // 灯22
    { 22, 30 },   // 灯23 (物理最左)
    
    // 第2行 (灯24-35) 从左到右
    { 10, 54 },  { 28, 54 },  { 64, 54 },  { 86, 54 },  { 108, 54 }, { 132, 54 },
    { 154, 54 }, { 174, 54 }, { 194, 54 }, { 212, 54 }, { 230, 54 }, { 244, 54 },
    
    // 第3行 (灯36-45) 物理上从左到右，编号从右到左
    // 物理最左边键 → 灯45，物理最右边键 → 灯36
    { 254, 68 },  // 灯36 (物理最右)
    { 232, 66 },  // 灯37
    { 210, 64 },  // 灯38
                   // [8] NO_LED 跳过
    { 176, 62 },  // 灯39
                   // [6] NO_LED 跳过
    { 146, 61 },  // 灯40
    { 124, 63 },  // 灯41
    { 100, 67 },  // 灯42
    { 78, 72 },   // 灯43
    { 54, 77 },   // 灯44
    { 28, 80 }    // 灯45 (物理最左)
}, {
    // LED flags: 全部0xFF
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
} };


extern void rtc_config(void);
void keyboard_post_init_user(void) {
//默认拉低中断接口,并且配置为输入,上升沿唤醒,回头休眠的代码加入唤醒

#ifdef CX_WK
    nrf_gpio_cfg_input(CX_WK, NRF_GPIO_PIN_PULLDOWN);
    //  extern void uart_init_fly(void);
    // uart_init_fly(); //动态初始化uart,如果还没有初始话的话
      NRF_LOG_INFO("CX_HOST init wk pin %d\n",CX_WK);
   #endif   
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
