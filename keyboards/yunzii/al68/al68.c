// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "uart.h"
//#include "common/smart_ble.h"
#include "al68.h"

void keyboard_pre_init_kb(void) {
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk);
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE; // diable JTAG (GD32 different to STM32)
    gpio_set_pin_output(A8);
    gpio_write_pin_high(A8); // ENABLE USB
    uart_init(460800);
    wait_ms(600);
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
// batteries removed, don't need wireless keycodes
//    if (record->event.pressed) {
//        if ((keycode >= KC_BLE1) && (keycode <= KC_24G)) {
//            wireless_start(keycode - KC_USB);
//            return false;
//        } else if (keycode == KC_USB) {
//            wireless_stop();
//            return false;
//        }
//    }
    return process_record_user(keycode, record);
}

#define __ NO_LED

led_config_t g_led_config = {
    {
        // Key Matrix to LED Index
        {  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  __ },
        {  53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41,  40,  39 },
        {  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  __,  37,  38 },
        {  24,  __,  20,  19,  18,  17,  16,  15,  14,  13,  12,  11,  10,   9,   8 },
        {  23,  22,  21,  __,  __,   0,  __,  __,   3,   4,   1,   2,   5,   6,   7 },
    },  
    {   
        // LED Index to Physical Position
        {89,64}, {150,64}, {170,64}, {187,55}, {187,59}, {194,64}, {209,64}, {224,64}, {224,48}, 
        {224,48}, {189,48}, {168,48}, {153,48}, {138,48}, {123,48}, {108,48}, {93,48}, {78,48}, 
        {63,48}, {48,48}, {33,48}, {39,48}, {19,64}, {2,64}, {8,48}, {6,32}, {26,32}, {41,32}, 
        {56,32}, {71,32}, {86,32}, {101,32}, {116,32}, {131,32}, {146,32}, {161,32}, {176,32}, 
        {201,32}, {224,32}, {224,15}, {207,15}, {187,15}, {172,15}, {157,15}, {142,15}, {127,15}, 
        {112,15}, {97,15}, {82,15}, {67,15}, {52,15}, {37,15}, {22,15}, {4,15}, {0, 0}, {15, 0}, 
        {30, 0}, {45, 0}, {60, 0}, {75, 0}, {90, 0}, {105, 0}, {120, 0}, {135, 0}, {150, 0}, 
        {165, 0}, {180, 0}, {203, 0}
    },
    {
        // RGB LED Index to Flag
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 
    }
};

