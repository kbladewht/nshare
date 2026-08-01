// select the right input source for device

#include <stdint.h>

void init_suitable_send_mode();

void start_current_mode();

uint8_t current_ram_dfu_flag();
void update_dfu_flag_eeprom(uint8_t flag);
uint8_t read_dfu_flag_eeprom();
void auto_set_protocol_mode_by_detect(void);

