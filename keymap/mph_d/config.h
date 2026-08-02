#pragma once


#include "rgb_qf.h"

#define GPIO(port, pin) ((port << 5) | pin) 
/* key matrix size */
#define DIODE_DIRECTION  COL2ROW

#define BLE_SLEEP_OFF_TIMEOUT 1800  // power off time , then shutdown    30min
#define RADIO_SLEEP_OFF_TIMEOUT 40  // power off time , then shutdown    40 * 125ms = 5000ms

#define DEBOUNCE  5

#define EECONFIG_KB_DATA_SIZE 10
#define EEPROM_SIZE 4096 
// #define EECONFIG_BASE_SIZE 37
#define EECONFIG_DFU_FLAG  (uint8_t *)37
#define EECONFIG_SEND_MODE  (uint8_t *)38
#define EECONFIG_MODE0_PEER  (uint8_t *)39
#define EECONFIG_MODE1_PEER  (uint8_t *)40
#define EECONFIG_MODE2_PEER  (uint8_t *)41

#define EECONFIG_CLI_MODE (uint8_t *)42
#define EECONFIG_RGB_MATRIX_STATUS2 (uint8_t *)43


#define DYNAMIC_KEYMAP_LAYER_COUNT 16
#define DEVICE_NAME_0                   "MPH-D BLE 1"                         /**< Name of device. Will be included in the advertising data. */
#define DEVICE_NAME_1                   "MPH-D BLE 2"                         /**< Name of device. Will be included in the advertising data. */
#define DEVICE_NAME_2                   "MPH-D BLE 3"                         /**< Name of device. Will be included in the advertising data. */

// #define VIAL_KEYBOARD_UID {0x6C, 0xD9, 0xA6, 0x5A, 0xE8, 0xA3, 0x7A, 0x8B}
#define VIAL_KEYBOARD_UID { 0xCD, 0x09, 0x03, 0x02, 0x20, 0xF5, 0x28, 0x33 }
#define RAW_HID_ENABLE

#define LOCKING_SUPPORT_ENABLE
#define LOCKING_RESYNC_ENABLE

#define MATRIX_ROWS 4
#define MATRIX_COLS 4

#define THIS_DEVICE_ROWS 4
#define THIS_DEVICE_COLS 4

#define COMBO_TERM 200

#define MATRIX_ROW_PINS { 17, 20, 0, 29}
#define MATRIX_COL_PINS { 15, 31, 30, 4 }
//13+32= 45 p1.13=p1.05 for e73
//C0 to C4 thsi si 840 version , pls note 1.13/1.05  NA/1.11 025/1.10
#define LED_PIN GPIO(0,24)
// #define CHARGED_PIN GPIO(1,0)
// #define CHARGING_PIN CAPSPIN
#define POWER_PIN GPIO(1,9)

#define CAPS_LOCK_IO GPIO(1,2)

#define USE_BATTERY_PIN NRF_SAADC_INPUT_AIN0
#define BATTERY_VMAX 4100
#define TAP_CODE_DELAY 5

#define STATE_LED_ACTIVE 1

#define STATE_LED NRF_GPIO_PIN_MAP(0, 2)
#define STATE_LED_01 NRF_GPIO_PIN_MAP(0, 28)
#define STATE_LED_02 NRF_GPIO_PIN_MAP(0, 3)
#define STATE_LED_03 NRF_GPIO_PIN_MAP(0, 2)

#define LED_BLINK_FAST_INTERVAL APP_TIMER_TICKS(100)
#define LED_BLINK_FAST_INTERVAL_RGB APP_TIMER_TICKS(3000)
#define LED_BLINK_SLOW_INTERVAL APP_TIMER_TICKS(2000)
#define DFU_MAGIC_SKIP                  0x6d

#define RGB_MATRIX_LED_COUNT 16
#define A11 11

#define BATTERY_PIN NRF_SAADC_INPUT_VDDHDIV5 
#define  QF_BATTERY_ENABLE
#define QF_NRF_LOG_ENABLED 1

#define QF_3V3

#define SLEEP_TIME 8 //seconds

#define BLE_SYSTEM_OFF_SEC 1800


#define BLE_BLINK_LED_ENABLE
#define SLEEP_COUNTER_MAX 1
#define QF_RGB_ENABLED
// #define BLE_BLINK_RGB_ENABLE

/*lint -emacro(524, MIN_CONN_INTERVAL) // Loss of precision */
#define MIN_CONN_INTERVAL                   MSEC_TO_UNITS(7.5, UNIT_1_25_MS)           /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL                   MSEC_TO_UNITS(30, UNIT_1_25_MS)            /**< Maximum connection interval (15 ms). */
#define SLAVE_LATENCY                       3                                         /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    MSEC_TO_UNITS(430, UNIT_10_MS)             /**< Connection supervisory timeout (430 ms). */

#define log(fmt, ...) NRF_LOG_INFO(fmt, ##__VA_ARGS__)
// #define NO_ACTION_TAPPING
#define RGB_MATRIX_WS2812
// #define MOUSE_ENABLE
#define QF_TX_POWER 4

#define APP_ADV_FAST_DURATION               3000                                       /**< The advertising duration of fast advertising in units of 10 milliseconds. */
#define APP_ADV_SLOW_DURATION               9000                                   /**< The advertising duration of slow advertising in units of 10 milliseconds. */

#define MUL_QF_SKIP_HEARTBEAT
#define ESB_QF_PIP_0_1
#define ADC_MAX_VALUE 940  // 12-bit ADC 最大值为 950
