
#include "nrf_drv_saadc.h"
#include "adc.h"
#include "nrf_assert.h"
#include "app_error.h"

// #define NRF_LOG_MODULE_NAME "ADC"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"


static nrf_saadc_value_t       adc_buffer[1]; /**< ADC buffer. */
static void adc_event_handler(nrf_drv_saadc_evt_t const *p_event) {}

void adc_init() {
  nrf_drv_saadc_config_t adccfg = NRF_DRV_SAADC_DEFAULT_CONFIG;
  adccfg.resolution = NRF_SAADC_RESOLUTION_12BIT;
  ret_code_t ret = nrf_drv_saadc_init(&adccfg, adc_event_handler);
  APP_ERROR_CHECK(ret);
}

void adc_start() {
  nrf_saadc_channel_config_t pincfg = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDDHDIV5);
  nrf_drv_saadc_channel_init(0, &pincfg);
  ret_code_t res = nrf_drv_saadc_sample_convert(0, adc_buffer); // blocking function
  if (res != NRF_SUCCESS) {
    NRF_LOG_DEBUG("error for adc convert return value is : %d ", res);
    adc_buffer[0] = 0;
  }

   NRF_LOG_DEBUG("adc_start runing... ");

}



//  3.0V，最小电池电压为 2.0V
#define VDD_MAX 4200  // 最大电压 4.0V 转换为 4000 毫伏
#define VDD_MIN 2000  // 最小电压 2.0V 转换为 2000 毫伏

// 电池电量百分比计算 
uint16_t adc_to_voltage(uint16_t adc_value) {
    // 将 ADC 数值转换为电压值 (使用整数运算)
    return (adc_value * VDD_MAX) / ADC_MAX_VALUE;
}

// 计算电池电量百分比（整数）
uint16_t calculate_battery_percentage(uint16_t voltage_mV) {
    // 电池电压超过最大电压时返回 100%，低于最小电压时返回 0%
    if (voltage_mV >= VDD_MAX) {
        return 100;  // 满电
    } else if (voltage_mV <= VDD_MIN) {
        return 0;    // 空电
    } else {
        // 计算电池电量百分比
       uint16_t diff= (voltage_mV - VDD_MIN) ;
        //  NRF_LOG_INFO("diff is %d",diff);
        //  NRF_LOG_INFO("diff2 is %d",(VDD_MAX - VDD_MIN));
        return (diff * 100) / (VDD_MAX - VDD_MIN);
    }
}

uint16_t get_battery_percentage(void) {
    uint16_t adc_value = adc_buffer[0] < 0 ? 0 : adc_buffer[0];
    //  NRF_LOG_INFO("adc_value is %d",adc_value);
   uint16_t voltage_mV = adc_to_voltage(adc_value);  // 计算电池电压（单位为毫伏）
    // NRF_LOG_INFO("voltage_mV is %d",voltage_mV);
    uint16_t fpercent= calculate_battery_percentage(voltage_mV);   // 返回电池电量百分比
    //  NRF_LOG_INFO("fpercent is %d",fpercent);
    return fpercent;
}