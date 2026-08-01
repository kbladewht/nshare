
#include <stdint.h>
#include <stdbool.h>

#include "config.h"

#include "board_support.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "nordic_common.h"
#include "nrf_delay.h"
#include "app_timer.h"
#include "app_scheduler.h"
#include "nrfx_timer.h"
#include "app_scheduler.h"
#include "nrf_log.h"
#include "protocol_mode.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_drv_clock.h"

#include "nrfx_power.h"
#include "report.h"
#include "output_select.h"

#include "app_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd_hid_generic.h"
#include "app_usbd_hid_kbd.h"
#include "nrf_drv_gpiote.h"
#include "gpio.h"
#include "usb_device_state.h"
#include "qf.h"


volatile bool usb_enabled_qf;
volatile bool vbus_enabled_qf;
volatile bool auto_switch_qf = false;

extern bool reset_from_switch;
void init_auto_switch(void){
    //根据复位来源,决定是否自动切换,如果是按键切换的模式,那么不自动切换,保持原来的模式,比如ESB
    NRF_LOG_INFO("reset_from_switch *************** is  %d", reset_from_switch);
    if(reset_from_switch){
            auto_switch_qf = false;
    }else{
            //如果是正常插入usb 的事件,那么自动切换到usb
            auto_switch_qf = true;
    }

    reset_from_switch = false;
}

//usb 事件捕获
void usbd_user_ev_handler_qf(app_usbd_event_type_t event) {
    switch (event) {
        case APP_USBD_EVT_DRV_SOF:
            break;
        case APP_USBD_EVT_DRV_RESET:
            usb_device_state_set_reset();
            break;
        case APP_USBD_EVT_DRV_SUSPEND:
            /* Allow the library to put the peripheral into sleep mode */
            app_usbd_suspend_req();

            usb_device_state_set_suspend(APP_USBD_STATE_Configured == app_usbd_core_state_get(), 1);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            /* Restore LED state - during SUSPEND all LEDS are turned off */
            usb_device_state_set_resume(APP_USBD_STATE_Configured == app_usbd_core_state_get(), 1);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            NRF_LOG_INFO("USB power detected");
            if (!nrf_drv_usbd_is_enabled()) {
                app_usbd_enable();
            }
            usb_device_state_init();
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            NRF_LOG_INFO("USB power removed");
            app_usbd_stop();
            usb_enabled_qf = false;
            vbus_enabled_qf = false;
            NVIC_SystemReset();
            break;
        case APP_USBD_EVT_POWER_READY:
            NRF_LOG_INFO("USB ready");
            app_usbd_start();
           // usb_enabled_qf = true;
           vbus_enabled_qf = true;
            init_auto_switch();
            //NRF_LOG_INFO("Switch to channe USB . modde is 0 ");
            break;
        case APP_USBD_EVT_STATE_CHANGED:
            if (app_usbd_core_state_get() == APP_USBD_STATE_Configured) {
                usb_device_state_set_configuration(true, 1);
            }

            break;
        default:
            break;
    }
}


