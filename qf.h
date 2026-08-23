
#pragma once
#include <stdint.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "report.h"
#include "host.h"
#include "host_driver.h"
#include "keyboard.h"
#include "action.h"
#include "action_util.h"
#include "usb_device_state.h"
#include "mousekey.h"
#include "led.h"
#include "sendchar.h"
#include "debug.h"
#include "print.h"
#include "protocol_mode.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_usbd.h"
#include "app_scheduler.h"
#include "power_manage.h"
#include "matrix.h"
#include "radio.h"

 #include "quantum_keycodes.h"

#define MXKBD_USB_MODE      0U
#define MXKBD_BLE_MODE      1U
#define MXKBD_BLE_CHANNEL_1 1U
#define MXKBD_BLE_CHANNEL_2 2U
#define MXKBD_BLE_CHANNEL_3 3U
#define MXKBD_BLE_CHANNEL_4 4U
#define MXKBD_BLE_CHANNEL_5 5U
#define MXKBD_BLE_CHANNEL_6 6U
#define GZ_MODE     7U
#define ESB_MODE      8U

#define BLE_QF_1 1U
#define BLE_QF_2 2U
#define BLE_QF_3 3U

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#ifndef QF_DEF_VIAL_JSON
enum custom_keycodes {
    KC_BLE_1= QK_KB_0,
    KC_BLE_2, 
    KC_BLE_3,
    KC_24_G,
    KC_USB_CHANNEL,
    DEL_BONDS,
    KB_LOCK,
    RESET_KB,
    ADC_SHOW,
    RADIO_DFU,
    TOG_QF_UNDER,
    LED_QF_KEYLIGHT
};
#endif


#define BL_SW_1     KC_BLE_1
#define BL_SW_2     KC_BLE_2
#define BL_SW_3     KC_BLE_3  // RADIO

#define BLE1     KC_BLE_1
#define BLE2     KC_BLE_2
#define BLE3     KC_BLE_3  // RADIO


#define RADIO_ESB     KC_24_G  // RADIO
#define RADIO     KC_24_G  // RADIO
#define USB_QF     KC_USB_CHANNEL

#define BLE_DEL DEL_BONDS

// #define BLE_SLEEP_OFF_TIMEOUT (2 * 60)

void update_dfu_flag_eeprom(uint8_t flag);
void eeconfig_update_dfu_flag(uint8_t val);
uint8_t read_dfu_flag_eeprom(void);
uint8_t eeconfig_read_dfu_flag(void);
uint8_t current_ram_dfu_flag(void);
void main_post_init_qf(void);
void rgb_power_enable(bool enable);
uint8_t eeconfig_read_qf(uint8_t * addr);
void eeconfig_write_qf(uint8_t * addr, uint8_t val);

uint8_t eeconfig_read_EECONFIG_CLI_MODE(void);
void eeconfig_update_EECONFIG_CLI_MODE(uint8_t val);
void usbd_init_cli(void);

void setup_protocol_mode(void) ;
void power_manage_init(void);
void power_manage_handler_ble_core(void);
void power_manage_handler_radio_core(void);
uint8_t read_mode(void);
uint8_t read_ram_mode(void);
void enter_sleep_off(void);

typedef void (*TypeCallbackshort)(void);
typedef void (*TypeCallbacklong)(void);

void process_hold_keys_cb(bool hold_pressed,TypeCallbackshort short_cb,TypeCallbacklong long_cb);
void poll_hold_keys_scan(void);
void handle_bluetooth_channel(void);
void rebond_reset_long_press(void);
// void switch_device_rebond(uint8_t id);
void switch_device_rebond(void);

