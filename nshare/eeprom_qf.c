#include <stdint.h>
#include QMK_KEYBOARD_H
#include "nrf_log.h"

#include "nrf_gzp.h"
#include "nrf_ecb.h"
#include "nrf_gzll_error.h"
#include "config.h"
#include "n40.h"
// #include "pl_gzll_dc.h"
#include "nrf_queue.h"
#include "app_timer.h"
#include "protocol_mode.h"
#include "app_scheduler.h"
#include "power_manage_qf.h"
#include "qf.h"

// #define EECONFIG_KB_DATA_SIZE 10
// #define EEPROM_SIZE 4096 
// #define EECONFIG_BASE_SIZE 37
// #define EECONFIG_DFU_FLAG  (uint8_t *)37
// #define EECONFIG_SEND_MODE  (uint8_t *)38
// #define EECONFIG_MODE0_PEER  (uint8_t *)39
// #define EECONFIG_MODE1_PEER  (uint8_t *)40
// #define EECONFIG_MODE2_PEER  (uint8_t *)41

// #define EECONFIG_CLI_MODE (uint8_t *)42
// #define EECONFIG_RGB_MATRIX_STATUS2 (uint8_t *)43
// #define EECONFIG_QF_RESET_CODE (uint8_t *)44

//ble_channel_manage.c
/*
eeprom_read_byte(EECONFIG_MODE0_PEER);
*/
uint32_t read_channel_peer_qf(uint8_t channel);
void write_channel_peer_qf(uint8_t channel, uint32_t peer);
uint8_t get_current_protocol_mode(void);

uint8_t read_mode(void) {
#pragma GCC diagnostic ignored "-Wtype-limits"
    uint8_t mode = eeprom_read_byte(EECONFIG_SEND_MODE);
    //NRF_LOG_INFO("read_mode mode is %d", mode);
    if (mode >= 0 && mode <= 8) {
        return mode;
    } else {
        return 0;
    }
}

uint8_t eeconfig_read_dfu_flag(void) {
    return eeprom_read_byte(EECONFIG_DFU_FLAG);
}

void eeconfig_update_dfu_flag(uint8_t val) {
    eeprom_update_byte(EECONFIG_DFU_FLAG, val);
}

uint8_t eeconfig_read_send_mode(void) {
    return eeprom_read_byte(EECONFIG_SEND_MODE);
}

void eeconfig_update_send_mode(uint8_t val) {
    eeprom_update_byte(EECONFIG_SEND_MODE, val);
}

void eeconfig_write_qf(uint8_t *addr, uint8_t val) {
    eeprom_update_byte(addr, val);
}

uint8_t eeconfig_read_qf(uint8_t *addr) {
    return eeprom_read_byte(addr);
}

void init_all_channel(void) {
    write_channel_peer_qf(0, 99);
    write_channel_peer_qf(1, 99);
    write_channel_peer_qf(2, 99);
}


void reset_kb_function(void) {
    xprintf("testing RESET_KB long press being triggered ****\n");
    eeconfig_disable();
    //soft_reset_keyboard();
    QF_RESET;
}