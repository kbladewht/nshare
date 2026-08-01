#include QMK_KEYBOARD_H
#include "rgb_matrix.h"
#include "qf.h"

// ========== 使用你的呼吸表 ==========
const uint8_t custom_rgblight_effect_breathe_table[] PROGMEM = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0xff, 0xff, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa,
    0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22,
    0x11, 0x00
};
#define BREATHE_TABLE_SIZE (sizeof(custom_rgblight_effect_breathe_table) / sizeof(custom_rgblight_effect_breathe_table[0]))

// ========== 全局变量 ==========
static uint32_t blink_end_time = 0;              // 闪烁结束时间
static bool is_blinking = false;                  // 闪烁状态标志
static uint8_t saved_led_index = 0;               // 保存的LED索引
static uint8_t saved_color_index = 0;             // 保存的颜色索引
static uint8_t previous_mode = 0;                 // 保存之前的模式
static bool was_rgb_enabled = false;              // 保存之前的RGB开关状态
static uint16_t breathe_table_index = 0;          // 呼吸表索引
static uint32_t animation_timer = 0;              // 动画计时器

// 颜色定义
typedef enum {
    red_qf_color = 0,
    green_qf_color,
    blue_qf_color,
    white_qf_color,
    yellow_qf_color,
    cyan_qf_color,
    magenta_qf_color
} color_enum;

// ========== 从呼吸表获取亮度值 ==========
static uint8_t get_breathe_value(void) {
    if (breathe_table_index >= BREATHE_TABLE_SIZE) {
        breathe_table_index = 0;
    }
    return custom_rgblight_effect_breathe_table[breathe_table_index];
}

// ========== 启动单个LED渐变闪烁 ==========
void start_fade_blink_led(uint8_t led_index, color_enum color, uint8_t duration_sec) {
    if (is_blinking) {
        return;  // 已经在闪烁状态
    }
    
    NRF_LOG_INFO("start_fade_blink_led: LED=%d, color=%d, duration=%ds", 
                 led_index, color, duration_sec);
    
    // 保存当前状态
    was_rgb_enabled = rgb_matrix_is_enabled();
    previous_mode = rgb_matrix_get_mode();
    
    saved_led_index = led_index;
    saved_color_index = color;
    
    // 确保RGB开启
    rgb_power_enable(true);
    rgb_matrix_enable_noeeprom();
    
    // 清除所有LED
    rgb_matrix_set_color_all(0, 0, 0);
    
    // 设置动画参数
    blink_end_time = timer_read32() + (duration_sec * 1000);
    animation_timer = timer_read32();
    breathe_table_index = 0;
    is_blinking = true;
    
    NRF_LOG_INFO("Fade blink started");
}

// ========== 更新渐变动画 ==========
void update_fade_blink_animation(void) {
    if (!is_blinking) return;
    
    // 检查是否超时
    if (timer_read32() > blink_end_time) {
        restore_original_state();
        return;
    }
    
    // 每20ms更新一次 (50FPS)
    if (timer_elapsed32(animation_timer) > 20) {
        animation_timer = timer_read32();
        
        // 获取呼吸表的值 (0-255)
        uint8_t breathe_value = get_breathe_value();
        
        // 清除之前的LED
        rgb_matrix_set_color_all(0, 0, 0);
        
        // 根据颜色设置LED
        switch (saved_color_index) {
            case red_qf_color:
                rgb_matrix_set_color(saved_led_index, breathe_value, 0, 0);
                break;
            case green_qf_color:
                rgb_matrix_set_color(saved_led_index, 0, breathe_value, 0);
                break;
            case blue_qf_color:
                rgb_matrix_set_color(saved_led_index, 0, 0, breathe_value);
                break;
            case white_qf_color:
                rgb_matrix_set_color(saved_led_index, breathe_value, breathe_value, breathe_value);
                break;
            case yellow_qf_color:
                rgb_matrix_set_color(saved_led_index, breathe_value, breathe_value, 0);
                break;
            case cyan_qf_color:
                rgb_matrix_set_color(saved_led_index, 0, breathe_value, breathe_value);
                break;
            case magenta_qf_color:
                rgb_matrix_set_color(saved_led_index, breathe_value, 0, breathe_value);
                break;
            default:
                rgb_matrix_set_color(saved_led_index, breathe_value, 0, 0);  // 默认红色
                break;
        }
        
        // 更新呼吸表索引
        breathe_table_index++;
        if (breathe_table_index >= BREATHE_TABLE_SIZE) {
            breathe_table_index = 0;
        }
    }
}

// ========== 恢复原始状态 ==========
void restore_original_state(void) {
    NRF_LOG_INFO("restore_original_state: was_enabled=%d", was_rgb_enabled);
    
    // 清除LED
    rgb_matrix_set_color_all(0, 0, 0);
    
    if (!was_rgb_enabled) {
        // 如果原来RGB是关闭的，则关闭RGB
        rgb_matrix_disable_noeeprom();
        rgb_power_enable(false);
        NRF_LOG_INFO("RGB turned off");
    } else {
        // 如果原来RGB是开启的，恢复原始模式
        rgb_matrix_mode_noeeprom(previous_mode);
        NRF_LOG_INFO("RGB mode restored to %d", previous_mode);
    }
    
    is_blinking = false;
    breathe_table_index = 0;
}

// ========== 便捷函数 ==========
void trigger_red_fade_blink(uint8_t led_index, uint8_t seconds) {
    start_fade_blink_led(led_index, red_qf_color, seconds);
}

void trigger_green_fade_blink(uint8_t led_index, uint8_t seconds) {
    start_fade_blink_led(led_index, green_qf_color, seconds);
}

void trigger_blue_fade_blink(uint8_t led_index, uint8_t seconds) {
    start_fade_blink_led(led_index, blue_qf_color, seconds);
}

// 更智能的版本，支持多种闪烁模式
void start_advanced_fade_blink(uint8_t led_index, color_enum color, uint8_t duration_sec, 
                                uint8_t speed, bool repeat) {
    if (is_blinking) return;
    
    was_rgb_enabled = rgb_matrix_is_enabled();
    previous_mode = rgb_matrix_get_mode();
    saved_led_index = led_index;
    saved_color_index = color;
    
    rgb_power_enable(true);
    rgb_matrix_enable_noeeprom();
    rgb_matrix_set_color_all(0, 0, 0);
    
    blink_end_time = timer_read32() + (duration_sec * 1000);
    animation_timer = timer_read32();
    breathe_table_index = 0;
    is_blinking = true;
}

// 从你的代码中复用的函数
void clear_all_leds(void) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }
}