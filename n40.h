#pragma once

#define LAYOUT_54(\
	K00, K01, K02, K03, \
	K10, K11, K12, K13, \
	K20, K21, K22, K23, \
	K30, K31, K32,      \
	K40, K41, K42, K43, \
	K50,      K52  \
) { \
	{ K00,   K01,   K02,   K03 }, \
	{ K10,   K11,   K12,   K13 }, \
	{ K20,   K21,   K22,   K23 }, \
	{ K30,   K31,   K32,   KC_NO }, \
	{ K40,   K41,   K42,   K43 }, \
	{ K50,   KC_NO, K52,   KC_NO }  \
}

uint8_t eeconfig_read_send_mode(void);
void eeconfig_update_send_mode(uint8_t val);
uint8_t eeconfig_read_dfu_flag(void);
void eeconfig_update_dfu_flag(uint8_t val);


#define QF_RESET NVIC_SystemReset()