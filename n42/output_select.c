#include "output_select.h"
#include "eeconfig.h"
#include "wait.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_gpio.h"
#include "nrf_gzll.h"
#include <stdint.h>
#include "app_timer.h"
#include "protocol_mode.h"
#include "eeprom.h"
#include "report.h"
#include "app_scheduler.h"
#include "nrf_sdh.h"

static volatile uint8_t usb_ind = 0;

extern volatile bool usb_enabled_qf;

uint8_t curr_mode = 99;

void usb_send_keyboard(report_keyboard_t *report);

/*

#define MXKBD_USB_MODE      0U
#define MXKBD_BLE_MODE      1U
#define MXKBD_BLE_CHANNEL_1 1U
#define MXKBD_BLE_CHANNEL_2 2U
#define MXKBD_BLE_CHANNEL_3 3U
#define MXKBD_BLE_CHANNEL_4 4U
#define MXKBD_BLE_CHANNEL_5 5U
#define MXKBD_BLE_CHANNEL_6 6U
#define MXKBD_GZLL_MODE     7U
#define ESB_MODE      8U

*/
uint8_t is_ble_mode(void) {
    uint8_t currMode = get_current_protocol_mode();

    if (currMode > 0 && currMode <= 8) {
        return 1;
    }

    return 0;
}

uint8_t is_gz_mode(void) {
    uint8_t currMode = get_current_protocol_mode();

    if (currMode == GZ_MODE) {
        return 1;
    }

    return 0;
}

extern uint8_t read_mode(void);

extern volatile bool usb_enabled_qf;
uint8_t read_ram_mode(void) {
    if (curr_mode == 99) {
        curr_mode = read_mode();
    }
    return curr_mode;
}

static void delayed_reset(void *p_event_data, uint16_t event_size) {
    NVIC_SystemReset();
}
#include "nrf_power.h"
extern volatile bool usb_enabled_qf;
void change_send_mode(uint8_t next_mode) {
    // uint8_t mode = read_ram_mode();
    // NRF_LOG_INFO("Change send mode from %s to %s", mode_to_str(mode), mode_to_str(next_mode));
    // if (next_mode == mode) {
    //     /*  */
    //     return;
    // }

    // 把修改后的 mode 存储到 GPREGRET2

    // set_protocol_mode(next_mode);
    //如果不是有线模式,直接更新eeprom
    eeprom_update_byte(EECONFIG_SEND_MODE, next_mode);
    // eeconfig_write_qf(EECONFIG_QF_RESET_CODE, 0xA5); //写入一个标记,表示是通过按键切换的模式

    //nrf_power_gpregret2_set(3);
    NRF_LOG_INFO("change_send_mode....to ...=========== %d", next_mode);
    NRF_LOG_FLUSH();

    // /* If we are in an interrupt, we need to wait for the events in the current event queue to be executed before resetting */
    //如果再中断中,那么延迟启动,放到事件队列中
    if (__get_IPSR() != 0) {
        /* Delay the reset operation to the event queue */
        if (app_sched_event_put(NULL, 0, delayed_reset) != NRF_SUCCESS) {
            NRF_LOG_ERROR("backing_store_write app_sched_event_put error");
        }

        return;
    }
    ret_code_t err_code;
    //正常启动
    // 2. 动态判断 SoftDevice 是否启用
    if (nrf_sdh_is_enabled()) {
        // 停用 SoftDevice
        err_code = nrf_sdh_disable_request();
        if (err_code != NRF_SUCCESS) {
            NRF_LOG_WARNING("SoftDevice disable failed: %d", err_code);
        }

        // 等待少许时间确保内部任务完成
        wait_ms(5);
    }

    wait_ms(100);
    // 安全设置 GPREGRET2,设置以后一定要重启,要不会挂
    NRF_POWER->GPREGRET2 = 0xA5; // 设置 GPREGRET2 寄存器为 0xA5

    NVIC_SystemReset();
}
