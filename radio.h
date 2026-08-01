
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "nrf_queue.h"
#include "nrf_esb.h"
#define WAIT_PRESSUP 2
#define NORMAL_HEART_BEAT 0
typedef struct{
  uint8_t p_data[8];
  uint16_t length;
} m_elementr_t;


void radio_init();

uint8_t radio_send_keyboard2(uint8_t *report);

void esb_tx_send(uint8_t *pdata);

void esb_send_heart_pack(uint32_t count_heart);
void radio_data_send_with_queue(void) ;
void add_to_message_queuer(uint8_t *data);