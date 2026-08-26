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



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer0 基础数字层
    [0] = {
        { LT(1,KC_NUM), KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS }, // 第一行：数字锁定/长按切Layer1、数字除号、数字乘号、数字减号
        { KC_P7 , KC_P8      , KC_P9         , KC_KP_PLUS  },       // 第二行：7、8、9、数字加号
        { KC_P4 , KC_P5      , KC_P6         , KC_NO       },       // 第三行：4、5、6
        { KC_P1 , KC_P2      , KC_P3         , KC_KP_ENTER },       // 第四行：1、2、3、数字减号
        { KC_P0 , KC_NO      , KC_KP_DOT     , KC_NO       }        // 第五行：数字加号、0、小数点、数字回车
    },
    // Layer1 灯光/蓝牙切换层
    [1] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, RM_TOGG },    // 第一行：透传、透传、透传、RGB灯光开关
        { KC_TRNS, RM_SPDD, RM_SPDU, RADIO_ESB },  // 第二行：透传、灯速减速、灯速加速、2.4G射频切换
        { BL_SW_1, BL_SW_2, BL_SW_3, KC_TRNS },    // 第三行：蓝牙通道1、蓝牙通道2、蓝牙通道3、透传
        { RM_PREV, RM_NEXT, KC_TRNS, KC_TRNS },    // 第四行：上一灯光、下一灯光、透传、透传
        { RM_HUED, RM_HUEU, KC_TRNS, KC_TRNS }     // 第五行：色相减少、色相增加、透传、透传
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

#ifdef RGB_MATRIX_ENABLE
// clang-format off
led_config_t g_led_config = { 
    {
        { 3,        2,        1,        0 }, 
        { 4,        5,        6,        7 }, 
        { 10,       9,        8,    NO_LED}, 
        { 11,      12,       13,       14 }, 
        { 16,  NO_LED,       15,    NO_LED}
    },{
        { 57, 0 }, { 38, 0 }, { 19, 0 }, { 0,  0 },  // LED 0~3  row0
        {  0,19 }, { 19,19 }, { 38,19 }, { 57,28 }, // LED 4~7  row1
        { 38,38 }, { 19,38 }, {  0,38 },            // LED 8~10 row2
        {  0,57 }, { 19,57 }, { 38,57 }, { 57,67 }, // LED11~14 row3
        { 76,57 }, {  9,57 }                        // LED15~16 row3
    }, {
        4, 4, 4, 4,
        4, 4, 4, 4,
        4, 4, 4, 
        4, 4, 4, 4,
        4, 4
    } 
};
// clang-format on
#endif
 void keyboard_post_init_user(void) {
//初始化磁吸detect pin


 }