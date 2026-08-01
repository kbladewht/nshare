#include "keyboard.h"
#include "debug.h"
#include "printf.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_usbd.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "nrf_delay.h"
#include "qf.h"


/**

__attribute__((weak)) void matrix_scan_kb(void) {
    matrix_scan_user();
}

keyboard_task->matrix_task-->matrix_scan()-> matrix_scan_kb()-> matrix_scan_user()
 */
//override 系统的scan函数
extern void matrix_scan_user_qf(void);
extern  void uart_task_qf(void);
extern void cx_uart_send_by_queue(void);

__attribute__((weak))  void matrix_scan_user_qf(void) {

}
void matrix_scan_user(void) {

    poll_hold_keys_scan();//支持3秒长按得自定义操作

    // #ifdef CX_HOST
    //    extern void cx_process_host(void);
    //    cx_process_host();
    // #endif
    matrix_scan_user_qf();
}