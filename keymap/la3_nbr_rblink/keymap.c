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

#define LAYOUT_42(                    \
    K00, K01, K02, K03,               \
    K10, K11, K12, K13,               \
    K20, K21, K22, K23,               \
    K30, K31, K32,                    \
    K40, K41, K42, K43,               \
    K50, K52)                         \
    {                                 \
        { K00, K01, K02, K03 },       \
            { K10, K11, K12, K13 },   \
            { K20, K21, K22, K23 },   \
            { K30, K31, K32, KC_NO }, \
            { K40, K41, K42, K43 },   \
        {                             \
            K50, KC_NO, K52, KC_NO    \
        }                             \
    }



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = {
        { KC_ESC, KC_Q,   KC_W,    KC_E,   KC_R,          KC_T,  KC_Y,   KC_U, KC_I,       KC_O,    KC_P    },
        { KC_TAB,  KC_A,    KC_S,    KC_D,   KC_F,          KC_G,  KC_H,   KC_J, KC_K,       KC_L,    KC_ENTER},
        { KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,          KC_B,  KC_N,   KC_M, KC_COMM,    KC_UP,   MO(1)  },
        { KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_NO,         KC_NO, KC_SPC, KC_LEFT,KC_DOWN,  KC_RIGHT,KC_BSPC}
    }   
	, [1] = {
        { RESET_KB,KC_1,    KC_2,    KC_3,   KC_4,          KC_5,  KC_6,   KC_7, KC_8,    KC_9,     KC_0},
        { KC_TAB,  KC_A,    RGB_MOD, RGB_TOG,TOG_QF_UNDER, LED_QF_KEYLIGHT,  KC_H,   KC_J, KC_K,    KB_LOCK,  KC_ENTER},
        { KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,          KC_B,  KC_N,   KC_M, KC_DOT,  KC_PGUP,  KC_NO},
        { KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_NO,         KC_NO, KC_SPC, MO(2),KC_PGDN, KC_INS,KC_MINS}
    } 
	, [2] = {
        { RESET_KB,BL_SW_1, BL_SW_2, BL_SW_3,  RADIO_ESB,      KC_T,  KC_Y,   KC_U, KC_I,     KC_O,     KC_P},
        { KC_TAB,  KC_A,    KC_S,    DEL_BONDS, KC_F,          KC_G,  KC_H,   KC_J, KC_K,     KC_L,     KC_ENTER},
        { KC_LSFT, KC_Z,    KC_X,    KC_C,      KC_V,          KC_B,  KC_N,   KC_M, KC_DOT,   KC_PGUP,  KC_NO},
        { KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,    KC_NO,         KC_NO, KC_SPC, KC_NO,KC_PGDN,  KC_INS,KC_EQL}
    }
};


extern bool process_record_user_cb(uint16_t keycode, keyrecord_t *record);
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
#define XX 255
// clang-format off
led_config_t g_led_config = {
	{
	
        // 第0行：从左到右 (0→10)
        {  0,   1,   2,   3,    4,        5,   6,   7,   8,   9,  10 },
        
        // 第1行：从右到左 (22←21←20←...←11)
        {  22,  21,  20,  19,   18,       17,  16,  15,  14,  13,  12 },
        
        // 第2行：从左到右 (23→33)
        {  23,  24,  25,  26,   27,       28,  29,  30,  31,  32,  33 },
        
        // 第3行：从右到左 (41←40←39←...，中间有NO_LED)
        {  41,  40,  39, NO_LED, NO_LED,  38,  37,  36,  35,  34, 11 }
	}, {

        {9, 4}, {28, 4}, {47, 4}, {65, 4}, {84, 4}, {103, 4}, {121, 4}, {140, 4}, {159, 4}, {177, 4}, {196, 4}, 
		{215, 4},
		{12, 20}, {33, 20}, {51, 20}, {70, 20}, {89, 20}, {107, 20}, {126, 20}, {145, 20}, {163, 20}, {182, 20}, {208, 20}, 
		{16, 36}, {42, 36}, {61, 36}, {79, 36}, {98, 36}, {117, 36}, {135, 36}, {154, 36}, {173, 36}, {191, 36}, {212, 36}, 
		{12, 52}, {33, 52}, {54, 52}, {86, 52}, {133, 52}, {170, 52}, {191, 52}, {212, 52}, 
		{12, 60}, {33, 60},  // 新增两颗左中底灯
		{0, 60},  // 之前添加的左下角底灯
        {0, 4},    // 新增左上角底灯
        // 新增4颗上中底灯
		{84, 0}, {103, 0}, {121, 0}, {140, 0},
        // 右侧新增4颗底灯
		{215, 20},		// 右上侧边灯
		{215, 36},		// 右中侧边灯
		{215, 52},		// 右下侧边灯
		{215, 60},		// 右下角底灯

        // 新增4颗下中底灯
		{84, 60}, {103, 60}, {121, 60}, {140, 60},
	}, {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
		4, 4, 4,       4, 4, 4, 4, 4, 
		2, 2,  // 新增两颗底灯标志
		2,  // 左下角底灯标志
        2,		// 新增左上角底灯标志
        	// 新增4颗上中底灯标志
		2, 2, 2, 2,
        // 右侧新增4颗底灯标志
		2, 2, 2, 2,
        		// 新增4颗下中底灯标志
		2, 2, 2, 2,

	}
};
 bool rgb_matrix_indicators_kb(void) {
     if (host_keyboard_led_state().caps_lock) {  // Capslock = RED
         rgb_matrix_set_color(23, 0, 200, 0);
     }else{
        if(rgb_matrix_get_flags() & LED_FLAG_KEYLIGHT){
           
        }else{
             rgb_matrix_set_color(23, 0, 0, 0);
        }
     }
     return true;
 }
#endif

// // keymap.c
// #ifdef AUTO_SHIFT_NO_AUTO_REPEAT_PER_KEY
// bool get_auto_shift_no_auto_repeat(uint16_t keycode, keyrecord_t *record) {
//     // 对所有键禁用重复（或按需返回 true/false）
//     return true;
// }
// #endif

// #ifdef AUTO_SHIFT_REPEAT_PER_KEY
// bool get_auto_shift_repeat(uint16_t keycode, keyrecord_t *record) {
//     return false; // 示例：禁用重复
// }
// #endif

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
