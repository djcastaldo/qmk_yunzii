/* 5/31/2025 custom keymap.c by @davex 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
// #include "common/smart_ble.h"
#include "print.h"
#include "features/layer_lock.h"

#define F_ZOOMI LCTL(KC_MS_WH_UP)               // form zoom in   
#define F_ZOOMO LCTL(KC_MS_WH_DOWN)             // form zoom out
#define F_ZOOMR LCTL(KC_0)                      // form zoom reset 

__attribute__ ((weak))
bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
  return true;
}
__attribute__ ((weak))
bool process_leader_secrets(void) {
  return true;
}

// layer identifiers
enum layers {
    BASE_LAYR,
    FN_LAYR,
    SFT_LAYR,
    CTL_LAYR,
    TMUX_LAYR,
    WIDE_TEXT_LAYR,
    LOCK_LAYR
};

// custom keycodes
enum my_keycodes {
    BSPCFAST = NEW_SAFE_RANGE,
    ARROW,
    WAVE,
    LLOCK,
    ENC_VOLU,
    ENC_VOLD,
    ENC_DUALPUSH,
    ENC_RGBRESET,
    DUAL_PLUSMIN,
    DUAL_MULTDIV,
    DUAL_ESC,
    DUAL_ZOOMI,
    DUAL_ZOOMO,
    FJLIGHT,
    HROWLIGHT,
    KTRACK,
    JIGGLE,
    MK_HOLD,
    MK_REL,
    MK_ACCEL0,
    MK_ACCEL2,
    ENC_TSIZEL,
    ENC_TSIZER,
    ENC_TMON,
    LTRANS,
    STHRU,
    UNDERLN,
    BARTEXT,
    COLORTEST,
    FLASH_KB,
    BOOTLDR
};

// custom tap dance key     
enum {
    CAPS_LAYR = 0,
    FN_OSL = 1,
    RCTL_OSL = 2,            
    RSFT_OSL = 3,
    KB_UNLOCK = 4 
}; 

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE_LAYR] =  { ENCODER_CCW_CW(ENC_VOLD, ENC_VOLU)  },
    [FN_LAYR]   =  { ENCODER_CCW_CW(DUAL_ZOOMO, DUAL_ZOOMI)  },
    [SFT_LAYR]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)  },
    [CTL_LAYR]  =  { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)  },
    [TMUX_LAYR] =  { ENCODER_CCW_CW(ENC_TSIZEL, ENC_TSIZER)  },
    [WIDE_TEXT_LAYR] =  { ENCODER_CCW_CW(KC_TRNS, KC_TRNS)  },
    [LOCK_LAYR] =  { ENCODER_CCW_CW(KC_NO, KC_NO)  }
};
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//  [BASE_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |D-ESC || 1    || 2    || 3    || 4    || 5    || 6    || 7    || 8    || 9    || 0    || -    || =    || Backspace    |: Vol  :  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  | LT(TMUX) || Q    || W    || E    || R    || T    || Y    || U    || I    || O    || P    || [    || ]    || \        || Del  |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  | CAPS / Fn   || A    || S    || D    || F    || G    || H    || J    || K    || L    || ;    || '    || Enter         || PgUp |  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  | Shift           || Z    || X    || C    || V    || B    || N    || M    || ,    || .    || /    ||TD(SFT_OSL)|| Up   || End  |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  | Ctrl    || GUI    || Alt     ||                   Space                      ||TD(FN_O) ||TD(CTL_O)|  | Left || Down || Rght |  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [BASE_LAYR] = LAYOUT_65_ansi_blocker(
        DUAL_ESC, KC_1,  KC_2,   KC_3,  KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0, KC_MINS,  KC_EQL,  BSPCFAST, ENC_DUALPUSH,
        LT(TMUX_LAYR,KC_TAB), KC_Q, KC_W, KC_E, KC_R, KC_T,  KC_Y,  KC_U,   KC_I,   KC_O,   KC_P,  KC_LBRC, KC_RBRC, KC_BSLS, KC_DELETE,
        TD(CAPS_LAYR), KC_A,   KC_S,  KC_D,    KC_F,   KC_G,   KC_H,  KC_J,   KC_K,    KC_L,  KC_SCLN, KC_QUOT,  KC_ENT,     KC_PAGE_UP,
        KC_LSFT,       KC_Z,  KC_X,  KC_C,   KC_V,   KC_B,   KC_N,   KC_M,  KC_COMM, KC_DOT, KC_SLSH, TD(RSFT_OSL), KC_UP, KC_PAGE_DOWN,
        KC_LCTL,  KC_LGUI,  KC_LALT,                    KC_SPC,                    TD(FN_OSL), TD(RCTL_OSL), KC_LEFT, KC_DOWN, KC_RIGHT
    ),
//  [FN_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |  `   || F1   || F2   || F3   || F4   || F5   || F6   || F7   || F8   || F9   || F10  || F11  || F12  ||              |: Zoom :  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |MO(TMUX_L)||WMail2||WMail1||Mail  ||RPhone||WPhone||      ||      ||      ||      ||      ||MRec1 ||MRec2 ||          || Ins  |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |             ||Addr  ||City  ||ZIP   ||Phone ||      ||      ||      ||      || Lead ||SLock || PScr ||               || Home |  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  | MO(SFT_LAYR)    ||      ||Rout  ||Acct  ||      ||      ||      ||      ||MPly1 ||MPly2 ||Pause || MO(SFT_L) ||      || End  |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  |         ||        ||MO(CTL_L)||                                              ||         ||MO(CTL_L)|  |      ||      ||      |  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [FN_LAYR] = LAYOUT_65_ansi_blocker(
        KC_GRV, KC_F1, KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,   KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_F11, KC_F12,    _______,    F_ZOOMR,
        MO(TMUX_LAYR),SECRET3,SECRET2,SECRET1,SECRET8,SECRET9,_______,_______,_______,_______,_______,DM_REC1,DM_REC2, _______,  KC_INS,
        _______,SECRET4,SECRET5,SECRET6,SECRET7,_______,_______,_______,_______,    QK_LEAD,  KC_SCRL, KC_PSCR,     _______,    KC_HOME,
        MO(SFT_LAYR), _______,SECRET10,SECRET11,_______,_______,_______,_______,DM_PLY1,DM_PLY2, KC_PAUS, MO(SFT_LAYR), _______, KC_END,
        _______, _______, MO(CTL_LAYR),                 _______,                    _______,    MO(CTL_LAYR), _______, _______, _______
    ),
//  [SFT_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |  ~   ||MyComp||      || Calc || Menu ||      ||      || MPrv || MPly || MNxt ||      ||Arrow || Wave ||              |:Scroll:  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |          ||      ||      ||      ||      ||      ||      ||  P7  ||  P8  ||  P9  || +  - ||      ||      ||  NumLock ||MSRel |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |             ||      ||      ||      ||      ||      ||      ||  P4  ||  P5  ||  P6  || *  / ||      ||               ||MSHold|  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  |                 ||MSAcl2||      ||      ||      ||      ||      ||  P1  ||  P2  ||  P3  ||      ||           || MSUp || RClk |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  |  LCtl   ||        || MSAcl0  ||                     P0                       ||   P.    ||  LClk   |  |MSLft || MSDn ||MSRgt |  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [SFT_LAYR] = LAYOUT_65_ansi_blocker(
        KC_TILD, KC_MYCM, _______, KC_CALC, KC_APP, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, ARROW, WAVE,  _______, JIGGLE,
        _______, _______,_______,_______, _______, _______, _______, KC_P7, KC_P8, KC_P9, DUAL_PLUSMIN,_______,_______,  KC_NUM, MK_REL,
        _______,    _______,_______,_______,_______,_______, _______, KC_P4, KC_P5, KC_P6, DUAL_MULTDIV, _______,   _______,    MK_HOLD,
        _______,   MK_ACCEL2,_______,_______,_______,_______,_______,  KC_P1, KC_P2, KC_P3,  _______,   _______,   KC_MS_UP, KC_MS_BTN2,
        _______,  _______, MK_ACCEL0,                  KC_P0,                  KC_PDOT, KC_MS_BTN1, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT
    ),
//  [CTL_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||Ctest ||Debug ||   RGBTogg    |: RGB  :  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |          ||      ||      ||      ||Reboot||      ||      ||      ||      ||      ||      ||      ||      || RGBMode  ||      |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |             ||      ||      ||      ||Flash ||      ||      ||      ||      ||      ||KTrack||FJLite||  HRowLight    ||RHue+ |  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  |                 ||      ||      || EClr ||      ||BootLd||      ||      ||      ||      ||      ||           ||RBri+ ||RHue- |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  |         ||        ||         ||                                              ||         ||         |  |RSpd- ||RBri- ||RSpd+ |  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [CTL_LAYR] = LAYOUT_65_ansi_blocker(
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,COLORTEST,DB_TOGG,RGB_TOG, ENC_RGBRESET,
        _______,_______,_______,_______, QK_RBT, _______,_______,_______,_______,_______,_______,_______, _______,  RGB_MOD,    _______,
        _______, _______,_______,_______, FLASH_KB, _______,_______,_______,_______,_______, KTRACK, FJLIGHT,    HROWLIGHT,     RGB_HUI,
        _______,       _______,_______, EE_CLR, _______,  BOOTLDR, _______,_______,_______,_______,_______,   _______, RGB_VAI, RGB_HUD,
        _______, _______, _______,                       _______,                      _______, _______,      RGB_SPD, RGB_VAD, RGB_SPI
    ),
//  [TMUX_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |      ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||              |:Resize:  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |          ||LTRANS||LTRANS||      ||      ||LTRANS||      ||      ||      ||LTRANS||LTRANS||LTRANS||LTRANS||          ||      |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |             ||      ||LTRANS||LTRANS||      ||      ||      ||      ||      ||LTRANS||LTRANS||LTRANS||               ||LTRANS|  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  |                 ||LTRANS||LTRANS||LTRANS||      ||      ||LTRANS||      ||LTRANS||LTRANS||LTRANS||           ||LTRANS||      |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  | Ctrl    ||        ||         ||                    LTRANS                    ||         ||  Ctrl   |  |LTRANS||LTRANS||LTRANS|  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [TMUX_LAYR] = LAYOUT_65_ansi_blocker(
        _______, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,  LTRANS,     _______, ENC_TMON,
        _______, LTRANS, LTRANS,_______,_______,  LTRANS,_______,_______,_______, LTRANS, LTRANS, LTRANS,  LTRANS,  _______,    _______,
        _______, _______, LTRANS, LTRANS,_______,_______,_______,_______,_______, LTRANS, LTRANS, LTRANS,           _______,     LTRANS,
        _______,        LTRANS, LTRANS, LTRANS, _______,  _______,  LTRANS,_______, LTRANS, LTRANS, LTRANS,   _______,  LTRANS, _______,
        KC_LCTL, _______, _______,                        LTRANS,                      _______, KC_RCTL,       LTRANS,  LTRANS,  LTRANS
    ),
//  [WIDE_TEXT_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |      ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||              |:      :  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |          ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS    ||BarTxt|  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |             ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||               ||SThru |  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  |                 ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||           ||      ||UnderL|  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  |         ||        ||         ||                    LTRANS                    ||         ||         |  |      ||      ||      |  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [WIDE_TEXT_LAYR] = LAYOUT_65_ansi_blocker(
        _______, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,  LTRANS,    _______,   _______,
        _______,   LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,  LTRANS,  LTRANS,    BARTEXT,
        _______,     LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,        _______,      STHRU,
        _______,          LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,    _______,  _______, UNDERLN,
        _______,  _______,  _______,                        LTRANS,                      _______,  _______,   _______, _______, _______
    ),
//  [LOCK_LAYR]
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  || No   ||  No  ||  No  ||  No  ||  No  ||     No       |: Unlk :  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |   No     ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||   No     ||  No  |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |     No      ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||      No       ||  No  |  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  |       No        ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||    No     ||  No  ||  No  |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  |   No    ||   No   ||   No    ||                     No                       ||   No    ||   No    |  |  No  ||  No  ||  No  |  :
//  :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
    [LOCK_LAYR] = LAYOUT_65_ansi_blocker(
        KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO, KC_NO,  KC_NO,    TD(KB_UNLOCK),
        KC_NO,     KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO, KC_NO,  KC_NO,  KC_NO,       KC_NO,   KC_NO,
        KC_NO,         KC_NO,  KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,      KC_NO,        KC_NO,
        KC_NO,              KC_NO,  KC_NO,  KC_NO, KC_NO,  KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,    KC_NO,     KC_NO,   KC_NO,
        KC_NO,     KC_NO,     KC_NO,                       KC_NO,                        KC_NO,    KC_NO,        KC_NO,  KC_NO,   KC_NO
    )
};

//  <~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>
//  ~=~=~=~=~=~=~=~=~=~LED Indexes~=~=~=~=~=~=~=~=~=~=~ 
//   ____________________________________________________________________________________________________________________________________
//  :   _______________________________________________________________________________________________________________________ .----.   :
//  :  |  54  ||  55  ||  56  ||  57  ||  58  ||  59  ||  60  ||  61  ||  62  ||  63  ||  64  ||  65  ||  66  ||     67       |:      :  :
//  :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
//  :  |    53    ||  52  ||  51  ||  50  ||  49  ||  48  ||  47  ||  46  ||  45  ||  44  ||  43  ||  42  ||  41  ||   40     ||  39  |  :
//  :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
//  :  |     25      ||  26  ||  27  ||  28  ||  29  ||  30  ||  31  ||  32  ||  33  ||  34  ||  35  ||  36  ||      37       ||  38  |  :
//  :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
//  :  |       24        ||  20  ||  19  ||  18  ||  17  ||  16  ||  15  ||  14  ||  13  ||  12  ||  11  ||    10     ||  9   ||  8   |  :
//  :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
//  :  |   23    ||   22   ||   21    ||                      0                       ||    1    ||    2    |3 |  5   ||  6   ||  7   |  :
//  :  |_________||________||_________||______________________________________________||_________||_________| 4|______||______||______|  :
//  `------------------------------------------------------------------------------------------------------------------------------------`
// useful key indexes
enum key_indexes {
    I_INDICATOR = 3,
    I_INDICATOR2 = 4,
    I_UNDERLN = 8,
    I_STHRU = 38,
    I_BARTEXT = 39,
    I_NUMLOCK = 40,
    I_ESC = 54,
    I_TAB = 53,
    I_Q = 52,
    I_W = 51,
    I_E = 50,
    I_R = 49,
    I_T = 48,
    I_Y = 47,
    I_U = 46,
    I_I = 45,
    I_O = 44,
    I_P = 43,
    I_MREC1 = 42,
    I_MREC2 = 41,
    I_CAPS = 25,
    I_A = 26,
    I_S = 27,
    I_D = 28,
    I_F = 29,
    I_G = 30,
    I_H = 31,
    I_J = 32,
    I_K = 33,
    I_L = 34,
    I_SEMI = 35,
    I_SLOCK = 35,
    I_FJLIGHT = 36,
    I_HROWLIGHT = 37,
    I_LSFT = 24,
    I_MPLY1 = 13,
    I_MPLY2 = 12,
    I_RSFT = 10,
    I_LCTL = 23,
    I_RCTL = 2,
    I_LGUI = 22,
    I_LALT = 21,
    I_FN = 1
};

// led indexes for keys that get capitalized when caps lock is on
bool is_capslock_shifted(uint8_t i) {
    if ((i > 42 && i < 53) || (i > 25 && i < 35) || (i > 13 && i < 21)) {
        return true;
    }
    return false;
}
// led indexes for keys that get shifted when caps word is on
bool is_capsword_shifted(uint8_t i) {
    if (i == 65 || is_capslock_shifted(i)) {
        return true;
    }
    return false;
}

// tap dance setup
typedef struct {
    bool is_press_action;
    int state;
} tap;
// tap dance states
enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    TRIPLE_TAP = 4,
    QUAD_TAP = 5,
    PENT_TAP = 6
};
// functions associated with all tap dances
int cur_dance (tap_dance_state_t *state);
// functions associated with individual tap dances
void caps_finished (tap_dance_state_t *state, void *user_data);
void caps_reset (tap_dance_state_t *state, void *user_data);
void fn_finished (tap_dance_state_t *state, void *user_data);
void fn_reset (tap_dance_state_t *state, void *user_data);
void rctl_finished (tap_dance_state_t *state, void *user_data);
void rctl_reset (tap_dance_state_t *state, void *user_data);
void rsft_finished (tap_dance_state_t *state, void *user_data);
void rsft_reset (tap_dance_state_t *state, void *user_data);
void kbunlock_finished (tap_dance_state_t *state, void *user_data);
void kbunlock_reset (tap_dance_state_t *state, void *user_data);

// key tracker
typedef struct {
    uint8_t index;
    bool press;
    int fade;
} keytracker;

// function for determining if a key should fade
bool key_should_fade(keytracker key, uint8_t layer);

// setup keytracker
static deferred_token key_token = INVALID_DEFERRED_TOKEN;
static keytracker tracked_keys[20];
static int tk_length = sizeof(tracked_keys) / sizeof(tracked_keys[0]);

// setup mouse jiggler
static deferred_token jiggler_token = INVALID_DEFERRED_TOKEN;
static report_mouse_t jiggler_report = {0};

// for tracking if leader sequence is started
bool is_in_leader_sequence; 
bool is_leader_led_on;
static uint16_t leader_timer;
bool is_leader_error;
bool is_leader_error_led_on;
static uint16_t leader_error_timer;

// setup leader sequence error blinking
static deferred_token leader_error_token = INVALID_DEFERRED_TOKEN;
uint32_t leader_error_callback(uint32_t trigger_time, void* cb_arg) {
    is_leader_error = false;
    is_leader_error_led_on = false;
    return 0;
}

// for tracking whether to highlight home row keys f and j
bool fj_light;
// and for tracking if the full home row light is on
bool hrow_light;
// for disabling the keytracker, which will also disable key-reactive fade
bool enable_keytracker = true;

// for tracking whether to blink an led as an indicator  
bool is_led_on;
static uint16_t layer_timer;

// for tracking layer lock in order to flash the indicator 
bool is_layer_lock_led_on;
static uint16_t layer_lock_timer;

// for tracking key lock blinking 
bool is_key_lock_led_on;
static uint16_t key_lock_timer;

// for tracking a recording macro 
int8_t macro_direction;
bool macro_recording;
bool is_macro_led_on;
static uint16_t macro_timer; 
// and a delayed callback after playing a macro from osl
static deferred_token osl_macro_token = INVALID_DEFERRED_TOKEN;

// funciton to send an alternate key if a modifier is being held
void dual_key(uint16_t std_keycode, uint16_t alt_keycode, uint8_t mod_mask);

// if rgb color-test is requested, set this bool
bool color_test;
// and use a timer so that it can be turned off after a delay even if no further key is pressed
static uint16_t color_test_timer;

// for tracking if oneshot layer is active
bool oneshot_layer_active;

// callback for when a mcaro on osl is run (to turn off the layer) 
uint32_t osl_macro_callback(uint32_t trigger_time, void *cb_arg) {
    layer_off(FN_LAYR);
    return 0;
}

// for tracking wide-text options for the WIDE_TEXT_LAYR
bool wide_sthru = false;
bool wide_underln = false;
bool wide_bartext = false;
bool wide_firstchar = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    // stop color test if active and a key is pressed
    if (color_test && record->event.pressed) {
        color_test = false;
        return false;
    }

    // stop mouse jiggler
    if (jiggler_token && record->event.pressed) {
        // if jiggler is currently running, stop when key is pressed
        cancel_deferred_exec(jiggler_token);
        jiggler_token = INVALID_DEFERRED_TOKEN;
        jiggler_report = (report_mouse_t){};  // clear the mouse
        host_mouse_send(&jiggler_report);
    }

    // record key index pressed for rgb reactive changes
    if (enable_keytracker && !is_macro_playing) {
        int key_idx = g_led_config.matrix_co[record->event.key.row][record->event.key.col];
        if (key_idx > 2 && key_idx < 255) {
            //key_idx = key_idx + 2;
        }
        // don't record some keycodes, because it makes the wrong keys light up when the rotary encoder uses these
        if (keycode != ENC_VOLU && keycode != ENC_VOLD && keycode != F_ZOOMI && keycode != F_ZOOMO &&
            keycode != KC_MS_WH_UP && keycode != KC_MS_WH_DOWN && keycode != RGB_MOD && keycode != RGB_RMOD &&
            keycode != ENC_TSIZEL && keycode != ENC_TSIZER) {
            if (record->event.pressed) {
                dprintf("%u \n", key_idx);
                if (key_idx != 255 ) { // no encoder tracking
                    for (int i = tk_length - 1; i > 0; i--) {
                        tracked_keys[i] = tracked_keys[i-1];
                        if (tracked_keys[i].index == key_idx) {
                            tracked_keys[i].press = true;
                            tracked_keys[i].fade = 255;
                        }
                    }
                    tracked_keys[0].press = true;
                    tracked_keys[0].fade = 255;
                    tracked_keys[0].index = key_idx;
                }
            }
            else {
                for (int i = 0; i < tk_length; i++) {
                    if (tracked_keys[i].index == key_idx) {
                        tracked_keys[i].press = false;
                        tracked_keys[i].fade = 119;
                    }
                }
                // setup the key fade
                if (key_token) {
                    cancel_deferred_exec(key_token);
                    key_token = INVALID_DEFERRED_TOKEN;
                }
                uint32_t keytracker_callback(uint32_t trigger_time, void* cb_arg) {
                    bool fade_changed = false;
                    for (int i = 0; i < tk_length; i++) {
                        if (!tracked_keys[i].press && tracked_keys[i].fade > 0) {
                            tracked_keys[i].fade--;
                            fade_changed = true;
                        }
                    }
                    if (fade_changed) {
                        return 20;  // Call the callback every 20ms
                    }
                    else {
                        return 0;
                    }
                }
                key_token = defer_exec(10, keytracker_callback, NULL);  // Schedule callback.
            }
        }
    }
    // layer lock
    if (!process_layer_lock(keycode, record, LLOCK)) {
       return false;
    }  
    switch (keycode) {
    // this is a custom version of KC_TRANS to press a key on default layer
    // setup so that I can use LTRANS in the keymap to denote which fallthrough keys get lit up on the layer
    case LTRANS:
        if (record->event.pressed) {
            uint8_t layer = get_highest_layer(layer_state);
            const uint8_t mods = get_mods();
            // prefix to send for the TMUX_LAYR
            // standard KC_TRANS keycodes will not get this prefix which is good for
            // stuff like shift and alt and control
            if (layer == TMUX_LAYR) {
                unregister_mods(mods); // temp remove mods
                tap_code16(C(KC_B));   // send ctrl-b before keycode processing
                register_mods(mods);   // reapply mods
            }
            // for some wide modes, should start with the spacing char
            else if (layer == WIDE_TEXT_LAYR && wide_firstchar) {
                unregister_mods(mods); // temp remove mods
                if (wide_bartext) {
                    tap_code16(KC_PIPE);
                }
                else if (wide_sthru) {
                    tap_code16(KC_MINS);
                }
                else if (wide_underln) {
                    tap_code16(KC_UNDS);
                }
                register_mods(mods);   // reapply mods
                wide_firstchar = false;
            }
            // send keydown from the default layer
            register_code(keymap_key_to_keycode(biton32(default_layer_state), record->event.key));
            // if WIDE_TEXT_LAYER, add the extra spacing char
            if (layer == WIDE_TEXT_LAYR) {
                unregister_mods(mods); // temp remove mods
                if (wide_bartext) {
                    tap_code16(KC_PIPE);
                }
                else if (wide_sthru) {
                    tap_code16(KC_MINS);
                }
                else if (wide_underln) {
                    tap_code16(KC_UNDS);
                }
                else {
                    tap_code16(KC_SPC);
                }
                register_mods(mods);   // reapply mods
            }
        }
        else {
            unregister_code(keymap_key_to_keycode(biton32(default_layer_state), record->event.key));
        }
        return false;
        break;
    // jiggler to keep from screen timeout without adjusting power settings
    case JIGGLE:
        if (record->event.pressed) {
            uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
                // deltas to move in a circle of radius 20 pixels over 32 frames
                static const int8_t deltas[32] = {
                  0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
                  0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0};
                static uint8_t phase = 0;
                // get x delta from table and y delta by rotating a quarter cycle
                jiggler_report.x = deltas[phase];
                jiggler_report.y = deltas[(phase + 8) & 31];
                phase = (phase + 1) & 31;
                host_mouse_send(&jiggler_report);
                return 16;  // call the callback every 16 ms
            }
            jiggler_token = defer_exec(1, jiggler_callback, NULL);  // schedule callback
        }
        break;
    // repeats pattern while key held, can do either ~=~=~=~=~=~ or <~>~<~>~<~>~<~>
    case WAVE:  
        static deferred_token wave_token = INVALID_DEFERRED_TOKEN;
        static uint8_t wave_phase = 0;
        if (!record->event.pressed) {  // on release
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            cancel_deferred_exec(wave_token);
            wave_token = INVALID_DEFERRED_TOKEN;
            // ensure the pattern always ends on a ">"
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {         // is ctl held?
                unregister_mods(MOD_MASK_CTRL); // temp remove ctl
                if ((wave_phase & 1) == 0) {
                    send_string("<~>");
                }
                else {
                    tap_code16(KC_RABK);
                }
                register_mods(mods);            // restore mods
            }
            else {
                if ((wave_phase & 1) == 0) { tap_code16(KC_TILD); }
            }
            wave_phase = 0;
        }
        else if (!wave_token) {  // on press
            uint32_t wave_callback(uint32_t trigger_time, void* cb_arg) {
                const uint8_t mods = get_mods();
                const uint8_t oneshot_mods = get_oneshot_mods();
                if ((mods | oneshot_mods) & MOD_MASK_CTRL) {  // is ctl held?
                    del_oneshot_mods(MOD_MASK_CTRL); // remove ctl 
                    unregister_mods(MOD_MASK_CTRL);  // remove ctl 
                    tap_code16((++wave_phase & 1) ? KC_LABK : KC_RABK);
                    tap_code16(KC_TILD);
                    register_mods(mods);             // restore mods
                }
                else {
                    tap_code16((++wave_phase & 1) ? KC_TILD : KC_EQL);
                }
                return 16;  // call the callback every 16 ms
            }
            wave_token = defer_exec(1, wave_callback, NULL);
        }
        return false;
        break;
    // use for -> or =>
    case ARROW:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {  // is ctl held?
                // temporarily delete ctl
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                SEND_STRING("=>");
                register_mods(mods);            // restore mods
                return false;
            } else {
                SEND_STRING("->");
            }
        }
        break;
    // backspace with exponential repeating.
    case BSPCFAST:  
        // Initial delay before the first repeat.
        static const uint8_t INIT_DELAY_MS = 250;
        // This array customizes the rate at which the Backspace key
        // repeats. The delay after the ith repeat is REP_DELAY_MS[i].
        // Values must be between 1 and 255.
        static const uint8_t REP_DELAY_MS[] PROGMEM = {
            99, 79, 65, 57, 49, 43, 40, 35, 33, 30, 28, 26, 25, 23, 22, 20,
            20, 19, 18, 17, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10};
        static deferred_token bspc_token = INVALID_DEFERRED_TOKEN;
        static uint8_t rep_count = 0;
        if (!record->event.pressed) {  // Backspace released: stop repeating.
            cancel_deferred_exec(bspc_token);
            bspc_token = INVALID_DEFERRED_TOKEN;
        }
        else if (!bspc_token) {  // Backspace pressed: start repeating.
            tap_code(KC_BSPC);  // Initial tap of Backspace key.
            rep_count = 0;
            uint32_t bspc_callback(uint32_t trigger_time, void* cb_arg) {
              tap_code(KC_BSPC);
              if (rep_count < sizeof(REP_DELAY_MS)) { ++rep_count; }
              return pgm_read_byte(REP_DELAY_MS - 1 + rep_count);
            }
            bspc_token = defer_exec(INIT_DELAY_MS, bspc_callback, NULL);
        }
        break;
    // get dynamic macros to work even with oneshot layers
    case DM_REC1:
    case DM_REC2:
    case DM_PLY1:
    case DM_PLY2:
        if (record->event.pressed) {
            if (oneshot_layer_active) {
                reset_oneshot_layer();
                osl_macro_token = defer_exec(100, osl_macro_callback, NULL);
            }
        }
        break;
    case ENC_VOLU:
        if (record->event.pressed) {
           tap_code(KC_VOLU); 
        }   
        break;
    case ENC_VOLD:
        if (record->event.pressed) {
           tap_code(KC_VOLD); 
        }   
        break;
    case ENC_RGBRESET:
        if (record->event.pressed) {
            rgblight_mode(6);
        }   
        break;
    case DUAL_PLUSMIN:
        if (record->event.pressed) {
            // standard: plus symbol, while control is held: minus
            dual_key(KC_PPLS, KC_PMNS, MOD_MASK_CTRL);
        }
        break;
    case DUAL_MULTDIV:
        if (record->event.pressed) {
            // standard: asterisk, while control is held: divide
            dual_key(KC_PAST, KC_PSLS, MOD_MASK_CTRL);
        }
        break;
    // use esc key to stop recording a dynamic macro
    case DUAL_ESC:
        if (record->event.pressed) { 
           if (!macro_recording) {
               // send escapse 
               register_code(KC_ESC);
           }
           else {
               // if macro is recording, stop it
               dynamic_macro_stop_recording();
           }
        }
        else {
            unregister_code(KC_ESC);
        }
        break;
    //  insert delay if recording or playing a macro. otherwise, send mute
    case ENC_DUALPUSH:
        if (record->event.pressed) {
            if (macro_recording || is_macro_playing) {
                send_string(SS_DELAY(150));
            }
            else {
                tap_code(KC_MUTE);
            }
        }
        break;
    // set up some different zoom (when ctrl is used) so the zoom knob can be used with multiple apps
    // that support different ways to zoom
    case DUAL_ZOOMI:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                send_string(SS_TAP(X_EQL));
            }
            else {
                tap_code16(F_ZOOMI);
            }
        }
        break;
    case DUAL_ZOOMO:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                send_string(SS_TAP(X_MINS));
            }
            else {
                tap_code16(F_ZOOMO);
            }
        }
        break;
    // tmux encoder control 
    // set this up to do resize l/r or u/d if control is held 
    case ENC_TSIZEL:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                unregister_mods(MOD_MASK_CTRL);                                  // remove control 
                send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_UP)),5);    // size up
                register_mods(mods);                                             // add back mods
            }
            else {
                send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_LEFT)),5);  // size left 
            }
        }
        break;
    case ENC_TSIZER:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                unregister_mods(MOD_MASK_CTRL);                                  // remove control 
                send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_DOWN)),5);  // size down
                register_mods(mods);                                             // add back mods
            }
            else {
                send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_RIGHT)),5); // size right 
            }
        }
        break;
    // when encoder is pushed, either enable or disable window activity monitor 
    case ENC_TMON:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                unregister_mods(MOD_MASK_CTRL);             // remove control 
                // turn on window actiivty monitor
                send_string_with_delay(SS_LCTL("b") ":setw monitor-activity on" SS_TAP(X_ENT),8);
                register_mods(mods);                        // add back mods
            }
            else {
                // turn off window actiivty monitor
                send_string_with_delay(SS_LCTL("b") ":setw monitor-activity off" SS_TAP(X_ENT),8);
            }
        }
        break;
    // rgb settings for the keyboard home row
    case FJLIGHT:
        if (record->event.pressed) {
           // update the var used for f and j home key highlighting
           fj_light = !fj_light;
        }
        break;
    case HROWLIGHT:
        if (record->event.pressed) {
           // update the var used for full home row keys highlighting
           hrow_light = !hrow_light;
        }
        break;
    case KTRACK:
        if (record->event.pressed) {
           // update the var used to enable/disable keytracker and per-key fade
           enable_keytracker = !enable_keytracker;
        }
        break;
    // mouse controls
    case MK_HOLD:
        if (record->event.pressed) {
           register_code(KC_MS_BTN1);
        }
        break;
    case MK_REL:
        if (record->event.pressed) {
           unregister_code(KC_MS_BTN1);
        }
        break;
    // mouse speed adjustment while keycode is held
    case MK_ACCEL0:
        tap_code(record->event.pressed ? KC_MS_ACCEL0 : KC_MS_ACCEL1);
        break;
    case MK_ACCEL2:
        tap_code(record->event.pressed ? KC_MS_ACCEL2 : KC_MS_ACCEL1);
        break;
    case STHRU:
        if (record->event.pressed) {
            if (wide_sthru) {
                wide_sthru = false;
                wide_firstchar = false;
            }
            else {
                wide_bartext = false;
                wide_sthru = true;
                wide_underln = false;
                wide_firstchar = true;
            }
        }
        break;
    case UNDERLN:
        if (record->event.pressed) {
            if (wide_underln) {
                wide_underln = false;
                wide_firstchar = false;
            }
            else {
                wide_bartext = false;
                wide_sthru = false;
                wide_underln = true;
                wide_firstchar = true;
            }
        }
        break;
    case BARTEXT:
        if (record->event.pressed) {
            if (wide_bartext) {
                wide_bartext = false;
                wide_firstchar = false;
            }
            else {
                wide_bartext = true;
                wide_sthru = false;
                wide_underln = false;
                wide_firstchar = true;
            }
        }
        break;
    case COLORTEST:
        if (record->event.pressed) {
            color_test_timer = timer_read();
            color_test = true;
        }
        break;
    case FLASH_KB:
        if (record->event.pressed) {
           // command to flash this keyboard
           send_string("qmk flash -kb yunzii/al68 -km djcastaldo\n");
        }
        break;
    case BOOTLDR:
        if (record->event.pressed) {
            key_timer = timer_read32();
        } else if (timer_elapsed32(key_timer) >= 500) {
            reset_keyboard();
        }
        break;
    }
    return process_record_secrets(keycode, record);
}

void dual_key(uint16_t std_keycode, uint16_t alt_keycode, uint8_t mod_mask) {
    // if mod is being held, send mod_keycode
    // get current mod states
    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    if ((mods | oneshot_mods) & mod_mask) {
        unregister_mods(mod_mask);  // remove mod
        tap_code16(alt_keycode);
        register_mods(mods); // restore original mods
    }   
    // otherwise send std_keycode
    else { 
        tap_code16(std_keycode);
    }       
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    // do the indicator bar
    switch (layer) {
    case FN_LAYR:
        rgb_matrix_set_color(I_INDICATOR, RGB_GREEN);
        rgb_matrix_set_color(I_INDICATOR2, RGB_GREEN);
        break;
    case SFT_LAYR:
        rgb_matrix_set_color(I_INDICATOR, RGB_ORANGE);
        rgb_matrix_set_color(I_INDICATOR2, RGB_ORANGE);
        break;
    case CTL_LAYR:
        rgb_matrix_set_color(I_INDICATOR, RGB_RED);
        rgb_matrix_set_color(I_INDICATOR2, RGB_RED);
        break;
    case TMUX_LAYR:
        rgb_matrix_set_color(I_INDICATOR, RGB_CYAN);
        rgb_matrix_set_color(I_INDICATOR2, RGB_CYAN);
        break;
    case WIDE_TEXT_LAYR:
        rgb_matrix_set_color(I_INDICATOR, 0x77, 0x77, 0x77);
        rgb_matrix_set_color(I_INDICATOR2, 0x77, 0x77, 0x77);
        break;
    case LOCK_LAYR:
        break;
    default:
        rgb_matrix_set_color(I_INDICATOR, RGB_BLUE);
        rgb_matrix_set_color(I_INDICATOR2, RGB_BLUE);
        break;
    }
    if (layer > 0) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];
                if (index > 2) {
                    //index = index + 2;
                }
                        
                if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                    switch (layer) {
                    case FN_LAYR:
                        if (index == I_MREC1 || index == I_MREC2) { // macro recording keys
                            rgb_matrix_set_color(index, RGB_MAGENTA);
                        }
                        else {
                            rgb_matrix_set_color(index, RGB_GREEN);
                        }
                        break;
                    case SFT_LAYR:
                        rgb_matrix_set_color(index, RGB_ORANGE);
                        break;
                    case CTL_LAYR:
                        rgb_matrix_set_color(index, RGB_RED);
                        break;
                    case TMUX_LAYR:
                        rgb_matrix_set_color(index, RGB_CYAN);
                        break;
                    case WIDE_TEXT_LAYR:
                        rgb_matrix_set_color(index, RGB_BLUE);
                        break;
                    case LOCK_LAYR:
                        break;
                    default:
                        rgb_matrix_set_color(index, 0x77, 0x77, 0x77);
                        break;
                    }
                }
            }   
        }
// ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
        // custom colors for layer keys on layer 1
        if (layer == FN_LAYR) {
          rgb_matrix_set_color(I_LSFT, RGB_ORANGE); // left shift
          rgb_matrix_set_color(I_RSFT, RGB_ORANGE); // right shift
          rgb_matrix_set_color(I_LALT, RGB_RED);    // left alt
          rgb_matrix_set_color(I_RCTL, RGB_RED);    // right ctrl
          rgb_matrix_set_color(I_TAB, RGB_CYAN);    // tab
        }  
// ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ */
        // set up the layer key blinking indicator
        if (!layer_timer) {
            is_led_on = true;
            layer_timer = timer_read();
        }
        // timer_elapsed() is a built-in function in qmk => it calculates in ms the time elapsed with timer_read()
        else if (timer_elapsed(layer_timer) > 500)
        {
            is_led_on = !is_led_on;
            layer_timer = timer_read();
        }
        if (is_led_on) 
        {
            switch (layer) {
            case FN_LAYR:
                rgb_matrix_set_color(I_CAPS, RGB_GREEN);  // caps
                rgb_matrix_set_color(I_FN, RGB_GREEN);    // fn
                break;
            case SFT_LAYR:
                rgb_matrix_set_color(I_LSFT, RGB_ORANGE); // lshift
                rgb_matrix_set_color(I_RSFT, RGB_ORANGE); // rshift
                break;
            case CTL_LAYR:
                rgb_matrix_set_color(I_LALT, RGB_RED);    // lalt
                rgb_matrix_set_color(I_RCTL, RGB_RED);    // rctrl
                break;
            case TMUX_LAYR:
                rgb_matrix_set_color(I_TAB, RGB_CYAN);    // tab
                break;
            case WIDE_TEXT_LAYR:
                rgb_matrix_set_color(I_RSFT, RGB_BLUE);   // rshift
                break;
            }
        }
        // blink the indicator to show layer lock in on
        if (is_layer_locked(layer)) {
            if (!layer_lock_timer || timer_elapsed(layer_lock_timer) > 1000) {
                is_layer_lock_led_on = !is_layer_lock_led_on;
                layer_lock_timer = timer_read();
            }
            if (is_layer_lock_led_on) {
                rgb_matrix_set_color(I_INDICATOR, 255, 255, 255); // flash the indicator
                rgb_matrix_set_color(I_INDICATOR2, 255, 255, 255);
            }
        }
    }
    else {
        rgb_matrix_set_color(I_FN, RGB_BLUE);        // fn 
        rgb_matrix_set_color(I_TAB, 0x77,0x77,0x77); // tab
        if (!host_keyboard_led_state().caps_lock) {
          rgb_matrix_set_color(I_CAPS, RGB_BLUE);    // caps
        }
        // if caps lock, color caps alpha keys
        else {
           for (uint8_t i = led_min; i < led_max; i++) {
             if (is_capslock_shifted(i)) {
                rgb_matrix_set_color(i, RGB_RED);
             }
           }
        }
        if (is_caps_word_on())
        {
          // set capitilized keys red when caps word is active
          for (uint8_t i = led_min; i < led_max; i++) {
            if (is_capsword_shifted(i)) {
               rgb_matrix_set_color(i, RGB_RED);
            }
          }
          rgb_matrix_set_color(I_LSFT, RGB_WHITE);       // turn left shift white when caps_word is active
          rgb_matrix_set_color(I_RSFT, RGB_WHITE);       // turn right shift white when caps_word is active
          rgb_matrix_set_color(I_INDICATOR, RGB_WHITE);  // indicator
          rgb_matrix_set_color(I_INDICATOR2, RGB_WHITE); // indicator
        }
        // if not caps word, nor caps lock, then do the hrow colors if turned on
        else if (!host_keyboard_led_state().caps_lock) {
          if (hrow_light) {
              rgb_matrix_set_color(I_A, RGB_GREEN);    // a
              rgb_matrix_set_color(I_S, RGB_GREEN);    // s
              rgb_matrix_set_color(I_D, RGB_GREEN);    // d
              rgb_matrix_set_color(I_F, RGB_GREEN);    // f
              rgb_matrix_set_color(I_J, RGB_GREEN);    // j
              rgb_matrix_set_color(I_K, RGB_GREEN);    // k
              rgb_matrix_set_color(I_L, RGB_GREEN);    // l
              rgb_matrix_set_color(I_SEMI, RGB_GREEN); // ;
          }
          if (fj_light) {
              rgb_matrix_set_color(I_F, RGB_WHITE); // f
              rgb_matrix_set_color(I_J, RGB_WHITE); // j
          }
        }
    }
    // if key lock is watching for next key, flash the indicator yellow and orange 
    if (is_key_lock_watching()) {
        if (!key_lock_timer || timer_elapsed(key_lock_timer) > 500) {
            is_key_lock_led_on = !is_key_lock_led_on;;
            key_lock_timer = timer_read();
        }
        if (is_key_lock_led_on) {
            rgb_matrix_set_color(I_ESC, RGB_YELLOW);        // esc
            rgb_matrix_set_color(I_INDICATOR, RGB_YELLOW);  // indicator
            rgb_matrix_set_color(I_INDICATOR2, RGB_YELLOW); // indicator
        }
        else {
            rgb_matrix_set_color(I_ESC, RGB_ORANGE);        // esc
            rgb_matrix_set_color(I_INDICATOR, RGB_ORANGE);  // indicator
            rgb_matrix_set_color(I_INDICATOR2, RGB_ORANGE); // indicator
        }
    }
    // if key lock is activated, flash the indicator white and red 
    else if (is_key_locked) {
        if (!key_lock_timer || timer_elapsed(key_lock_timer) > 500) {
            is_key_lock_led_on = !is_key_lock_led_on;;
            key_lock_timer = timer_read();
        }
        if (is_key_lock_led_on) {
            rgb_matrix_set_color(I_INDICATOR, RGB_WHITE);   // indicator
            rgb_matrix_set_color(I_INDICATOR2, RGB_WHITE);  // indicator
        }
        else {
            rgb_matrix_set_color(I_INDICATOR, RGB_RED);     // indicator
            rgb_matrix_set_color(I_INDICATOR2, RGB_RED);    // indicator
        }
        // and light up normal modifiers if they are locked
        if (lctl_locked) {
            rgb_matrix_set_color(I_LCTL, 255, 255, 255);   // lctl
        }
        if (lalt_locked) {
            rgb_matrix_set_color(I_LALT, 255, 255, 255);   // lalt
        }
        if (lsft_locked) {
            rgb_matrix_set_color(I_LSFT, 255, 255, 255);   // lsft
        }
        if (lgui_locked) {
            rgb_matrix_set_color(I_LGUI, 255, 255, 255);   // lgui
        }
        if (rsft_locked) {
            rgb_matrix_set_color(I_RSFT, 255, 255, 255);   // rsft
        }
        if (rctl_locked) {
            rgb_matrix_set_color(I_RCTL, 255, 255, 255);    // rctl
        }
    }
    if (is_in_leader_sequence) {
        if (!leader_timer || timer_elapsed(leader_timer) > 500) {
            is_leader_led_on = !is_leader_led_on;
            leader_timer = timer_read();
        } 
        if (is_leader_led_on) {
            rgb_matrix_set_color(I_ESC, RGB_TURQUOISE);        // esc
            rgb_matrix_set_color(I_INDICATOR, RGB_TURQUOISE);  // led indicator 
            rgb_matrix_set_color(I_INDICATOR2, RGB_TURQUOISE); // led indicator 
        }           
        else {
            rgb_matrix_set_color(I_ESC, RGB_CYAN);
            rgb_matrix_set_color(I_INDICATOR, RGB_CYAN);  
            rgb_matrix_set_color(I_INDICATOR2, RGB_CYAN);  
        }  
    }
    // if a leader sequence error occured, blink all leds red
    else if (is_leader_error) {
        if (!leader_error_timer || timer_elapsed(leader_error_timer) > 250) {
            is_leader_error_led_on = !is_leader_error_led_on;
            leader_error_timer = timer_read();
        }
        if (is_leader_error_led_on) {
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, RGB_RED);
            }
        }
    }

    // if any rgb key highlights are on, turn the setting keys white on layer CTL_LAYR
    if (layer == CTL_LAYR)
    {
        if (fj_light) {
            rgb_matrix_set_color(I_FJLIGHT, RGB_WHITE);     // home (fj highlight key)
        }
        if (hrow_light) {
            rgb_matrix_set_color(I_HROWLIGHT, RGB_WHITE);   // end (hrow highlight key)
        }
        if (enable_keytracker) {
            rgb_matrix_set_color(I_SEMI, RGB_WHITE);        // semi (keytracker set key)
        }
    }

    // calculate the reactive rgb for keypresses
    if (enable_keytracker) {
        for (int i = 0; i < tk_length; i++) {
            if (tracked_keys[i].press) {
                rgb_matrix_set_color(tracked_keys[i].index, 255, 255, 255);
            }
            // do the key fade if key should fade
            // this is a modified fade to get a smoother look and be mostly white but a bit blue
            else if (key_should_fade(tracked_keys[i], layer)) {
                if (tracked_keys[i].fade > 255) {
                    rgb_matrix_set_color(tracked_keys[i].index, 255, 255, 255);
                }
                else if (tracked_keys[i].fade > 230) {
                    rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, 255);
                } else {
                    rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, tracked_keys[i].fade + 25);
                } 
            }
        }
    }
    if (macro_recording) {
        // flash the indicator if macro is recording
        if (timer_elapsed(macro_timer) > 250) {
            is_macro_led_on = !is_macro_led_on;;
            macro_timer = timer_read();
        }
        if (is_macro_led_on) {
            rgb_matrix_set_color(I_INDICATOR, RGB_RED);  // indicator
            rgb_matrix_set_color(I_INDICATOR2, RGB_RED); // indicator
            rgb_matrix_set_color(I_ESC, RGB_RED);        // esc 
            if (layer == FN_LAYR) {
                if (macro_direction == 1) {
                    rgb_matrix_set_color(I_MREC1, RGB_RED);   // [ 
                }
                else {
                    rgb_matrix_set_color(I_MREC2, RGB_RED);   // ] 
                }
            }        
        }
        else {
            rgb_matrix_set_color(I_INDICATOR, 0, 0, 0);  // indicator
            rgb_matrix_set_color(I_INDICATOR2, 0, 0, 0); // indicator
            rgb_matrix_set_color(I_ESC, 0, 0, 0);        // esc
            if (layer == FN_LAYR) {
                if (macro_direction == 1) {
                    rgb_matrix_set_color(I_MREC1, 0, 0, 0);   // [ 
                }
                else {
                    rgb_matrix_set_color(I_MREC2, 0, 0, 0);   // ] 
                }
            }        
        }
    }
    // if this is a color test, then light keys in each qmk named color
    if (color_test) {
        // turn off the color test if it has been going for 15 seconds
        if (timer_elapsed(color_test_timer) > 15000) {
            color_test = false;
        }
        else { // show the named colors defined in color.h, https://docs.qmk.fm/features/rgb_matrix
            rgb_matrix_set_color(I_Q, RGB_AZURE);
            rgb_matrix_set_color(I_W, RGB_BLACK);
            rgb_matrix_set_color(I_E, RGB_BLUE);
            rgb_matrix_set_color(I_R, RGB_CHARTREUSE);
            rgb_matrix_set_color(I_T, RGB_CORAL);
            rgb_matrix_set_color(I_Y, RGB_CYAN);
            rgb_matrix_set_color(I_U, RGB_GOLD);
            rgb_matrix_set_color(I_I, RGB_GOLDENROD);
            rgb_matrix_set_color(I_O, RGB_GREEN);
            rgb_matrix_set_color(I_P, RGB_MAGENTA);
            rgb_matrix_set_color(I_A, RGB_ORANGE);
            rgb_matrix_set_color(I_S, RGB_PINK);
            rgb_matrix_set_color(I_D, RGB_PURPLE);
            rgb_matrix_set_color(I_F, RGB_RED);
            rgb_matrix_set_color(I_G, RGB_SPRINGGREEN);
            rgb_matrix_set_color(I_H, RGB_TEAL);
            rgb_matrix_set_color(I_J, RGB_TURQUOISE);
            rgb_matrix_set_color(I_K, RGB_WHITE);
            rgb_matrix_set_color(I_L, RGB_YELLOW);
        }
    }
    // track mode keys on WIDE_TEXT_LAYR
    if (layer == WIDE_TEXT_LAYR) {
        if (wide_bartext) {
            rgb_matrix_set_color(I_BARTEXT, 0x77, 0x77, 0x77);  // bartext toggle
        }
        else if (wide_sthru) {
            rgb_matrix_set_color(I_STHRU, 0x77, 0x77, 0x77);    // sthru toggle
        }
        else if (wide_underln) {
            rgb_matrix_set_color(I_UNDERLN, 0x77, 0x77, 0x77);  // underln toggle
        }
    }
    // track caps_lock
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(I_CAPS, RGB_WHITE);         // caps
        rgb_matrix_set_color(I_INDICATOR, RGB_WHITE);    // indicator
        rgb_matrix_set_color(I_INDICATOR2, RGB_WHITE);   // indicator
    }
    // track num_lock
    if (layer == SFT_LAYR && host_keyboard_led_state().num_lock) {
        rgb_matrix_set_color(I_NUMLOCK, RGB_WHITE);
    }
    // track scroll_lock
    if (layer == FN_LAYR && host_keyboard_led_state().scroll_lock) {
        rgb_matrix_set_color(I_SLOCK, RGB_WHITE);
    }

    return false;
}
// a function to check for if a key press should fade the rgb
bool key_should_fade(keytracker key, uint8_t layer) {
    bool should_fade = true;
    if ((key.fade < 1) || 
      ((layer == FN_LAYR || layer == SFT_LAYR || layer == WIDE_TEXT_LAYR || is_caps_word_on()) &&
        (key.index == I_LSFT || key.index == I_RSFT)) ||                                           // left and right shift
      (layer == FN_LAYR && (key.index == I_LALT || key.index == I_RCTL)) ||                        // left alt and right ctrl
      (layer == SFT_LAYR && key.index == I_NUMLOCK) ||                                             // num lock key
      (layer == FN_LAYR && key.index == I_SLOCK) ||                                                // scroll lock
      (layer == WIDE_TEXT_LAYR &&
        (key.index == I_BARTEXT || key.index == I_STHRU || key.index == I_UNDERLN)) ||             // wide-text mode toggles
      (layer == CTL_LAYR && (key.index == I_FJLIGHT || key.index == I_HROWLIGHT)) ||               // fj/hrow light keys 
      (layer == BASE_LAYR && key.index == I_FN) ||                                                 // fn on base 
      (key.index == I_CAPS || key.index == I_TAB)) {                                               // caps lock and tab
        should_fade = false;
    }
    return should_fade;
}

//Determine the current tap dance state
int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed) {
      return SINGLE_TAP;
    } else {
      return SINGLE_HOLD;
    }
  } else if (state->count == 2) {
    return DOUBLE_TAP;
  } else if (state->count == 3) {
    return TRIPLE_TAP; 
  } else if (state->count == 4) {
    return QUAD_TAP;
  } else if (state->count == 5) {
    return PENT_TAP;
  }
  else return 8;
}

//Initialize tap structure associated with caps tap dance key
static tap caps_tap_state = {
  .is_press_action = true,
  .state = 0 
};
static tap fn_tap_state = {
  .is_press_action = true,
  .state = 0 
};
static tap rctl_tap_state = {
  .is_press_action = true,
  .state = 0 
};
static tap rsft_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap kbunlock_tap_state = {
  .is_press_action = true,
  .state = 0
};

// caps tap dance key function
void caps_finished (tap_dance_state_t *state, void *user_data) {
  caps_tap_state.state = cur_dance(state);
  switch (caps_tap_state.state) {
    case SINGLE_TAP:
      tap_code(KC_CAPS);
      break;
    case SINGLE_HOLD:
      layer_on(FN_LAYR);
      break;
    case DOUBLE_TAP:
      if (layer_state_is(FN_LAYR)) {
        //if already set, then switch it off
        layer_lock_off(FN_LAYR);
      } else {
        //if not already set, then switch the layer on
        layer_lock_on(FN_LAYR);
      }
      break;
    case TRIPLE_TAP:
      if (layer_state_is(SFT_LAYR)) {
        //if already set, then switch it off
        layer_lock_off(SFT_LAYR);
      } else {
        //if not already set, then switch the layer on
        layer_lock_on(SFT_LAYR);
      }
      break;
    case QUAD_TAP:
      if (layer_state_is(CTL_LAYR)) {
        //if already set, then switch it off
        layer_lock_off(CTL_LAYR);
      } else {
        //if not already set, then switch the layer on
        layer_lock_on(CTL_LAYR);
      }
      break;
    case PENT_TAP:
      if (layer_state_is(TMUX_LAYR)) {
        //if already set, then switch it off
        layer_lock_off(TMUX_LAYR);
      } else {
        //if not already set, then switch the layer on
        layer_lock_on(TMUX_LAYR);
      }
      break;
  }
}
void caps_reset (tap_dance_state_t *state, void *user_data) {
  //if the key was held down and now is released then switch off the layer
  if (caps_tap_state.state==SINGLE_HOLD && !is_layer_locked(FN_LAYR)) {
    layer_off(FN_LAYR);
  }
  caps_tap_state.state = 0;
}

// function for fn tap dance
void fn_finished (tap_dance_state_t *state, void *user_data) {
  fn_tap_state.state = cur_dance(state);
  switch (fn_tap_state.state) {
    case SINGLE_TAP:
      set_oneshot_layer(FN_LAYR, ONESHOT_START);
      clear_oneshot_layer_state(ONESHOT_PRESSED);
      break;
    case SINGLE_HOLD:
      layer_on(FN_LAYR);
      break;
  }
}
void fn_reset (tap_dance_state_t *state, void *user_data) {
  switch (fn_tap_state.state) {
    case SINGLE_TAP:
      break;
    case SINGLE_HOLD:
      layer_off(FN_LAYR);
      break;
  }
  fn_tap_state.state = 0;
}

// function for rctl tap dance
void rctl_finished (tap_dance_state_t *state, void *user_data) {
  rctl_tap_state.state = cur_dance(state);
  switch (rctl_tap_state.state) {
    case SINGLE_TAP:
      set_oneshot_layer(CTL_LAYR, ONESHOT_START);
      clear_oneshot_layer_state(ONESHOT_PRESSED);
      break;
    case SINGLE_HOLD:
      register_code(KC_RCTL);
      break;
  }
}
void rctl_reset (tap_dance_state_t *state, void *user_data) {
  switch (rctl_tap_state.state) {
    case SINGLE_TAP:
      break;
    case SINGLE_HOLD:
      unregister_code(KC_RCTL);
      break;
  }
  rctl_tap_state.state = 0;
}

// function for each press of rsft
void rsft_each(tap_dance_state_t *state, void *user_data) {
    if (get_mods() & MOD_BIT(KC_LSFT)) {
        caps_word_on();
    }
}
// function for rsft tap dance
void rsft_finished (tap_dance_state_t *state, void *user_data) {
  rsft_tap_state.state = cur_dance(state);
  switch (rsft_tap_state.state) {
    case SINGLE_TAP:
      // check if this is caps word activation, otherwise set the osl
      if (!is_caps_word_on()) {
          set_oneshot_layer(SFT_LAYR, ONESHOT_START);
          clear_oneshot_layer_state(ONESHOT_PRESSED);
      }
      break;
    case DOUBLE_TAP:
      // activate WIDE_TEXT_LAYR
      if (IS_LAYER_ON(WIDE_TEXT_LAYR)) {
          layer_lock_off(WIDE_TEXT_LAYR);
      }
      else {
          layer_lock_on(WIDE_TEXT_LAYR);
      }
      break;
    case SINGLE_HOLD:
      // check if this is caps word activation, otherwise regular shift
      if (get_mods() & MOD_BIT(KC_LSFT)) {
          caps_word_on();
      }
      else {
          register_code(KC_RSFT);
      }
      break;
  }
}
void rsft_reset (tap_dance_state_t *state, void *user_data) {
  switch (rsft_tap_state.state) {
    case SINGLE_TAP:
      // check if this is caps word activation
      if (get_mods() & MOD_BIT(KC_LSFT)) {
          reset_oneshot_layer();
          caps_word_on();
      }
      break;
    case DOUBLE_TAP:
      break;
    case SINGLE_HOLD:
      if (get_mods() & MOD_BIT(KC_LSFT)) {
          caps_word_on();
      }
      else {
          unregister_code(KC_RSFT);
      }
      break;
  }
  rsft_tap_state.state = 0;
}

 // function for kbunlock tap dance
 void kbunlock_finished (tap_dance_state_t *state, void *user_data) {
   kbunlock_tap_state.state = cur_dance(state);
   switch (kbunlock_tap_state.state) {
     case SINGLE_TAP:
       break;
     case DOUBLE_TAP:
       break;
     case TRIPLE_TAP:
       layer_off(LOCK_LAYR);
       break;
     case SINGLE_HOLD:
       break;
   }
 }
 void kbunlock_reset (tap_dance_state_t *state, void *user_data) {
   kbunlock_tap_state.state = 0;
 }

// associate the tap dance keys with their funcitons  
tap_dance_action_t tap_dance_actions[] = { 
  [CAPS_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
  [FN_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_finished, fn_reset),
  [RCTL_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rctl_finished, rctl_reset),
  [RSFT_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(rsft_each, rsft_finished, rsft_reset),
  [KB_UNLOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, kbunlock_finished, kbunlock_reset),
};

// this is needed to prevent CAPS_WORD from breaking when some custom key commands are used
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case BSPCFAST:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

// setup to store vars when macro recording starts or ends. then can flash some rgb
bool dynamic_macro_record_start_user(int8_t direction) {
    macro_direction = direction;
    macro_recording = true;
    macro_timer = timer_read();
    return true; 
}
bool dynamic_macro_record_end_user(int8_t direction) {
    macro_direction = direction;
    macro_recording = false;
    is_macro_led_on = false;
    for (int i = 0; i < tk_length; i++) {
        if (tracked_keys[i].index == I_MREC1 || tracked_keys[i].index == I_MREC2) {
            tracked_keys[i].press = false;
            tracked_keys[i].fade = 0;
        }
    }
    return true;
}
// this is so the macro key lights don't get stuck when i play the macro
bool dynamic_macro_play_user(int8_t direction) {
    for (int i = 0; i < tk_length; i++) {
        if (tracked_keys[i].index == I_MPLY1 || tracked_keys[i].index == I_MPLY2) {
            tracked_keys[i].press = false;
            tracked_keys[i].fade = 0;
        }
    }
    return true;
}

void oneshot_layer_changed_user(uint8_t layer) {
    if (layer > 0) {
        oneshot_layer_active = true; 
    }
    if (!layer) {
        oneshot_layer_active = false;
    }
}

void leader_start_user(void) {
    is_in_leader_sequence = true;
}    

void leader_end_user(void) {
    if (leader_sequence_three_keys(KC_L, KC_L, KC_S)) {        // layer lock SFT_LAYR 
        if (is_layer_locked(SFT_LAYR)) {
            layer_lock_off(SFT_LAYR);
        }
        else {
            layer_lock_on(SFT_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_C)) {   // layer lock CTL_LAYR 
        if (is_layer_locked(CTL_LAYR)) {
            layer_lock_off(CTL_LAYR);
        }
        else {
            layer_lock_on(CTL_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_F)) {   // layer lock FN_LAYR 
        if (is_layer_locked(FN_LAYR)) {
            layer_lock_off(FN_LAYR);
        }
        else {
            layer_lock_on(FN_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_T)) {   // layer lock TMUX_LAYR
        if (is_layer_locked(TMUX_LAYR)) {
            layer_lock_off(TMUX_LAYR);
        }
        else {
            layer_lock_on(TMUX_LAYR);
        }
    }
    else if (leader_sequence_four_keys(KC_L, KC_O, KC_C, KC_K)) {   // switch to LOCK_LAYR 
        layer_on(LOCK_LAYR);
    }
    else if (leader_sequence_two_keys(KC_L, KC_K)) {          // set key lock watch
        set_key_lock_watching();
    }
    else if (leader_sequence_three_keys(KC_P, KC_O, KC_C)) {  // insert test POC data for template 
        send_string_with_delay("Firstname" SS_TAP(X_TAB) "Lastname" SS_TAP(X_TAB) "123-456-7890" SS_TAP(X_TAB) "first.last@mail.mil" SS_TAP(X_TAB),10);
    }
    else if (leader_sequence_two_keys(KC_N, KC_1)) {          // mac mini start nut upsdrvctl
        SEND_STRING("sudo /usr/local/opt/nut/sbin/upsdrvctl start" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_N, KC_2)) {          // mac mini start nut upsd
        SEND_STRING("sudo /usr/local/opt/nut/sbin/upsd" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_N, KC_R)) {          // restart nut
        SEND_STRING("sudo service nut-server restart" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_U, KC_R)) {          // restart unifi
        SEND_STRING("sudo service unifi restart" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_U, KC_U)) {          // update unifi
        SEND_STRING("wget https://get.glennr.nl/unifi/update/unifi-update.sh && sudo bash unifi-update.sh --custom-url " SS_LCMD(SS_TAP(X_V))      SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_V, KC_H)) {          // open hosts file in vi
        SEND_STRING("sudo vi /etc/hosts" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_V, KC_A)) {          // open tb authorized in vi
        SEND_STRING("sudo vi /sys/bus/thunderbolt/devices/0-3/authorized" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_S, KC_W)) {          // select word
        SEND_STRING(SS_LCTL(SS_TAP(X_LEFT) SS_LSFT(SS_TAP(X_RIGHT))));
    }
    else if (leader_sequence_two_keys(KC_S, KC_L)) {          // select line
        SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END)));
    }
    else if (leader_sequence_two_keys(KC_T, KC_Y)) {          // thank you
        SEND_STRING("thank you");
    }
    else if (leader_sequence_two_keys(KC_N, KC_P)) {          // no problem
        SEND_STRING("no problem");
    }
    else if (leader_sequence_three_keys(KC_O, KC_M, KC_W)) {  // on my way
        SEND_STRING("on my way");
    }
    else if (leader_sequence_three_keys(KC_B, KC_R, KC_B)) {  // be right back
        SEND_STRING("be right back");
    }
    else if (leader_sequence_three_keys(KC_H, KC_G, KC_E)) {  // have a good evening
        SEND_STRING("have a good evening");
    }
    else if (leader_sequence_three_keys(KC_H, KC_G, KC_N)) {  // have a good night
        SEND_STRING("have a good night");
    }
    else if (leader_sequence_four_keys(KC_T, KC_T, KC_Y, KC_L)) {  // talk to you later
        SEND_STRING("talk to you later");
    }
    else if (leader_sequence_five_keys(KC_C, KC_O, KC_L, KC_O, KC_R)) {  // start the color test
        color_test_timer = timer_read();
        color_test = true;
    }
    else if (leader_sequence_four_keys(KC_G, KC_I, KC_T, KC_L)) {        // git log
        SEND_STRING("git log\n");
    }
    else if (leader_sequence_four_keys(KC_G, KC_I, KC_T, KC_A)) {        // git add
        SEND_STRING("git add -A\n");
    }
    else if (leader_sequence_four_keys(KC_G, KC_I, KC_T, KC_C)) {        // git commit
        SEND_STRING("git commit -m \"\"" SS_TAP(X_LEFT));
    }
    else if (leader_sequence_four_keys(KC_G, KC_I, KC_T, KC_P)) {        // git push
        SEND_STRING("git push\n");
    }
    else if (leader_sequence_five_keys(KC_G, KC_I, KC_T, KC_C, KC_O)) {  // git checkout .
        SEND_STRING("git checkout .\n");
    }
    else if (leader_sequence_three_keys(KC_Q, KC_C, KC_B)) {  // qmk compile shortcutstudio bridge75 firmware
        SEND_STRING("qmk compile -kb shortcut/bridge75 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_F, KC_B)) {  // qmk flash shortcutstudio bridge75 firmware
        SEND_STRING("qmk flash -kb shortcut/bridge75 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_C, KC_Y)) {  // qmk compile yunzii firmware
        SEND_STRING("qmk compile -kb yunzii/al68 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_F, KC_Y)) {  // qmk flash yunzii firmware
        SEND_STRING("qmk flash -kb yunzii/al68 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_C, KC_L)) {  // qmk compile lemokey p1 firmware
        SEND_STRING("qmk compile -kb lemokey/p1_pro/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_F, KC_L)) {  // qmk flash lemokey p1 firmware
        SEND_STRING("qmk flash -kb lemokey/p1_pro/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_C, KC_K, KC_V)) {  // qmk compile keychron V6 firmware
        SEND_STRING("qmk compile -kb keychron/v6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_F, KC_K, KC_V)) {  // qmk flash keychron V6 firmware
        SEND_STRING("qmk flash -kb keychron/v6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_C, KC_K, KC_Q)) {  // qmk compile keychron Q6 firmware
        SEND_STRING("qmk compile -kb keychron/q6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_F, KC_K, KC_Q)) {  // qmk flash keychron Q6 firmware
        SEND_STRING("qmk flash -kb keychron/q6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_five_keys(KC_A, KC_S, KC_P, KC_D, KC_C)) {  // asp.net decrypt connectionStrings path from clipboard
        send_string_with_delay("C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\aspnet_regiis -pdf connectionStrings " SS_LCTL(SS_TAP(X_V)) SS_TAP(X_ENT),9);
    }
    else if (leader_sequence_five_keys(KC_A, KC_S, KC_P, KC_E, KC_C)) {  // asp.net encrypt connectionStrings path from clipboard
        send_string_with_delay("C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\aspnet_regiis -pef connectionStrings " SS_LCTL(SS_TAP(X_V)) SS_TAP(X_ENT),9);
    }
    else if (leader_sequence_five_keys(KC_A, KC_S, KC_P, KC_D, KC_S)) {  // asp.net decrypt sessionState path from clipboard
        send_string_with_delay("C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\aspnet_regiis -pdf system.web/sessionState " SS_LCTL(SS_TAP(X_V)) SS_TAP(X_ENT),9);
    }
    else if (leader_sequence_five_keys(KC_A, KC_S, KC_P, KC_E, KC_S)) {  // asp.net encrypt sessionState path from clipboard
        send_string_with_delay("C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\aspnet_regiis -pef system.web/sessionState " SS_LCTL(SS_TAP(X_V)) SS_TAP(X_ENT),9);
    }
    // if process_leader_secrets returns false, a secret leader sequence ran and leader matching should stop
    // if true, no secrets match and leader matching continues here.
    else if (process_leader_secrets()) {
        // leader sequence failed, so blink the rgb red a few times
        is_leader_error = true;
        leader_error_token = defer_exec(1500, leader_error_callback, NULL);  // Schedule callback.
    }

    is_in_leader_sequence = false;
}

