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
    SYMBOL_LAYR,
    WIDE_TEXT_LAYR,
    CIRCLE_TEXT_LAYR,
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
    OPT2,
    OPT3,
    OPT4,
    OPT5,
    OPT6,
    OPT7,
    OPT8,
    OPT9,
    OPT0,
    OPTMIN,
    OPTEQ,
    OPTQ,
    OPTW,
    OPTR,
    OPTT,
    OPTY,
    OPTO,
    OPTP,
    OPTLBR,
    OPTRBR,
    OPTBSL,
    OPTA,
    OPTS,
    OPTD,
    OPTF,
    OPTG,
    OPTH,
    OPTJ,
    OPTK,
    OPTL,
    OPTSEM,
    OPTAPO,
    OPTZ,
    OPTX,
    OPTC,
    OPTV,
    OPTB,
    OPTM,
    OPTCOM,
    OPTDOT,
    OPTSLS,
    CIRCL1,
    CIRCL2,
    CIRCL3,
    CIRCL4,
    CIRCL5,
    CIRCL6,
    CIRCL7,
    CIRCL8,
    CIRCL9,
    CIRCL0,
    CIRCLQ,
    CIRCLW,
    CIRCLE,
    CIRCLR,
    CIRCLT,
    CIRCLY,
    CIRCLU,
    CIRCLI,
    CIRCLO,
    CIRCLP,
    CIRCLA,
    CIRCLS,
    CIRCLD,
    CIRCLF,
    CIRCLG,
    CIRCLH,
    CIRCLJ,
    CIRCLK,
    CIRCLL,
    CIRCLZ,
    CIRCLX,
    CIRCLC,
    CIRCLV,
    CIRCLB,
    CIRCLN,
    CIRCLM,
    STHRU,
    UNDERLN,
    BARTEXT,
    BBRTEXT,
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
    ACT_GRV = 4,
    ACT_1   = 5,
    ACT_E   = 6,
    ACT_U   = 7,
    ACT_I   = 8,
    ACT_N   = 9,
    LGUI_OSL = 10,
    KB_UNLOCK = 11 
}; 

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE_LAYR]        =  { ENCODER_CCW_CW(ENC_VOLD, ENC_VOLU) },
    [FN_LAYR]          =  { ENCODER_CCW_CW(DUAL_ZOOMO, DUAL_ZOOMI) },
    [SFT_LAYR]         =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN) },
    [CTL_LAYR]         =  { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
    [TMUX_LAYR]        =  { ENCODER_CCW_CW(ENC_TSIZEL, ENC_TSIZER) },
    [SYMBOL_LAYR]      =  { ENCODER_CCW_CW(ENC_VOLD, ENC_VOLU) },
    [WIDE_TEXT_LAYR]   =  { ENCODER_CCW_CW(ENC_VOLD, ENC_VOLU) },
    [CIRCLE_TEXT_LAYR] =  { ENCODER_CCW_CW(ENC_VOLD, ENC_VOLU) },
    [LOCK_LAYR]        =  { ENCODER_CCW_CW(KC_NO, KC_NO) }
};
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//  [BASE_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |D-ESC || 1    || 2    || 3    || 4    || 5    || 6    || 7    || 8    || 9    || 0    || -    || =    || Backspace    |: Vol  :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  | LT(TMUX) || Q    || W    || E    || R    || T    || Y    || U    || I    || O    || P    || [    || ]    || \        || Del  |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  | CAPS / Fn   || A    || S    || D    || F    || G    || H    || J    || K    || L    || ;    || '    || Enter         || PgUp |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  | Shift           || Z    || X    || C    || V    || B    || N    || M    || ,    || .    || /    ||TD(SFT_OSL)|| Up   || End  |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  | Ctrl    ||TD(GUI) || Alt     ||                   Space                      ||TD(FN_O) ||TD(CTL_O)|  | Left || Down || Rght |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [BASE_LAYR] = LAYOUT_65_ansi_blocker(
        DUAL_ESC, KC_1,  KC_2,   KC_3,  KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0, KC_MINS,  KC_EQL,  BSPCFAST, ENC_DUALPUSH,
        LT(TMUX_LAYR,KC_TAB), KC_Q, KC_W, KC_E, KC_R, KC_T,  KC_Y,  KC_U,   KC_I,   KC_O,   KC_P,  KC_LBRC, KC_RBRC, KC_BSLS, KC_DELETE,
        TD(CAPS_LAYR), KC_A,   KC_S,  KC_D,    KC_F,   KC_G,   KC_H,  KC_J,   KC_K,    KC_L,  KC_SCLN, KC_QUOT,  KC_ENT,     KC_PAGE_UP,
        KC_LSFT,       KC_Z,  KC_X,  KC_C,   KC_V,   KC_B,   KC_N,   KC_M,  KC_COMM, KC_DOT, KC_SLSH, TD(RSFT_OSL), KC_UP, KC_PAGE_DOWN,
        KC_LCTL, TD(LGUI_OSL), KC_LALT,                  KC_SPC,                   TD(FN_OSL), TD(RCTL_OSL), KC_LEFT, KC_DOWN, KC_RIGHT
    ),
//  [FN_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |  `   || F1   || F2   || F3   || F4   || F5   || F6   || F7   || F8   || F9   || F10  || F11  || F12  ||              |: Zoom :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |MO(TMUX_L)||WMail2||WMail1||Mail  ||RPhone||WPhone||      ||      ||      ||      ||      ||MRec1 ||MRec2 ||          || Ins  |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||Addr  ||City  ||ZIP   ||Phone ||      ||      ||      ||      || Lead ||SLock || PScr ||               || Home |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  | MO(SFT_LAYR)    ||      ||Rout  ||Acct  ||      ||      ||      ||      ||MPly1 ||MPly2 ||Pause || MO(SFT_L) ||      || End  |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |         ||        ||MO(CTL_L)||                                              ||         ||MO(CTL_L)|  |      ||      ||      |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [FN_LAYR] = LAYOUT_65_ansi_blocker(
        KC_GRV, KC_F1, KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,   KC_F7,  KC_F8,  KC_F9,  KC_F10,  KC_F11, KC_F12,    _______,    F_ZOOMR,
        MO(TMUX_LAYR),SECRET3,SECRET2,SECRET1,SECRET8,SECRET9,_______,_______,_______,_______,_______,DM_REC1,DM_REC2, _______,  KC_INS,
        _______,SECRET4,SECRET5,SECRET6,SECRET7,_______,_______,_______,_______,    QK_LEAD,  KC_SCRL, KC_PSCR,     _______,    KC_HOME,
        MO(SFT_LAYR), _______,SECRET10,SECRET11,_______,_______,_______,_______,DM_PLY1,DM_PLY2, KC_PAUS, MO(SFT_LAYR), _______, KC_END,
        _______, MO(SYMBOL_LAYR), MO(CTL_LAYR),              _______,                 _______,  MO(CTL_LAYR), _______, _______, _______
    ),
//  [SFT_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |  ~   ||MyComp||      || Calc || Menu ||      ||      || MPrv || MPly || MNxt ||      ||Arrow || Wave ||              |:Scroll:  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |          ||      ||      ||      ||      ||      ||      ||  P7  ||  P8  ||  P9  || +  - ||      ||      ||  NumLock ||MSRel |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||      ||      ||      ||      ||      ||      ||  P4  ||  P5  ||  P6  || *  / ||      ||               ||MSHold|  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |                 ||MSAcl2||      ||      ||      ||      ||      ||  P1  ||  P2  ||  P3  ||      ||           || MSUp || RClk |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |  LCtl   ||        || MSAcl0  ||                     P0                       ||   P.    ||  LClk   |  |MSLft || MSDn ||MSRgt |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [SFT_LAYR] = LAYOUT_65_ansi_blocker(
        KC_TILD, KC_MYCM, _______, KC_CALC, KC_APP, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, ARROW, WAVE,  _______, JIGGLE,
        _______, _______,_______,_______, _______, _______, _______, KC_P7, KC_P8, KC_P9, DUAL_PLUSMIN,_______,_______,  KC_NUM, MK_REL,
        _______,    _______,_______,_______,_______,_______, _______, KC_P4, KC_P5, KC_P6, DUAL_MULTDIV, _______,   _______,    MK_HOLD,
        _______,   MK_ACCEL2,_______,_______,_______,_______,_______,  KC_P1, KC_P2, KC_P3,  _______,   _______,   KC_MS_UP, KC_MS_BTN2,
        _______,  _______, MK_ACCEL0,                  KC_P0,                  KC_PDOT, KC_MS_BTN1, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT
    ),
//  [CTL_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||Ctest ||Debug ||   RGBTogg    |: RGB  :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |          ||      ||      ||      ||Reboot||      ||      ||      ||      ||      ||      ||      ||      || RGBMode  ||      |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||      ||      ||      ||Flash ||      ||      ||      ||      ||      ||KTrack||FJLite||  HRowLight    ||RHue+ |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |                 ||      ||      || EClr ||      ||BootLd||      ||      ||      ||      ||      ||           ||RBri+ ||RHue- |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |         ||        ||         ||                                              ||         ||         |  |RSpd- ||RBri- ||RSpd+ |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [CTL_LAYR] = LAYOUT_65_ansi_blocker(
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,COLORTEST,DB_TOGG,RGB_TOG, ENC_RGBRESET,
        _______,_______,_______,_______, QK_RBT, _______,_______,_______,_______,_______,_______,_______, _______,  RGB_MOD,    _______,
        _______, _______,_______,_______, FLASH_KB, _______,_______,_______,_______,_______, KTRACK, FJLIGHT,    HROWLIGHT,     RGB_HUI,
        _______,       _______,_______, EE_CLR, _______,  BOOTLDR, _______,_______,_______,_______,_______,   _______, RGB_VAI, RGB_HUD,
        _______, _______, _______,                       _______,                      _______, _______,      RGB_SPD, RGB_VAD, RGB_SPI
    ),
//  [TMUX_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |      ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||              |:Resize:  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |          ||LTRANS||LTRANS||      ||      ||LTRANS||      ||      ||      ||LTRANS||LTRANS||LTRANS||LTRANS||          ||      |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||      ||LTRANS||LTRANS||      ||      ||      ||      ||      ||LTRANS||LTRANS||LTRANS||               ||LTRANS|  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |                 ||LTRANS||LTRANS||LTRANS||      ||      ||LTRANS||      ||LTRANS||LTRANS||LTRANS||           ||LTRANS||      |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  | Ctrl    ||        ||         ||                    LTRANS                    ||         ||  Ctrl   |  |LTRANS||LTRANS||LTRANS|  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [TMUX_LAYR] = LAYOUT_65_ansi_blocker(
        _______, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,  LTRANS,     _______, ENC_TMON,
        _______, LTRANS, LTRANS,_______,_______,  LTRANS,_______,_______,_______, LTRANS, LTRANS, LTRANS,  LTRANS,  _______,    _______,
        _______, _______, LTRANS, LTRANS,_______,_______,_______,_______,_______, LTRANS, LTRANS, LTRANS,           _______,     LTRANS,
        _______,        LTRANS, LTRANS, LTRANS, _______,  _______,  LTRANS,_______, LTRANS, LTRANS, LTRANS,   _______,  LTRANS, _______,
        KC_LCTL, _______, _______,                        LTRANS,                      _______, KC_RCTL,       LTRANS,  LTRANS,  LTRANS
    ),
//  [SYMBOL_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |TD(G) ||TD(1) ||OPT2  ||OPT3  ||OPT4  ||OPT5  ||OPT6  ||OPT7  ||OPT8  ||OPT9  ||OPT0  ||OPTMIN||OPTEQ ||              |: Vol  :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |          ||OPTQ  ||OPTW  ||TD(E) ||OPTR  ||OPTT  ||OPTY  ||TD(U) ||TD(I) ||OPTO  ||OPTP  ||OPTLBR||OPTRBR||OPTBSL    ||      |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||OPTA  ||OPTS  ||OPTD  ||OPTF  ||OPTG  ||OPTH  ||OPTJ  ||OPTK  ||OPTL  ||OPTSEM||OPTAPO||               ||      |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |                 ||OPTZ  ||OPTX  ||OPTC  ||OPTV  ||OPTB  ||TD(N) ||OPTM  ||OPTCOM||OPTDOT||OPTSLS||           ||      ||      |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |         ||        ||         ||                                              ||         ||         |  |      ||      ||      |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [SYMBOL_LAYR] = LAYOUT_65_ansi_blocker(
        TD(ACT_GRV), TD(ACT_1), OPT2, OPT3, OPT4, OPT5,  OPT6,   OPT7,  OPT8,   OPT9,  OPT0,   OPTMIN,  OPTEQ,       _______,   KC_MUTE,
        _______,    OPTQ,  OPTW, TD(ACT_E), OPTR,  OPTT,   OPTY, TD(ACT_U), TD(ACT_I), OPTO, OPTP,  OPTLBR,  OPTRBR, OPTBSL,    _______,
        _______,        OPTA,  OPTS,  OPTD,  OPTF,   OPTG,     OPTH,   OPTJ,   OPTK,   OPTL,   OPTSEM, OPTAPO,      _______,    _______,
        _______,           OPTZ,  OPTX,  OPTC,   OPTV,   OPTB, TD(ACT_N),  OPTM,  OPTCOM,  OPTDOT, OPTSLS,   _______,  _______, _______,
        _______,    _______,  _______,                    _______,                      _______, _______,     _______, _______, _______
    ),
//  [WIDE_TEXT_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |      ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||              |: Vol  :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |          ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS    ||BarTxt|  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||               ||SThru |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |                 ||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||LTRANS||           ||      ||UnderL|  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |         ||        ||         ||                    LTRANS                    ||         ||         |  |      ||      ||      |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [WIDE_TEXT_LAYR] = LAYOUT_65_ansi_blocker(
        _______, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,  LTRANS,    _______,   BBRTEXT,
        _______,   LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,  LTRANS,  LTRANS,    BARTEXT,
        _______,     LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,        _______,      STHRU,
        _______,          LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS, LTRANS,    _______,  _______, UNDERLN,
        _______,  _______,  _______,                        LTRANS,                      _______,  _______,   _______, _______, _______
    ),
//  [CIRCLE_TEXT_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |      ||CIRCL1||CIRCL2||CIRCL3||CIRCL4||CIRCL5||CIRCL6||CIRCL7||CIRCL8||CIRCL9||CIRCL0||      ||      ||              |: Vol  :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |          ||CIRCLQ||CIRCLW||CIRCLE||CIRCLR||CIRCLT||CIRCLY||CIRCLU||CIRCLI||CIRCLO||CIRCLP||      ||      ||          ||      |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |             ||CIRCLA||CIRCLS||CIRCLD||CIRCLF||CIRCLG||CIRCLH||CIRCLJ||CIRCLK||CIRCLL||      ||      ||               ||      |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |                 ||CIRCLZ||CIRCLX||CIRCLC||CIRCLV||CIRCLB||CIRCLN||CIRCLM||      ||      ||      ||           ||      ||      |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |         ||        ||         ||                                              ||         ||         |  |      ||      ||      |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
    [CIRCLE_TEXT_LAYR] = LAYOUT_65_ansi_blocker(
        _______, CIRCL1, CIRCL2, CIRCL3, CIRCL4, CIRCL5, CIRCL6, CIRCL7, CIRCL8, CIRCL9, CIRCL0, _______, _______,   _______,   KC_MUTE,
        _______,   CIRCLQ, CIRCLW, CIRCLE, CIRCLR, CIRCLT, CIRCLY, CIRCLU, CIRCLI, CIRCLO, CIRCLP, _______, _______,  _______,  _______,
        _______,     CIRCLA, CIRCLS, CIRCLD, CIRCLF, CIRCLG, CIRCLH, CIRCLJ, CIRCLK, CIRCLL, _______, _______,      _______,    _______,
        _______,          CIRCLZ, CIRCLX, CIRCLC, CIRCLV, CIRCLB, CIRCLN, CIRCLM, _______, _______, _______,  _______, _______, _______,
        _______,  _______,  _______,                       _______,                      _______,  _______,   _______, _______, _______
    ),
//  [LOCK_LAYR]
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  || No   ||  No  ||  No  ||  No  ||  No  ||     No       |: Unlk :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |   No     ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||   No     ||  No  |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |     No      ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||      No       ||  No  |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |       No        ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||  No  ||    No     ||  No  ||  No  |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |   No    ||   No   ||   No    ||                     No                       ||   No    ||   No    |  |  No  ||  No  ||  No  |  :
// :  |_________||________||_________||______________________________________________||_________||_________|  |______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
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
//  ____________________________________________________________________________________________________________________________________
// :   _______________________________________________________________________________________________________________________ .----.   :
// :  |  54  ||  55  ||  56  ||  57  ||  58  ||  59  ||  60  ||  61  ||  62  ||  63  ||  64  ||  65  ||  66  ||     67       |:      :  :
// :  |______||______||______||______||______||______||______||______||______||______||______||______||______||______________|'.____.'  :
// :  |    53    ||  52  ||  51  ||  50  ||  49  ||  48  ||  47  ||  46  ||  45  ||  44  ||  43  ||  42  ||  41  ||   40     ||  39  |  :
// :  |__________||______||______||______||______||______||______||______||______||______||______||______||______||__________||______|  :
// :  |     25      ||  26  ||  27  ||  28  ||  29  ||  30  ||  31  ||  32  ||  33  ||  34  ||  35  ||  36  ||      37       ||  38  |  :
// :  |_____________||______||______||______||______||______||______||______||______||______||______||______||_______________||______|  :
// :  |       24        ||  20  ||  19  ||  18  ||  17  ||  16  ||  15  ||  14  ||  13  ||  12  ||  11  ||    10     ||  9   ||  8   |  :
// :  |_________________||______||______||______||______||______||______||______||______||______||______||___________||______||______|  :
// :  |   23    ||   22   ||   21    ||                      0                       ||    1    ||    2    |3 |  5   ||  6   ||  7   |  :
// :  |_________||________||_________||______________________________________________||_________||_________| 4|______||______||______|  :
// `------------------------------------------------------------------------------------------------------------------------------------`
// useful key indexes
enum key_indexes {
    I_INDICATOR = 3,
    I_INDICATOR2 = 4,
    I_UNDERLN = 8,
    I_STHRU = 38,
    I_BARTEXT = 39,
    I_NUMLOCK = 40,
    I_ESC = 54,
    I_GRV = 54,
    I_N1 = 55,
    I_N2 = 56,
    I_N3 = 57,
    I_N4 = 58,
    I_N5 = 59,
    I_N6 = 60,
    I_N7 = 61,
    I_N8 = 62,
    I_N9 = 63,
    I_N0 = 64,
    I_MIN = 65,
    I_PLUS = 66,
    I_BBRTEXT = 67,
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
    I_PGUP = 38,
    I_LSFT = 24,
    I_N = 15,
    I_V = 17,
    I_MPLY1 = 13,
    I_MPLY2 = 12,
    I_RSFT = 10,
    I_UP = 9,
    I_PGDN = 8,
    I_LCTL = 23,
    I_RCTL = 2,
    I_LEFT = 5,
    I_DOWN = 6,
    I_RIGHT = 7,
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
    DOUBLE_HOLD = 4,
    TRIPLE_TAP = 5,
    QUAD_TAP = 6,
    PENT_TAP = 7,
    HEXA_TAP = 8
};
// functions associated with all tap dances
int cur_dance (tap_dance_state_t *state);
// functions associated with individual tap dances
void caps_finished (tap_dance_state_t *state, void *user_data);
void caps_reset (tap_dance_state_t *state, void *user_data);
void lgui_finished (tap_dance_state_t *state, void *user_data);
void lgui_reset (tap_dance_state_t *state, void *user_data);
void fn_finished (tap_dance_state_t *state, void *user_data);
void fn_reset (tap_dance_state_t *state, void *user_data);
void rctl_finished (tap_dance_state_t *state, void *user_data);
void rctl_reset (tap_dance_state_t *state, void *user_data);
void rsft_finished (tap_dance_state_t *state, void *user_data);
void rsft_reset (tap_dance_state_t *state, void *user_data);
void actgrv_finished (tap_dance_state_t *state, void *user_data);
void actgrv_reset (tap_dance_state_t *state, void *user_data);
void act1_finished (tap_dance_state_t *state, void *user_data);
void act1_reset (tap_dance_state_t *state, void *user_data);
void acte_finished (tap_dance_state_t *state, void *user_data);
void acte_reset (tap_dance_state_t *state, void *user_data);
void actu_finished (tap_dance_state_t *state, void *user_data);
void actu_reset (tap_dance_state_t *state, void *user_data);
void acti_finished (tap_dance_state_t *state, void *user_data);
void acti_reset (tap_dance_state_t *state, void *user_data);
void actn_finished (tap_dance_state_t *state, void *user_data);
void actn_reset (tap_dance_state_t *state, void *user_data);
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

// function to send symbols normally requiring hex codes in linux
void symbol_key_linux(const char *hex_code, const char *shift_hex_code);

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
enum {
    WIDE_STANDARD,
    WIDE_STHRU,
    WIDE_UNDERLN,
    WIDE_BARTEXT,
    WIDE_BBRTEXT
};
uint8_t wide_text_mode = WIDE_STANDARD;
bool wide_firstchar = false;

// for tracking if an accent char tap dance should light up a particular key to show what the tap will send
int act_char_led_index = 0;

bool is_super_held;
int super_scut_keys[] = {I_INDICATOR, I_INDICATOR2, I_N1, I_N2, I_N3, I_N4, I_N5, I_N6, I_N7, I_N8, I_N9, I_N0, I_TAB, I_Q, I_A, I_D, I_L, I_PGUP, I_PGDN, I_V};
int super_scut_altcolor[] = {I_GRV, I_UP, I_DOWN, I_LEFT, I_RIGHT};
int super_scut_keys_size = sizeof(super_scut_keys) / sizeof(super_scut_keys[0]);
int super_scut_altcolor_size = sizeof(super_scut_altcolor) / sizeof(super_scut_altcolor[0]);

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
            if (layer == WIDE_TEXT_LAYR && wide_firstchar) {
                unregister_mods(mods); // temp remove mods
                switch (wide_text_mode) {
                case WIDE_STHRU:
                    tap_code16(KC_MINS);
                    break;
                case WIDE_UNDERLN:
                    tap_code16(KC_UNDS);
                    break;
                case WIDE_BARTEXT:
                    tap_code16(KC_PIPE);
                    break;
                case WIDE_BBRTEXT:
                    symbol_key_linux("00a6","");
                    break;
                default:
                    break;
                }
                register_mods(mods);   // reapply mods
            }

            register_code(keymap_key_to_keycode(biton32(default_layer_state), record->event.key));

            // if WIDE_TEXT_LAYER, add the extra spacing char
            if (layer == WIDE_TEXT_LAYR) {
                unregister_mods(mods); // temp remove mods
                switch (wide_text_mode) {
                case WIDE_STHRU:
                    tap_code16(KC_MINS);
                    break;
                case WIDE_UNDERLN:
                    tap_code16(KC_UNDS);
                    break;
                case WIDE_BARTEXT:
                    tap_code16(KC_PIPE);
                    break;
                case WIDE_BBRTEXT:
                    symbol_key_linux("00a6","");
                    break;
                default:
                    tap_code16(KC_SPC);
                    break;
                }
                register_mods(mods);   // reapply mods
                wide_firstchar = false;
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
            if (wide_text_mode == WIDE_STHRU) {
                wide_text_mode = WIDE_STANDARD;
                wide_firstchar = false;
            }
            else {
                wide_text_mode = WIDE_STHRU;
                wide_firstchar = true;
            }
        }
        break;
    case UNDERLN:
        if (record->event.pressed) {
            if (wide_text_mode == WIDE_UNDERLN) {
                wide_text_mode = WIDE_STANDARD;
                wide_firstchar = false;
            }
            else {
                wide_text_mode = WIDE_UNDERLN;
                wide_firstchar = true;
            }
        }
        break;
    case BARTEXT:
        if (record->event.pressed) {
            if (wide_text_mode == WIDE_BARTEXT) {
                wide_text_mode = WIDE_STANDARD;
                wide_firstchar = false;
            }
            else {
                wide_text_mode = WIDE_BARTEXT;
                wide_firstchar = true;
            }
        }
        break;
    case BBRTEXT:
        if (record->event.pressed) {
            if (wide_text_mode == WIDE_BBRTEXT) {
                wide_text_mode = WIDE_STANDARD;
                wide_firstchar = false;
            }
            else {
                wide_text_mode = WIDE_BBRTEXT;
                wide_firstchar = true;
            }
        }
        break;
    // the following OPT keycodes mimic a macos option os layer for symbols and accents
    // symbol_key() is a fn to type a windows alt code on the numpad
    // symbol_key_linux() uses hex codes to type the same symbols on linux
    case OPT2:
        if (record->event.pressed) {
            symbol_key_linux("2122","20ac");
        }
        break;
    case OPT3:
        if (record->event.pressed) {
            symbol_key_linux("00a3","2039");
        }
        break;
    case OPT4:
        if (record->event.pressed) {
            symbol_key_linux("00a2","203a");
        }
        break;
    case OPT5:
        if (record->event.pressed) {
            symbol_key_linux("221e","fb01");
        }
        break;
    case OPT6:
        if (record->event.pressed) {
            symbol_key_linux("00a7","fb02");
        }
        break;
    case OPT7:
        if (record->event.pressed) {
            symbol_key_linux("00b6","2021");
        }
        break;
    case OPT8:
        if (record->event.pressed) {
            symbol_key_linux("2022","00b0");
        }
        break;
    case OPT9:
        if (record->event.pressed) {
            symbol_key_linux("00aa","00b7");
        }
        break;
    case OPT0:
        if (record->event.pressed) {
            symbol_key_linux("00ba","201a");
        }
        break;
    case OPTMIN:
        if (record->event.pressed) {
            symbol_key_linux("2013","2014");
        }
        break;
    case OPTEQ:
        if (record->event.pressed) {
            symbol_key_linux("2260","00b1");
        }
        break;
    case OPTQ:
        if (record->event.pressed) {
            symbol_key_linux("0153","0152");
        }
        break;
    case OPTW:
        if (record->event.pressed) {
            symbol_key_linux("2211","201e");
        }
        break;
    case OPTR:
        if (record->event.pressed) {
            symbol_key_linux("00ae","2030");
        }
        break;
    case OPTT:
        if (record->event.pressed) {
            symbol_key_linux("2020","02c7");
        }
        break;
    case OPTY:
        if (record->event.pressed) {
            symbol_key_linux("00a5","00c1");
        }
        break;
    case OPTO:
        if (record->event.pressed) {
            symbol_key_linux("00f8","00d8");
        }
        break;
    case OPTP:
        if (record->event.pressed) {
            symbol_key_linux("03c0","03a0");
        }
        break;
    case OPTLBR:
        if (record->event.pressed) {
            symbol_key_linux("201c","201d");
        }
        break;
    case OPTRBR:
        if (record->event.pressed) {
            symbol_key_linux("2018","2019");
        }
        break;
    case OPTBSL:
        if (record->event.pressed) {
            symbol_key_linux("00ab","00bb");
        }
        break;
    case OPTA:
        if (record->event.pressed) {
            symbol_key_linux("00e5","00c5");
        }
        break;
    case OPTS:
        if (record->event.pressed) {
            symbol_key_linux("00df","00cd");
        }
        break;
    case OPTD:
        if (record->event.pressed) {
            symbol_key_linux("2202","00ce");
        }
        break;
    case OPTF:
        if (record->event.pressed) {
            symbol_key_linux("0192","00cf");
        }
        break;
    case OPTG:
        if (record->event.pressed) {
            symbol_key_linux("00a9","02dd");
        }
        break;
    case OPTH:
        if (record->event.pressed) {
            symbol_key_linux("02d9","00d3");
        }
        break;
    case OPTJ:
        if (record->event.pressed) {
            symbol_key_linux("2206","00d4");
        }
        break;
    case OPTK:
        if (record->event.pressed) {
            symbol_key_linux("02da","03f0");
        }
        break;
    case OPTL:
        if (record->event.pressed) {
            symbol_key_linux("00ac","00d2");
        }
        break;
    case OPTSEM:
        if (record->event.pressed) {
            symbol_key_linux("2026","00da");
        }
        break;
    case OPTAPO:
        if (record->event.pressed) {
            symbol_key_linux("00e6","00c6");
        }
        break;
    case OPTZ:
        if (record->event.pressed) {
            symbol_key_linux("03a9","00b8");
        }
        break;
    case OPTX:
        if (record->event.pressed) {
            symbol_key_linux("2248","02db");
        }
        break;
    case OPTC:
        if (record->event.pressed) {
            symbol_key_linux("00e7","00c7");
        }
        break;
    case OPTV:
        if (record->event.pressed) {
            symbol_key_linux("221a","25ca");
        }
        break;
    case OPTB:
        if (record->event.pressed) {
            symbol_key_linux("222b","0131");
        }
        break;
    case OPTM:
        if (record->event.pressed) {
            symbol_key_linux("03bc","00c2");
        }
        break;
    case OPTCOM:
        if (record->event.pressed) {
            symbol_key_linux("2264","00af");
        }
        break;
    case OPTDOT:
        if (record->event.pressed) {
            symbol_key_linux("2265","02d8");
        }
        break;
    case OPTSLS:
        if (record->event.pressed) {
            symbol_key_linux("00f7","00bf");
        }
        break;
    case CIRCL1:
        if (record->event.pressed) {
            // check mods first so can do a double-cirled number if alt is held
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24f5","24f5");
                register_mods(mods);
            } else {
                symbol_key_linux("2460","246a");
            }
        }
        break;
    case CIRCL2:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24f6","24f6");
                register_mods(mods);
            } else {
                symbol_key_linux("2461","246b");
            }
        }
        break;
    case CIRCL3:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24f7","24f7");
                register_mods(mods);
            } else {
                symbol_key_linux("2462","246c");
            }
        }
        break;
    case CIRCL4:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24f8","24f8");
                register_mods(mods);
            } else {
                symbol_key_linux("2463","246d");
            }
        }
        break;
    case CIRCL5:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24f9","24f9");
                register_mods(mods);
            } else {
                symbol_key_linux("2464","246e");
            }
        }
        break;
    case CIRCL6:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24fa","24fa");
                register_mods(mods);
            } else {
                symbol_key_linux("2465","246f");
            }
        }
        break;
    case CIRCL7:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24fb","24fb");
                register_mods(mods);
            } else {
                symbol_key_linux("2466","2470");
            }
        }
        break;
    case CIRCL8:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24fc","24fc");
                register_mods(mods);
            } else {
                symbol_key_linux("2467","2471");
            }
        }
        break;
    case CIRCL9:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24fd","24fd");
                register_mods(mods);
            } else {
                symbol_key_linux("2468","2472");
            }
        }
        break;
    case CIRCL0:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                symbol_key_linux("24fe","2473");
                register_mods(mods);
            } else {
                symbol_key_linux("24ea","2469");
            }
        }
        break;
    case CIRCLQ:
        if (record->event.pressed) {
            symbol_key_linux("24e0","24c6");
        }
        break;
    case CIRCLW:
        if (record->event.pressed) {
            symbol_key_linux("24e6","24cc");
        }
        break;
    case CIRCLE:
        if (record->event.pressed) {
            symbol_key_linux("24d4","24ba");
        }
        break;
    case CIRCLR:
        if (record->event.pressed) {
            symbol_key_linux("24e1","24c7");
        }
        break;
    case CIRCLT:
        if (record->event.pressed) {
            symbol_key_linux("24e3","24c9");
        }
        break;
    case CIRCLY:
        if (record->event.pressed) {
            symbol_key_linux("24e8","24ce");
        }
        break;
    case CIRCLU:
        if (record->event.pressed) {
            symbol_key_linux("24e4","24ca");
        }
        break;
    case CIRCLI:
        if (record->event.pressed) {
            symbol_key_linux("24d8","24be");
        }
        break;
    case CIRCLO:
        if (record->event.pressed) {
            symbol_key_linux("24de","24c4");
        }
        break;
    case CIRCLP:
        if (record->event.pressed) {
            symbol_key_linux("24df","24c5");
        }
        break;
    case CIRCLA:
        if (record->event.pressed) {
            symbol_key_linux("24d0","24b6");
        }
        break;
    case CIRCLS:
        if (record->event.pressed) {
            symbol_key_linux("24e2","24c8");
        }
        break;
    case CIRCLD:
        if (record->event.pressed) {
            symbol_key_linux("24d3","24b9");
        }
        break;
    case CIRCLF:
        if (record->event.pressed) {
            symbol_key_linux("24d5","24bb");
        }
        break;
    case CIRCLG:
        if (record->event.pressed) {
            symbol_key_linux("24d6","24bc");
        }
        break;
    case CIRCLH:
        if (record->event.pressed) {
            symbol_key_linux("24d7","24bd");
        }
        break;
    case CIRCLJ:
        if (record->event.pressed) {
            symbol_key_linux("24d9","24bf");
        }
        break;
    case CIRCLK:
        if (record->event.pressed) {
            symbol_key_linux("24da","24c0");
        }
        break;
    case CIRCLL:
        if (record->event.pressed) {
            symbol_key_linux("24db","24c1");
        }
        break;
    case CIRCLZ:
        if (record->event.pressed) {
            symbol_key_linux("24e9","24cf");
        }
        break;
    case CIRCLX:
        if (record->event.pressed) {
            symbol_key_linux("24e7","24cd");
        }
        break;
    case CIRCLC:
        if (record->event.pressed) {
            symbol_key_linux("24d2","24b8");
        }
        break;
    case CIRCLV:
        if (record->event.pressed) {
            symbol_key_linux("24e5","24cb");
        }
        break;
    case CIRCLB:
        if (record->event.pressed) {
            symbol_key_linux("24d1","24b7");
        }
        break;
    case CIRCLN:
        if (record->event.pressed) {
            symbol_key_linux("24dd","24c3");
        }
        break;
    case CIRCLM:
        if (record->event.pressed) {
            symbol_key_linux("24dc","24c2");
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

void symbol_key_linux(const char *hex_code, const char *shift_hex_code) {
    // get current mod and one-shot mod states.
    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    const char *ucode = ((mods | oneshot_mods) & MOD_MASK_SHIFT) ? shift_hex_code : hex_code;
    uint8_t layer = get_highest_layer(layer_state);
    if (ucode == NULL || *ucode == '\0') { // null or empty string
        return;
    }
    clear_oneshot_mods();
    unregister_mods(mods); // temp remove mods
    // this alternate delay setup is needed specifically on the Yunzii for bbrtext to work right
    if (layer == WIDE_TEXT_LAYR) {
        send_string_with_delay(SS_LCTL(SS_LSFT(SS_TAP(X_U))),5); // start the sequence
        send_string_with_delay(ucode, 5); // type the ucode
        send_string_with_delay(SS_TAP(X_SPC), 16); // finish sequence
    }
    else {
        tap_code16(C(S(KC_U))); // start the unicode sequence
        // type the hex chars
        send_string_with_delay(ucode,5);
        // finish sequence
        tap_code(KC_SPC);
    }
    register_mods(mods);
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
    case SYMBOL_LAYR:
        if (is_led_on) {    
            if (timer_elapsed(layer_timer) > 250) {
                rgb_matrix_set_color(I_INDICATOR, RGB_YELLOW);
                rgb_matrix_set_color(I_INDICATOR2, RGB_YELLOW);
            }
            else {
                rgb_matrix_set_color(I_INDICATOR, RGB_BLUE);
                rgb_matrix_set_color(I_INDICATOR2, RGB_BLUE);
            }
        }   
        break;
    case WIDE_TEXT_LAYR:
        rgb_matrix_set_color(I_INDICATOR, RGB_PURPLE);
        rgb_matrix_set_color(I_INDICATOR2, RGB_PURPLE);
        break;
    case CIRCLE_TEXT_LAYR:
        rgb_matrix_set_color(I_INDICATOR, RGB_CORAL);
        rgb_matrix_set_color(I_INDICATOR2, RGB_CORAL);
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
                    case SYMBOL_LAYR:
                        rgb_matrix_set_color(index, RGB_BLUE);
                        break;
                    case WIDE_TEXT_LAYR:
                        rgb_matrix_set_color(index, RGB_PURPLE);
                        break;
                    case CIRCLE_TEXT_LAYR:
                        rgb_matrix_set_color(index, RGB_CORAL);
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
        // custom colors for layer keys on fn layer
        if (layer == FN_LAYR) {
          rgb_matrix_set_color(I_LSFT, RGB_ORANGE); // left shift
          rgb_matrix_set_color(I_RSFT, RGB_ORANGE); // right shift
          rgb_matrix_set_color(I_LALT, RGB_RED);    // left alt
          rgb_matrix_set_color(I_RCTL, RGB_RED);    // right ctrl
          rgb_matrix_set_color(I_TAB, RGB_CYAN);    // tab
          rgb_matrix_set_color(I_LGUI, RGB_BLUE);   // super
        }  
// ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ */
// ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
        // custom colors for tap dance keys on symbol layer
        else if (layer == SYMBOL_LAYR) {
          rgb_matrix_set_color(I_GRV, RGB_YELLOW); // grave
          rgb_matrix_set_color(I_N1, RGB_YELLOW);   // 1
          rgb_matrix_set_color(I_E, RGB_YELLOW);   // e
          rgb_matrix_set_color(I_U, RGB_YELLOW);   // u
          rgb_matrix_set_color(I_I, RGB_YELLOW);   // i
          rgb_matrix_set_color(I_N, RGB_YELLOW);   // n
        }
// ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
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
            case SYMBOL_LAYR:
                if (timer_elapsed(layer_timer) > 250) {
                    rgb_matrix_set_color(I_LGUI, RGB_YELLOW); // lgui
                    rgb_matrix_set_color(I_FN, RGB_YELLOW);   // fn
                }
                else {
                    rgb_matrix_set_color(I_LGUI, RGB_BLUE);   // lgui
                    rgb_matrix_set_color(I_FN, RGB_BLUE);     // fn
                }
                break;
            case WIDE_TEXT_LAYR:
                rgb_matrix_set_color(I_RSFT, RGB_PURPLE);   // rshift
                break;
            case CIRCLE_TEXT_LAYR:
                rgb_matrix_set_color(I_RSFT, RGB_CORAL);      // rshift
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
        if (is_super_held) {
            for (int i = 0; i < super_scut_keys_size; i++) {
                rgb_matrix_set_color(super_scut_keys[i], 102, 178, 255);   // gnome super shortcut keys
            }
            for (int i = 0; i < super_scut_altcolor_size; i++) {
                rgb_matrix_set_color(super_scut_altcolor[i], RGB_BLUE);    // gnome super shortcut keys
            }
        }
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
    
    // track if this is the middle of an accent key tap dance and illuminate the appropriate accent char
    if (act_char_led_index > 0) {
        rgb_matrix_set_color(act_char_led_index, RGB_WHITE);  // accent char led
    }

    // track mode keys on WIDE_TEXT_LAYR
    if (layer == WIDE_TEXT_LAYR) {
        switch (wide_text_mode) {
        case WIDE_STHRU:
            rgb_matrix_set_color(I_STHRU, RGB_WHITE);    // sthru toggle
            break;
        case WIDE_UNDERLN:
            rgb_matrix_set_color(I_UNDERLN, RGB_WHITE);  // underln toggle
            break;
        case WIDE_BARTEXT:
            rgb_matrix_set_color(I_BARTEXT, RGB_WHITE);  // bartext toggle
            break;
        case WIDE_BBRTEXT:
            rgb_matrix_set_color(I_BBRTEXT, RGB_WHITE);  // bbrtext toggle
            break;
        default:
            break;
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
      ((layer == FN_LAYR || layer == SFT_LAYR || layer == WIDE_TEXT_LAYR ||
        layer == CIRCLE_TEXT_LAYR || is_caps_word_on()) &&
        (key.index == I_LSFT || key.index == I_RSFT)) ||                                           // left and right shift
      ((layer == FN_LAYR || layer == CTL_LAYR) &&
            (key.index == I_LALT || key.index == I_RCTL)) ||                                       // left alt and right ctrl
      (layer == SFT_LAYR && key.index == I_NUMLOCK) ||                                             // num lock key
      (layer == FN_LAYR && key.index == I_SLOCK) ||                                                // scroll lock
      (layer == WIDE_TEXT_LAYR &&
        (key.index == I_BARTEXT || key.index == I_STHRU ||
         key.index == I_UNDERLN || key.index == I_BBRTEXT)) ||                                     // wide-text mode toggles
      (layer == CTL_LAYR && (key.index == I_FJLIGHT || key.index == I_HROWLIGHT)) ||               // fj/hrow light keys 
      (layer == BASE_LAYR && key.index == I_FN) ||                                                 // fn on base 
      (layer == SYMBOL_LAYR && (key.index == I_GRV || key.index == I_N1 || key.index == I_E ||
                                key.index == I_I || key.index == I_U || key.index == I_N ||        // accent keys
                                key.index == I_FN || key.index == I_LGUI)) ||                      // sym_layr lgui and fn
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
    if (!state->pressed) {
        return DOUBLE_TAP;
    } else {
        return DOUBLE_HOLD;
    }
  } else if (state->count == 3) {
    return TRIPLE_TAP; 
  } else if (state->count == 4) {
    return QUAD_TAP;
  } else if (state->count == 5) {
    return PENT_TAP;
  } else if (state->count == 6) {
    return HEXA_TAP;
  }
  else return 9;
}

//Initialize tap structure associated with caps tap dance key
static tap caps_tap_state = {
  .is_press_action = true,
  .state = 0 
};
static tap lgui_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap actgrv_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap act1_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap acte_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap actu_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap acti_tap_state = {
  .is_press_action = true,
  .state = 0
};
static tap actn_tap_state = {
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
    case HEXA_TAP:
      if (layer_state_is(SYMBOL_LAYR)) {
        //if already set, then switch it off
        layer_lock_off(SYMBOL_LAYR);
      } else {
        //if not already set, then switch the layer on
        layer_lock_on(SYMBOL_LAYR);
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

// function for lgui tap dance
void lgui_finished (tap_dance_state_t *state, void *user_data) {
  lgui_tap_state.state = cur_dance(state);
  switch (lgui_tap_state.state) {
    case SINGLE_TAP:
      tap_code(KC_LGUI);
      break;
    case SINGLE_HOLD:
      register_code(KC_LGUI);
      is_super_held = true;
      break;
    case DOUBLE_TAP:
      set_oneshot_layer(SYMBOL_LAYR, ONESHOT_START);
      clear_oneshot_layer_state(ONESHOT_PRESSED);
      break;
    case DOUBLE_HOLD:
      layer_on(SYMBOL_LAYR);
      break;
  }
}
void lgui_reset (tap_dance_state_t *state, void *user_data) {
  switch (lgui_tap_state.state) {
    case SINGLE_TAP:
      break;
    case SINGLE_HOLD:
      unregister_code(KC_LGUI);
      is_super_held = false;
      break;
    case DOUBLE_TAP:
      break;
    case DOUBLE_HOLD:
      if (!is_layer_locked(SYMBOL_LAYR)) {
        layer_off(SYMBOL_LAYR);
      }
      break;
  }
  lgui_tap_state.state = 0;
}

// function for each press of grv on symbol layer
void actgrv_each(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            act_char_led_index = I_A;
            break;
        case 2:
            act_char_led_index = I_E;
            break;
        case 3:
            act_char_led_index = I_I;
            break;
        case 4:
            act_char_led_index = I_O;
            break;
        case 5:
            act_char_led_index = I_U;
            break;
        default:
            act_char_led_index = 0;
            break;
    }
}
// function for symbol layer grv key tap dance
void actgrv_finished (tap_dance_state_t *state, void *user_data) {
  actgrv_tap_state.state = cur_dance(state);
  switch (actgrv_tap_state.state) {
    case SINGLE_TAP:
      symbol_key_linux("00e0","00c0");
      break;
    case DOUBLE_TAP:
      symbol_key_linux("00e8","00c8");
      break;
    case TRIPLE_TAP:
      symbol_key_linux("00ec","00cc");
      break;
    case QUAD_TAP:
      symbol_key_linux("00f2","00d2");
      break;
    case PENT_TAP:
      symbol_key_linux("00f9","00d9");
      break;
    case HEXA_TAP:
      break;
  }
}
void actgrv_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    actgrv_tap_state.state = 0;
}

// function for each press of 1 on symbol layer
void act1_each(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            act_char_led_index = I_N1;
            break;
        case 2:
            act_char_led_index = I_N2;
            break;
        case 3:
            act_char_led_index = I_N3;
            break;
        default:
            act_char_led_index = 0;
            break;
    }
}
// function for symbol layer 1 key tap dance
void act1_finished (tap_dance_state_t *state, void *user_data) {
  act1_tap_state.state = cur_dance(state);
  switch (act1_tap_state.state) {
    case SINGLE_TAP:
      symbol_key_linux("00a1","00bc");
      break;
    case DOUBLE_TAP:
      symbol_key_linux("00bd","00bd");
      break;
    case TRIPLE_TAP:
      symbol_key_linux("00be","00be");
      break;
    case QUAD_TAP:
      break;
    case PENT_TAP:
      break;
    case HEXA_TAP:
      break;
  }
}
void act1_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    act1_tap_state.state = 0;
}

// function for each press of e on symbol layer
void acte_each(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            act_char_led_index = I_E;
            break;
        case 2:
            act_char_led_index = I_A;
            break;
        case 3:
            act_char_led_index = I_I;
            break;
        case 4:
            act_char_led_index = I_O;
            break;
        case 5:
            act_char_led_index = I_U;
            break;
        case 6:
            act_char_led_index = I_Y;
            break;
        default:
            act_char_led_index = 0;
            break;
    }
}
// function for symbol layer e key tap dance
void acte_finished (tap_dance_state_t *state, void *user_data) {
  acte_tap_state.state = cur_dance(state);
  switch (acte_tap_state.state) {
    case SINGLE_TAP:
      symbol_key_linux("00e9","00c9");
      break;
    case DOUBLE_TAP:
      symbol_key_linux("00e1","00c1");
      break;
    case TRIPLE_TAP:
      symbol_key_linux("00ed","00cd");
      break;
    case QUAD_TAP:
      symbol_key_linux("00f3","00d3");
      break;
    case PENT_TAP:
      symbol_key_linux("00fa","00da");
      break;
    case HEXA_TAP:
      symbol_key_linux("00fd","00dd");
      break;
  }
}
void acte_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    acte_tap_state.state = 0;
}

// function for each press of u on symbol layer
void actu_each(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            act_char_led_index = I_A;
            break;
        case 2:
            act_char_led_index = I_E;
            break;
        case 3:
            act_char_led_index = I_I;
            break;
        case 4:
            act_char_led_index = I_O;
            break;
        case 5:
            act_char_led_index = I_U;
            break;
        case 6:
            act_char_led_index = I_Y;
            break;
        default:
            act_char_led_index = 0;
            break;
    }
}
// function for symbol layer u key tap dance
void actu_finished (tap_dance_state_t *state, void *user_data) {
  actu_tap_state.state = cur_dance(state);
  switch (actu_tap_state.state) {
    case SINGLE_TAP:
      symbol_key_linux("00e4","00c4");
      break;
    case DOUBLE_TAP:
      symbol_key_linux("00eb","00cb");
      break;
    case TRIPLE_TAP:
      symbol_key_linux("00ef","00cf");
      break;
    case QUAD_TAP:
      symbol_key_linux("00f6","00d6");
      break;
    case PENT_TAP:
      symbol_key_linux("00fc","00dc");
      break;
    case HEXA_TAP:
      symbol_key_linux("00ff","0178");
      break;
  }
}
void actu_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    actu_tap_state.state = 0;
}

// function for each press of i on symbol layer
void acti_each(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            act_char_led_index = I_I;
            break;
        case 2:
            act_char_led_index = I_A;
            break;
        case 3:
            act_char_led_index = I_E;
            break;
        case 4:
            act_char_led_index = I_O;
            break;
        case 5:
            act_char_led_index = I_U;
            break;
        default:
            act_char_led_index = 0;
            break;
    }
}
// function for symbol layer i key tap dance
void acti_finished (tap_dance_state_t *state, void *user_data) {
  acti_tap_state.state = cur_dance(state);
  switch (acti_tap_state.state) {
    case SINGLE_TAP:
      symbol_key_linux("00ee","00ce");
      break;
    case DOUBLE_TAP:
      symbol_key_linux("00e2","00c2");
      break;
    case TRIPLE_TAP:
      symbol_key_linux("00ea","00ca");
      break;
    case QUAD_TAP:
      symbol_key_linux("00f4","00d4");
      break;
    case PENT_TAP:
      symbol_key_linux("00fb","00db");
      break;
    case HEXA_TAP:
      break;
  }
}
void acti_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    acti_tap_state.state = 0;
}

// function for each press of n on symbol layer
void actn_each(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            act_char_led_index = I_N;
            break;
        case 2:
            act_char_led_index = I_A;
            break;
        case 3:
            act_char_led_index = I_O;
            break;
        default:
            act_char_led_index = 0;
            break;
    }
}
// function for symbol layer n key tap dance
void actn_finished (tap_dance_state_t *state, void *user_data) {
  actn_tap_state.state = cur_dance(state);
  switch (actn_tap_state.state) {
    case SINGLE_TAP:
      symbol_key_linux("00f1","00d1");
      break;
    case DOUBLE_TAP:
      symbol_key_linux("00e3","00c3");
      break;
    case TRIPLE_TAP:
      symbol_key_linux("00f5","00d5");
      break;
  }
}
void actn_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    actn_tap_state.state = 0;
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
    case DOUBLE_TAP:
      set_oneshot_layer(SYMBOL_LAYR, ONESHOT_START);
      clear_oneshot_layer_state(ONESHOT_PRESSED);
      break;
    case DOUBLE_HOLD:
      layer_on(SYMBOL_LAYR);
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
    case DOUBLE_TAP:
      break;
    case DOUBLE_HOLD:
      if (!is_layer_locked(SYMBOL_LAYR)) {
        layer_off(SYMBOL_LAYR);
      }
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
    case SINGLE_HOLD:
      // check if this is caps word activation, otherwise regular shift
      if (get_mods() & MOD_BIT(KC_LSFT)) {
          caps_word_on();
      }
      else {
          register_code(KC_RSFT);
      }
      break;
    case DOUBLE_TAP:
      // activate WIDE_TEXT_LAYR
      if (IS_LAYER_ON(WIDE_TEXT_LAYR)) {
          layer_lock_off(WIDE_TEXT_LAYR);
      }
      else {
          layer_lock_on(WIDE_TEXT_LAYR);
          wide_firstchar = true;
      }
      break;
    case TRIPLE_TAP:
      // activate CIRCLE_TEXT_LAYR
      if (IS_LAYER_ON(CIRCLE_TEXT_LAYR)) {
          layer_lock_off(CIRCLE_TEXT_LAYR);
      }
      else {
          layer_lock_on(CIRCLE_TEXT_LAYR);
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
    case SINGLE_HOLD:
      if (get_mods() & MOD_BIT(KC_LSFT)) {
          caps_word_on();
      }
      else {
          unregister_code(KC_RSFT);
      }
      break;
    case DOUBLE_TAP:
      break;
    case TRIPLE_TAP:
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
  [LGUI_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lgui_finished, lgui_reset),
  [FN_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_finished, fn_reset),
  [RCTL_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rctl_finished, rctl_reset),
  [RSFT_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(rsft_each, rsft_finished, rsft_reset),
  [ACT_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(actgrv_each, actgrv_finished, actgrv_reset),
  [ACT_1] = ACTION_TAP_DANCE_FN_ADVANCED(act1_each, act1_finished, act1_reset),
  [ACT_E] = ACTION_TAP_DANCE_FN_ADVANCED(acte_each, acte_finished, acte_reset),
  [ACT_U] = ACTION_TAP_DANCE_FN_ADVANCED(actu_each, actu_finished, actu_reset),
  [ACT_I] = ACTION_TAP_DANCE_FN_ADVANCED(acti_each, acti_finished, acti_reset),
  [ACT_N] = ACTION_TAP_DANCE_FN_ADVANCED(actn_each, actn_finished, actn_reset),
  [KB_UNLOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, kbunlock_finished, kbunlock_reset),
};

// accent tap dances should give a little bit longer to see the leds
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(ACT_GRV):
        case TD(ACT_1):
        case TD(ACT_E):
        case TD(ACT_U):
        case TD(ACT_I):
        case TD(ACT_N):
            return TAPPING_TERM + 400;
        default:
            return TAPPING_TERM;
    }
}

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

