set(BOARD "nrf52833_kit" CACHE STRING "KB" FORCE)

set(RADIO_ENABLE ON CACHE BOOL "KB" FORCE)
set(RADIO_ROLE "keyboard" CACHE STRING "KB" FORCE)
set(RADIO_DRIVER "esb" CACHE STRING "KB" FORCE)
set(RGB_MATRIX_ENABLE ON CACHE BOOL "KB" FORCE)
set(RGB_MATRIX_DRIVER "ws2812" CACHE STRING "KB" FORCE)
set(WS2812_DRIVER "pwm" CACHE STRING "KB" FORCE)
set(BLE_ENABLE ON CACHE BOOL "KB" FORCE)
set(UF2_FAMILY_ID "0x621e937a" CACHE STRING "KB" FORCE)
set(CUSTOM_MATRIX "lite" CACHE STRING "KB" FORCE)

# mxkbd_append_user_src(esb_send.c)
mxkbd_append_user_src(../../../cross/esb_send.c)
mxkbd_append_user_src(../../../cross/protocol_esb_major.c)
mxkbd_append_user_src(../../../cross/protocol_esb_other.c)
mxkbd_append_user_src(../../../cross/protocol_qmk_qf.c)
mxkbd_append_user_src(../../../cross/ble_services_pad.c)
mxkbd_append_user_src(../../../cross/ble_hid_service_pad.c)
mxkbd_append_user_src(../../../cross/usb_main_pad.c)
mxkbd_append_user_src(../../../cross/keymap_cb.c)
mxkbd_append_user_src(../../../cross/keys_hold.c)
mxkbd_append_user_src(../../../cross/ble_major.c)
mxkbd_append_user_src(../../../cross/qf_channel_manage.c)
mxkbd_append_user_src(../../../cross/rtc_qf.c)
# mxkbd_append_user_src(../../../common/74hc595.c)
# mxkbd_append_user_src(matrix_qf.c) inlcuded in parrent folder matrix_scan_user

mxkbd_append_user_src(../../../common/matrix_l3.c)

mxkbd_append_user_src(../../../common/rgb_t.c)

mxkbd_set_user_ld(Ld/nrf52833.ld)
add_compile_definitions(QMK_MCU_NRF52833)