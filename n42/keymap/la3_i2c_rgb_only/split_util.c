
#include "compiler_support.h"
#include "split_util.h"
#include "matrix.h"
#include "keyboard.h"
#include "timer.h"
// #include "transport.h"
#include "wait.h"
#include "debug.h"
#include "usb_util.h"
#include "bootloader.h"

#ifdef EE_HANDS
#    include "eeconfig.h"
#endif

#if defined(RGBLIGHT_ENABLE) && defined(RGBLED_SPLIT)
#    include "rgblight.h"
#endif

#ifndef SPLIT_USB_TIMEOUT
#    define SPLIT_USB_TIMEOUT 2000
#endif

#ifndef SPLIT_USB_TIMEOUT_POLL
#    define SPLIT_USB_TIMEOUT_POLL 10
#endif

// Max number of consecutive failed communications (one per scan cycle) before the communication is seen as disconnected.
// Set to 0 to disable the disconnection check altogether.
#ifndef SPLIT_MAX_CONNECTION_ERRORS
#    define SPLIT_MAX_CONNECTION_ERRORS 10
#endif // SPLIT_MAX_CONNECTION_ERRORS

// How long (in milliseconds) to block all connection attempts after the communication has been flagged as disconnected.
// One communication attempt will be allowed everytime this amount of time has passed since the last attempt. If that attempt succeeds, the communication is seen as working again.
// Set to 0 to disable communication throttling while disconnected
#ifndef SPLIT_CONNECTION_CHECK_TIMEOUT
#    define SPLIT_CONNECTION_CHECK_TIMEOUT 500
#endif // SPLIT_CONNECTION_CHECK_TIMEOUT

static uint8_t connection_errors = 0;

volatile bool isLeftHand = true;

static struct {
    bool master;
    bool left;
} split_config;

__attribute__((weak)) bool is_keyboard_master(void) {
    return split_config.master;
}

// this code runs before the keyboard is fully initialized
void split_pre_init(void) {
    split_config.master = true;
    split_config.left   = true;

    isLeftHand = true;

    //改成初始化uart
    // transport_master_init();
}

// this code runs after the keyboard is fully initialized
//   - avoids race condition during matrix_init_quantum where slave can start
//     receiving before the init process has completed
void split_post_init(void) {

}

bool is_transport_connected(void) {
    return connection_errors < SPLIT_MAX_CONNECTION_ERRORS;
}
#include "cx_comm.h"
keyboard_matrix_t slave_matrix_d;
extern bool  get_matrix_from_queue(keyboard_matrix_t *matrix) ;
bool transport_master(matrix_row_t master_matrix[], matrix_row_t slave_matrix[]) {
    //todo
    // 清空之前的标志
    slave_matrix_d.flag = false;
     get_matrix_from_queue(&slave_matrix_d);
// return false;
    // if(slave_matrix_d.flag){
        // 复制数据到slave_matrix
        for (uint8_t i = 0; i < 4; i++) {
            slave_matrix[i] = slave_matrix_d.matrix[i];
        }
    // }

     return slave_matrix_d.flag;
    // return true;
    // return transactions_master(master_matrix, slave_matrix);
}

bool transport_master_if_connected_old(matrix_row_t master_matrix[], matrix_row_t slave_matrix[]) {

    // Throttle transaction attempts if target doesn't seem to be connected
    // Without this, a solo half becomes unusable due to constant read timeouts
    static uint16_t connection_check_timer = 0;
    const bool      is_disconnected        = !is_transport_connected();
    if (is_disconnected && timer_elapsed32(connection_check_timer) < 500) {
        return false;
    }

    __attribute__((unused)) bool okay = transport_master(master_matrix, slave_matrix);

    if (!okay) {
        if (connection_errors < UINT8_MAX) {
            connection_errors++;
        }

        bool connected = is_transport_connected();
        if (!connected) {
            connection_check_timer = timer_read();
            NRF_LOG_INFO("Target disconnected, throttling connection attempts");
        }
        return connected;
    } else if (is_disconnected) {
        NRF_LOG_INFO("Target connected");
    }

    connection_errors = 0;
    return true;
}
// static uint32_t connection_errors_qf = 0;


bool transport_master_if_connected(matrix_row_t master_matrix[], matrix_row_t slave_matrix[]) {

    __attribute__((unused)) bool okay = transport_master(master_matrix, slave_matrix);

      
    return okay;
}
