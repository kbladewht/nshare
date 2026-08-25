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

#define LAYOUT_4X4( \
    K00, K01, K02, K03, \
    K10, K11, K12, K13, \
    K20, K21, K22, K23, \
    K30, K31, K32, K33  \
) { \
    { K00, K01, K02, K03 }, \
    { K10, K11, K12, K13 }, \
    { K20, K21, K22, K23 }, \
    { K30, K31, K32, K33 }  \
}
enum combos {
    COMBO_SW_LAYER1,
    COMBO_TOTAL_COUNT
};
// 同时按下 KC_7 + KC_4 触发
const uint16_t PROGMEM combo_mo1_keys[] = {KC_7, KC_4, COMBO_END};

// combo_t key_combos[COMBO_TOTAL_COUNT] = {
//     [COMBO_SW_LAYER1] = COMBO(combo_mo1_keys, MO(1)), // 7+4组合键，临时切换灯光/蓝牙层Layer1
// };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer0 基础数字层
    [0] = {
        { KC_7, KC_8, KC_9, KC_KP_SLASH },          // 第一行：7、8、9、数字除号
        { KC_4, KC_5, KC_6, KC_KP_ASTERISK },       // 第二行：4、5、6、数字乘号
        { KC_1, KC_2, KC_3, KC_KP_MINUS },          // 第三行：1、2、3、数字减号
        { KC_KP_PLUS, KC_0, KC_KP_DOT, KC_ENTER }   // 第四行：数字加号、0、小数点、数字回车
    },
    // Layer1 灯光/蓝牙切换层
    [1] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, RM_TOGG },    // 第一行：透传、透传、透传、RGB灯光开关
        { KC_TRNS, KC_TRNS, RM_SPDD, RM_SPDU },    // 第二行：透传、透传、灯速减速、灯速加速
        { BL_SW_1, BL_SW_2, BL_SW_3, RADIO_ESB },  // 第三行：蓝牙通道1、蓝牙通道2、蓝牙通道3、2.4G射频切换
        { RM_PREV, RM_NEXT, RM_HUED, RM_HUEU }     // 第四行：上一灯光、下一灯光、色相减少、色相增加
    },
    // Layer2 备用层，全部按键透传
    [2] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },    // 第一行：全部透传
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },    // 第二行：全部透传
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },    // 第三行：全部透传
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }     // 第四行：全部透传
    },
    // Layer3 空层，全透传
    [3] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },
    // Layer4 空层，全透传
    [4] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },
    // Layer5 空层，全透传
    [5] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },
    // Layer6 空层，全透传
    [6] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },
    // Layer7 空层，全透传
    [7] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
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
        { 3,    2,   1,    0 }, 
        { 4,    5,   6,    7 }, 
        { 10,   9,   8}, 
        { 11,  12,  13,   14 }, 
        { 17,  16,  15}
    },{
        {  0, 0 }, { 19, 0 }, { 38, 0 }, { 57, 0 }, // LED 0~3  row0
        { 57,19 }, { 38,19 }, { 19,19 }, {  0,19 }, // LED 4~7  row1
        {  0,38 }, { 19,38 }, { 38,38 }, { 57,38 }, // LED 8~11 row2
        { 57,57 }, { 38,57 }, { 19,57 }, {  0,57 }  // LED12~15 row3
    }, {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
    } 
};
// clang-format on
#endif
 void keyboard_post_init_user(void) {
//初始化磁吸detect pin


 }