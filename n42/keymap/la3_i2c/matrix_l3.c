
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "atomic_util.h"
#include "74hc595.h"
#include "qf.h"

static const uint8_t col_to_595_pins[COL_TO_74HC595_PINS_COUNT]  = COL_TO_74HC595_PINS;

#    include "split_util.h"
#    define ROWS_PER_HAND (MATRIX_ROWS / 2)


#ifdef DIRECT_PINS_RIGHT
#    define SPLIT_MUTABLE
#else
#    define SPLIT_MUTABLE const
#endif
#ifdef MATRIX_ROW_PINS_RIGHT
#    define SPLIT_MUTABLE_ROW
#else
#    define SPLIT_MUTABLE_ROW const
#endif
#ifdef MATRIX_COL_PINS_RIGHT
#    define SPLIT_MUTABLE_COL
#else
#    define SPLIT_MUTABLE_COL const
#endif

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif



static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;

static const pin_t col_pins[MATRIX_COLS]   = MATRIX_COL_PINS;



/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
extern matrix_row_t matrix[MATRIX_ROWS];     // debounced values

#ifdef SPLIT_KEYBOARD
// row offsets for each hand
extern uint8_t thisHand, thatHand;
#endif

// user-defined overridable functions
__attribute__((weak)) void matrix_init_pins(void);
__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter);

static inline void gpio_atomic_set_pin_output_low(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_output(pin);
        gpio_write_pin_low(pin);
    }
}

static inline void gpio_atomic_set_pin_output_high(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_output(pin);
        gpio_write_pin_high(pin);
    }
}

static inline void gpio_atomic_set_pin_input_high(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        gpio_set_pin_input_high(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (gpio_read_pin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
    } else {
        return 1;
    }
}

// matrix code
extern void shift595_write_pin_ex(uint8_t pin_index, uint8_t level, uint8_t other_level);
static bool select_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
        gpio_atomic_set_pin_output_low(pin);
        return true;
    }else {
        if (col <= 10) {
            shift595_write_pin_ex(col_to_595_pins[col], 0, 1);
            return true;
        }
        else {
            return false;
        }
        return true;
    }
    return false;
}

static void unselect_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_atomic_set_pin_output_high(pin);
#            else
        gpio_atomic_set_pin_input_high(pin);
#            endif
    }else {
            shift595_write_all_high();
        }
}

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        unselect_col(x);
    }
}

 void matrix_init_pins(void) {
     shift595_pin_init();
    unselect_cols();
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        if (row_pins[x] != NO_PIN) {
            gpio_atomic_set_pin_input_high(row_pins[x]);
        }
    }
    shift595_write_all_high();
    // shift595_write_all_low();
}

__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    bool key_pressed = false;

    // Select col
    if (!select_col(current_col)) { // select col
        return;                     // skip NO_PIN col
    }
    matrix_output_select_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < ROWS_PER_HAND; row_index++) {
        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
            key_pressed = true;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed); // wait for all Row signals to go HIGH
}

void matrix_init(void) {

    thisHand = isLeftHand ? 0 : (ROWS_PER_HAND);
    thatHand = ROWS_PER_HAND - thisHand;


    // initialize key pins
    matrix_init_pins();

    // initialize matrix state: all keys off
    memset(matrix, 0, sizeof(matrix));
    memset(raw_matrix, 0, sizeof(raw_matrix));

    debounce_init(ROWS_PER_HAND);

    matrix_init_kb();
}

bool is_keyboard_master(void) {
    return true;
}
void print_split_matrix(matrix_row_t *matrix) {
    // 方法1：使用 NRF_LOG_RAW_INFO（推荐）
    NRF_LOG_RAW_INFO("=== Matrix State ===\n");
    
    // 打印右手矩阵
    NRF_LOG_RAW_INFO("Right hand:\n");
    for (int row = 0; row < ROWS_PER_HAND; row++) {
        NRF_LOG_RAW_INFO("R%02d: ", row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            bool pressed = (matrix[row + ROWS_PER_HAND] >> col) & 1;
            NRF_LOG_RAW_INFO("%d", pressed ? 1 : 0);
        }
        NRF_LOG_RAW_INFO("\n");
    }
    
    NRF_LOG_RAW_INFO("====================\n");
    // NRF_LOG_FLUSH();  // 确保输出
}
uint32_t counter = 1;
uint32_t countercc = 1;
#include "cx_comm.h"
extern keyboard_matrix_t slave_matrix_d;
bool matrix_post_scan_qf(void) {
    bool changed = false;

    counter++;
    //这里是主手
    static bool last_connected = false;
    matrix_row_t slave_matrix[ROWS_PER_HAND] = { 0 };
    //获取副手数据
    bool ssss=false;
    bool ss = transport_master_if_connected(matrix + thisHand, slave_matrix);
    if(!ss){
        if(countercc<10000){
            countercc++;
            if(slave_matrix_d.wk==0){
                ssss=false;
            }else{
                ssss=true;
            }
        }else{
            ssss=false;
        }
        
    }else{
        countercc=1;
        ssss=true;
    }
    if (ssss) {
        //和现有的数组比较是否有变化
        changed = memcmp(matrix + thatHand, slave_matrix, sizeof(slave_matrix)) != 0;
        if (counter % 5000 == 0) {
            if (changed) {
                NRF_LOG_INFO("11 data from slave  changed %ld\n", counter / 1000);
            } else {
                NRF_LOG_INFO("22 data from slave no changed %ld\n", counter / 1000);
            }
        }
        last_connected = true;
    } else if (last_connected) {
        if (changed) {
            NRF_LOG_INFO("333last_connected  changed %ld\n", counter / 1000);
        } else {
            NRF_LOG_INFO("444 last_connected no changed %ld\n", counter / 1000);
        }

        // reset other half when disconnected
        memset(slave_matrix, 0, sizeof(slave_matrix));
        changed = true;
        last_connected = false;
    } 
    //数据copy 到从机数组位置
    if (changed)
        memcpy(matrix + thatHand, slave_matrix, sizeof(slave_matrix));

     matrix_scan_kb();

  
//  if (changed) {

//         // 打印当前整个矩阵状态
//         print_split_matrix(matrix);
//     }
    return changed;
}

extern  bool read_keyboard_data_on_interrupt(void) ;
uint8_t matrix_scan(void) {
    matrix_row_t curr_matrix[MATRIX_ROWS] = {0};

    read_keyboard_data_on_interrupt();
    // Set col, read rows
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(curr_matrix, current_col, row_shifter);
    }


    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));


    changed = debounce(raw_matrix, matrix + thisHand, ROWS_PER_HAND, changed) | matrix_post_scan_qf();

    return (uint8_t)changed;
}
