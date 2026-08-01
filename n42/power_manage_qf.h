
#pragma once
#include "nrf_log.h"


#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_pwr_mgmt.h"

#include "app_timer.h"

#include "rgb_matrix_types.h"
#include "wait.h"
#include "power_manage_qf.h"
#include "nrfx_power.h"
#include "app_usbd.h"
#include "output_select.h"
#include "gpio.h"
#include "nrf_gpio.h"
#include "protocol_mode.h"
#include "nrf_sdh.h"
#include "rgb_matrix.h"
#include "qf.h"

// void keyboard_sleep_counter_reset(void);

/// power mgr provided 2 feature
/// auto sleep detection and battery detection
void power_mgr_init(void);

void power_mgr_start();

void sleep_mode_enter(void);
void sleep_mode_enterFS(void);
void sleep_mode_enter_special(void);
void keyboard_tick_handler_qf(void);

uint32_t getSWADC(void);
void rgb_power_enable(bool enable);
