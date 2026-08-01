
#include "qfly_custom_animation.h"

#include <stdint.h>
#include "progmem.h"
#include "nrf.h"
#include "rgb_matrix_types.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "rgb_matrix.h"
#include "power_manage_qf.h"

// ========== 全局变量 ==========
static bool original_rgb_was_enabled = false;              // 保存之前的RGB开关状态
static bool original_state_saved = false;  // 新增：标记是否已保存状态
const uint8_t custom_rgblight_effect_breathe_table[] PROGMEM = {
    0x00, // 添加这一行
    0x11,
    0x22,
    0x33,
    0x44,
    0x55,
    0x66,
    0x77,
    0x88,
    0x99,
    0xaa,
    0xbb,
    0xcc,
    0xdd,
    0xee,
    0xff,
    0xff,
    0xff,
    0xff,
    0xee,
    0xdd,
    0xcc,
    0xbb,
    0xaa,
    0x99,
    0x88,
    0x77,
    0x66,
    0x55,
    0x44,
    0x33,
    0x22,
    0x11,
    0x00, // 添加这一行
};

typedef struct
{
    bool playing;
    uint8_t light_index;
    color_enum color_index;
    uint8_t ticks_per_round; //
    uint16_t all_ticks;      // % round count
    uint16_t tick_idx;
    uint16_t tick_wait;
    bool reset_when_done;
} ks_anmation_config;

ks_anmation_config custom_config;
#define RGBLED_NUM 21
// extern rgb_led_t custom_led[];
rgb_led_t custom_led[RGBLED_NUM];

extern void ws2812_setleds(rgb_led_t *ledarray, uint16_t leds);

void clear_old_effect() {
    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        custom_led[i].r = 0;
        custom_led[i].g = 0;
        custom_led[i].b = 0;
    }
    ws2812_setleds(custom_led, RGBLED_NUM);
}

void rgb_power_enable(bool enable);

void blink_qf_animation_color_init(uint8_t light_index, color_enum color_index, uint8_t wait_count) {
    NRF_LOG_INFO("blink_qf_animation_color_init  is done .....%d!!\n");

  // 只在第一次调用时保存状态
    if (!original_state_saved) {
        original_rgb_was_enabled = rgb_matrix_config.enable;
        original_state_saved = true;
        NRF_LOG_INFO("First call - Saving original state: enabled=%d", original_rgb_was_enabled);
    } else {
        NRF_LOG_INFO("Subsequent call - Using saved state: enabled=%d", original_rgb_was_enabled);
    }
    
    // 2. 临时禁用RGB矩阵效果（不保存到EEPROM）
    rgb_matrix_disable_noeeprom();
    rgb_power_enable(true);
    // clear_old_effect();
    // rgblight_timer_disable();
    custom_config.playing = true;
    custom_config.ticks_per_round = sizeof(custom_rgblight_effect_breathe_table);
    custom_config.all_ticks = custom_config.ticks_per_round * 10;
    custom_config.tick_idx = 0;
    custom_config.light_index = light_index;
    custom_config.color_index = color_index;
    custom_config.reset_when_done = false;
    custom_config.tick_wait = wait_count;
}

// 恢复原来的RGB效果
void restore_original_rgb_effect(void) {
    NRF_LOG_INFO("Restoring original RGB: enabled=%d, rgb=%d", original_rgb_was_enabled, rgb_matrix_config.enable);
    
    if (original_rgb_was_enabled || rgb_matrix_config.enable    ) {
        // 如果原来RGB是开启的，恢复原来的模式和效果
        rgb_matrix_enable_noeeprom();  // 对应的启用函数
        rgb_power_enable(true);
    } else {
        // 如果原来RGB是关闭的，关闭RGB
        rgb_power_enable(false);
        clear_old_effect();
    }
}

void blink_qf_stop() {
     NRF_LOG_INFO("blink_qf_stop called");
    custom_config.playing = false;
    custom_config.tick_idx = 0;
    clear_old_effect();
    rgb_power_enable(false);
    restore_original_rgb_effect();
    // 重置状态保存标志
    original_state_saved = false;
}

void __single_blink_animation_func(uint8_t idx);
void __whole_board_breath_func();
void __battery_indication_func();

// extern rgblight_config_t rgblight_config;

extern rgb_config_t rgb_matrix_config;
extern void rgb_power_enable(bool enable);

static uint32_t cacount = 0;

bool rgb_matrix_indicators_user(void){
    if(rgb_matrix_config.enable){
        custom_animation_task();
    }
    return false;
}

void blink_animation_rgb_matrix_on(uint8_t idx, color_enum enum_color);
void custom_animation_task(void) {
    //NRF_LOG_INFO("custom_animation_task running\n");
    if (!custom_config.playing) {
        return;
    }

    cacount++;
    if (cacount > 100000) {
        cacount = 0;
    }
    // NRF_LOG_INFO("custom_config.tick_idx  is %d!!\n",custom_config.tick_idx);
    //  NRF_LOG_INFO("custom_config.all_ticks  is %d!!\n",custom_config.all_ticks);
    //  NRF_LOG_INFO("custom_config.color_index  is %d!!\n",custom_config.color_index);
    if (custom_config.tick_idx >= custom_config.all_ticks) {
        //  NRF_LOG_INFO("11custom_config.tick_idx  is %d!!\n",custom_config.tick_idx);
        //  NRF_LOG_INFO("11custom_config.all_ticks  is %d!!\n",custom_config.all_ticks);
        //  NRF_LOG_INFO("22custom_config.color_index  is %d!!\n",custom_config.color_index);
        if (custom_config.reset_when_done) {
            NRF_LOG_INFO("custom_config.reset_when_done !!\n");
            return;
        }

        cacount = 0;
        
        //clear_old_effect();
        rgb_power_enable(false);
        custom_config.playing = false;
        custom_config.tick_idx = 0;

        return;
    }


    if (cacount % custom_config.tick_wait == 0) {

      if(rgb_matrix_config.enable){
          blink_animation_rgb_matrix_on(custom_config.light_index, custom_config.color_index);
      }else{
          blink_animation(custom_config.light_index, custom_config.color_index);
      }
       
      custom_config.tick_idx++;
        // NRF_LOG_INFO("bb custom_config.tick_idx  is %d!!\n",custom_config.tick_idx);
    }
}

void __single_blink_animation_func(uint8_t idx) {
    custom_led[idx].b = custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round];
    ws2812_setleds(custom_led, RGBLED_NUM);
}

void blink_animation(uint8_t idx, color_enum enum_color) {

    if (enum_color == red_qf_color) {
         custom_led[idx].r = custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round];
    }
    if (enum_color == green_qf_color) {
        custom_led[idx].g = custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round];
    }
    if (enum_color == blue_qf_color) {
       custom_led[idx].b = custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round];
    }

    ws2812_setleds(custom_led, RGBLED_NUM);

}

void blink_animation_rgb_matrix_on(uint8_t idx, color_enum enum_color) {

    uint8_t rcurr=  custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round];
    if (enum_color == red_qf_color) {
        rgb_matrix_set_color(idx, rcurr, 0, 0);
        // custom_led[idx].r = custom_rgblight_effect_breathe_table[custom_config.tick_idx % custom_config.ticks_per_round];
    }
    if (enum_color == green_qf_color) {
        rgb_matrix_set_color(idx, 0,rcurr, 0);
    }
    if (enum_color == blue_qf_color) {
       rgb_matrix_set_color(idx,  0, 0,rcurr);
    }

    // ws2812_setleds(custom_led, RGBLED_NUM);s

}



