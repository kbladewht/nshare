#include <stdbool.h>

#include <stdint.h>
#include <string.h>
// fixme should write to register mode
// need refraction

typedef enum
{
    red_qf_color=0,
    green_qf_color=1,
    blue_qf_color=2
} color_enum;
// fixme should write to register mode
// need refraction


void qf_low_power_animation(void);

// task method
bool qf_custom_animation_task(void);

void clear_old_effect(void);
// test only
void qf_battery_indication(uint8_t percentage);

void custom_animation_task(void);

void blink_animation(uint8_t idx,color_enum enum_color);
void blink_qf_animation_color_init(uint8_t light_index, color_enum color_index, uint8_t wait_count);
void blink_qf_stop();
void __battery_indication_func(void);
