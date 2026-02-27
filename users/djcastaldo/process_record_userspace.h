// process_record_userspace.h
// @djcastaldo 07/30/2025
#pragma once

#include QMK_KEYBOARD_H
#ifdef KEYBOARD_IS_KEYCHRON
#include "keychron_common.h"
#elif defined(KEYBOARD_IS_LEMOKEY)
#include "lemokey_common.h"
#endif
#include "features/layer_lock.h"

#define NEWFINDER LOPT(LCMD(KC_SPC))            // open new Finder search window
#define FHOME LCMD(LSFT(KC_H))                  // open new Finder home dir
#define FAPPS LCMD(LSFT(KC_A))                  // open new Finder apps dir
#define FDOCS LCMD(LSFT(KC_O))                  // open new Finder docs dir
#define FDOWNL LCMD(LOPT(KC_L))                 // open new Finder downloads dir
#define CMDGRAVE LCMD(KC_GRV)                   // switch to next app window
#define SCMDGRAVE LSFT(LCMD(KC_GRV))            // switch to prev app window
#define FINSPECT LCMD(KC_I)                     // open inspector
#define FVHIDDEN LCMD(LSFT(KC_DOT))             // show Finder hidden files
#define FQLOOK LCMD(KC_Y)                       // open Finder quick look
#define FDELETE LCMD(KC_BSPC)                   // Finder move selection to trash
#define FSTATBAR LCMD(KC_SLSH)                  // show Finder status bar
#define TOVERVIEW LSFT(LCMD(KC_BSLS))           // show Terminal tab overview
#define FORCEQUIT LOPT(LCMD(KC_ESC))            // force quit an app
#define TASKMGR LCTL(LSFT(KC_ESC))              // open windows task manager
#define FEXPLORE LGUI(KC_E)                     // open windows file explorer
#define TIMESTAMP LCTL(LOPT(LCMD(KC_T)))        // command to gen timestamp
#define SOC1 LSFT(LOPT(LCMD(KC_1)))             // snap dock app switching
#define SOC2 LSFT(LOPT(LCMD(KC_2)))             // snap dock app switching
#define SOC3 LSFT(LOPT(LCMD(KC_3)))             // snap dock app switching
#define SOC4 LSFT(LOPT(LCMD(KC_4)))             // snap dock app switching
#define SOC5 LSFT(LOPT(LCMD(KC_5)))             // snap dock app switching
#define SOC6 LSFT(LOPT(LCMD(KC_6)))             // snap dock app switching
#define SOC7 LSFT(LOPT(LCMD(KC_7)))             // snap dock app switching
#define SOC8 LSFT(LOPT(LCMD(KC_8)))             // snap dock app switching
#define SOC9 LSFT(LOPT(LCMD(KC_9)))             // snap dock app switching
#define SOC0 LSFT(LOPT(LCMD(KC_0)))             // snap dock app switching
#define UNICODE LCTL(LCMD(KC_SPC))              // open unicode input menu
#define V_WSPC_NXT LCTL(LSFT(KC_PGUP))          // vivaldi workspace next
#define V_WSPC_PRV LCTL(LSFT(KC_PGDN))          // vivaldi workspace prev
#define V_TAB_NXT LCTL(KC_TAB)                  // vivaldi tab next
#define V_TAB_PRV LCTL(LSFT(KC_TAB))            // vivaldi tab prev
#define MOD_SIRI HYPR(KC_S)                     // customized siri launch
#define US_SIRI LCMD(KC_SPC)                    // standard siri launch
#define US_CATANA LGUI(KC_C)                    // standard catana launch

enum userspace_keycodes {
    SECRET0 = NEW_SAFE_RANGE,
    SECRET1,
    SECRET2,
    SECRET3,
    SECRET4,
    SECRET5,
    SECRET6,
    SECRET7,
    SECRET8,
    SECRET9,
    SECRET10,
    SECRET11,
    SECRET12,
    SECRET13,
    SECRET14,
    SECRET15,
    SECRET16,
    SECRET17,
    SECRET18,
    SECRET19,
    SECRET20,
    SECRET21,
    SECRET22,
    SECRET23,
    SECRET24,
    SECRET25,
    SECRET26,
    SECRET27,
    SECRET28,
    SECRET29,
    SECRET30,
    SECRET31,
    SECRET32,
    SECRET33,
    SECRET34,
    SECRET35,
    SECRET36,
    SECRET37,
    SECRET38,
    SECRET39,
    SECRET40,
    SECRET41,
    SECRET42,
    SECRET43,
    LLOCK,
    BASE_CHG,
    WM_MYCM,
    WM_SYM,
    WM_SOC1,
    WM_SOC2,
    WM_SOC3,
    WM_SOC4,
    WM_SOC5,
    WM_SOC6,
    WM_SOC7,
    WM_SOC8,
    WM_SOC9,
    WM_SOC0,
    WM_QUIT,
    WM_DOCS,
    WM_DLS,
    WM_SIRCAT,
    VSEMOLR,
    DYN_LT,
    JIGGLE,
    FJLIGHT,
    HROWLIGHT,
    KTRACK,
    WAVE,
    ARROW,
    BSPCFAST,
    TMUXLKEY,
    TMUXLCMD,
    TMONON,
    TMONOF,
    TVISON,
    TVISOF,
    TWINLFT,
    TWINRGT,
    TJPANE,
    ENC_TMON,
    ENC_TSIZEL,
    ENC_TSIZER,
    ENC_MUTEPLAY,
    ENC_VOLD,
    ENC_VOLU,
    ENC_UNIMENU,
    ENC_MENUL,
    ENC_MENUR,
    ENC_APPHIDE,
    ENC_SCROLLAPPL,
    ENC_SCROLLAPPR,
    ENC_RGBPUSH,
    ENC_RGBL,
    ENC_RGBR,
    F_ZOOMR,
    DUAL_ZOOMI,
    DUAL_ZOOMO,
    DUAL_PLUSMIN,
    DUAL_MULTDIV,
    DUAL_ESC,
    DUAL_F12,
    DUAL_F13,
    DUAL_F14,
    DUAL_PMNS,
    VI_REPLACE,
    GIT_ADD,
    GIT_COMMIT,
    GIT_PUSH,
    GIT_CHKOUT,
    GIT_LOG,
    LTRANS,
    STHRU,
    UNDERLN,
    BARTEXT,
    BBRTEXT,
    MK_HOLD,
    MK_ACCEL0,
    MK_ACCEL2,
    LOCKSCR,
    BACKDIR,
    HOMEDIR,
    LSLTRAH,
    SSMENU,
    GNEWS,
    CURSORL,
    CURSORR,
    OPT_HOLD,
    DUAL_SNAP,
    AP_GLOB,
    COLORTEST,
    MSYMGRV,
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
    SUITH,
    SUITD,
    SUITC,
    SUITS,
    SUP1,
    SUP2,
    SUP3,
    NBSP,
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
    E_SMILE,
    E_SMIRK,
    E_ASTON,
    E_SAD,
    E_CAR,
    E_BUS,
    E_TRAIN,
    E_TAXI,
    E_FLAGUS,
    E_FLAGPI,
    E_GLOBE,
    E_PIZZA,
    E_BURGER,
    E_CHICKEN,
    E_TACO,
    E_SNAIL,
    E_NINJA,
    E_STOP,
    E_HAND,
    E_SHH,
    E_PACKAGE,
    E_POINTU,
    E_POINTD,
    E_POINTL,
    E_POINTR,
    E_DRINK,
    E_EMARK,
    E_QMARK,
    E_CMARK,
    E_BULB,
    E_SSHOT,
    E_MIC,
    E_TRASH,
    E_I,
    E_K0,
    E_K1,
    E_K2,
    E_K3,
    E_K4,
    E_K5,
    E_K6,
    E_K7,
    E_K8,
    E_K9,
    E_STAR,
    E_HASH,
    E_MINS,
    E_PLUS,
    E_DIV,
    E_GRIM,
    E_KB,
    E_OK,
    E_SGLASS,
    E_BOTTLE,
    VSMVLNU,
    VSMVLND,
    VSDUPLN,
    VSDELLN,
    VSINSLN,
    VSLBCMT,
    VSLNCMC,
    VSLNCMU,
    VSWRDWP,
    VSLNSRT,
    VSLJOIN,
    VSMATCH,
    VSINFO,
    VSNOTIF,
    VSFIND,
    VSREPL,
    VSRNAME,
    VSGOTO,
    VSSOLU,
    VSERR,
    VSOUTPT,
    VSPUB,
    VSBASE,
    VSCLIPB,
    OSL_FKEY,
    NOKEY,
    CSTMTOG,
    KB_RESET,
    FLASH_KB,
    BOOTLDR,
    USERSPACE_END
};

// custom tap dances
enum {
    CAPS_LAYR   = 0,
    FN_OSL      = 1,
    RSFT_OSL    = 2,
    RALT_OSL    = 3,
    KB_UNLOCK   = 4,
    ACT_GRV     = 5,
    ACT_1       = 6,
    ACT_E       = 7,
    ACT_U       = 8,
    ACT_I       = 9,
    ACT_N       = 10,
    LGUI_OSL    = 11,
    RCMD_OSL    = 12,
    LOPT_OSL    = 13,
    ROPT_OSL    = 14,
    RCTL_OSL    = 15,
    MOUSE_ACCEL = 16,
    DYN_LAYR    = 17
};

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
    TRIPLE_HOLD = 6,
    QUAD_TAP = 7,
    PENT_TAP = 8,
    HEXA_TAP = 9
};

typedef union {
    uint32_t raw;
    struct {
        bool is_linux_base : 1;
        uint8_t rgb_mode   : 7;
    };
} user_config_t;

extern user_config_t user_config;

// key tracker
typedef struct {
    uint8_t index;
    bool press;
    int fade;
} keytracker;

// setup keytracker
extern deferred_token key_token;
extern keytracker tracked_keys[20];
extern uint8_t tk_length;

// for tracking wide-text options for the WIDE_TEXT_LAYR
enum {
    WIDE_STANDARD,
    WIDE_STHRU,
    WIDE_UNDERLN,
    WIDE_BARTEXT,
    WIDE_BBRTEXT
};
extern uint8_t wide_text_mode;
extern bool wide_firstchar;

extern tap_dance_action_t tap_dance_actions[18];
// functions associated with all tap dances
int cur_dance (tap_dance_state_t *state);
// functions associated with individual tap dances
void caps_finished (tap_dance_state_t *state, void *user_data);
void caps_reset (tap_dance_state_t *state, void *user_data);
void ralt_finished (tap_dance_state_t *state, void *user_data);
void ralt_reset (tap_dance_state_t *state, void *user_data);
void fn_finished (tap_dance_state_t *state, void *user_data);
void fn_reset (tap_dance_state_t *state, void *user_data);
void rsft_finished (tap_dance_state_t *state, void *user_data);
void rsft_reset (tap_dance_state_t *state, void *user_data);
void kbunlock_finished (tap_dance_state_t *state, void *user_data);
void kbunlock_reset (tap_dance_state_t *state, void *user_data);
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
void lgui_finished (tap_dance_state_t *state, void *user_data);
void lgui_reset (tap_dance_state_t *state, void *user_data);
void rcmd_finished (tap_dance_state_t *state, void *user_data);
void rcmd_reset (tap_dance_state_t *state, void *user_data);
void lopt_finished (tap_dance_state_t *state, void *user_data);
void lopt_reset (tap_dance_state_t *state, void *user_data);
void ropt_finished (tap_dance_state_t *state, void *user_data);
void ropt_reset (tap_dance_state_t *state, void *user_data);
void rctl_finished (tap_dance_state_t *state, void *user_data);
void rctl_reset (tap_dance_state_t *state, void *user_data);
void macl_finished (tap_dance_state_t *state, void *user_data);
void macl_reset (tap_dance_state_t *state, void *user_data);
void dyn_finished (tap_dance_state_t *state, void *user_data);
void dyn_reset (tap_dance_state_t *state, void *user_data);

extern deferred_token jiggler_token;
extern report_mouse_t jiggler_report;
extern bool ms_btn_held;
extern bool is_lopt_held;
extern bool fj_light;
extern bool hrow_light;
extern bool enable_keytracker;
extern bool color_test;
extern uint16_t color_test_timer;
extern const uint8_t monitored_macos_base_layers[];
extern const uint8_t monitored_macos_base_count;
extern int8_t macro_direction;
extern bool macro_recording;
extern bool is_macro_led_on;
extern uint16_t macro_timer;
extern deferred_token osl_macro_token;
extern bool oneshot_layer_active;
extern bool sim_osl;
extern deferred_token sim_osl_token;
extern bool is_cmd_tab_active;
extern bool is_cmd_shift_tab_active;
extern deferred_token cmd_tab_token;
extern bool is_in_leader_sequence;
extern bool is_leader_led_on;
extern uint16_t leader_timer;
extern bool is_leader_error;
extern bool is_leader_error_led_on;
extern uint16_t leader_error_timer;
extern deferred_token leader_error_token;
extern bool is_led_on;
extern uint16_t layer_timer;
extern bool is_layer_lock_led_on;
extern uint16_t layer_lock_timer;
extern bool is_key_lock_led_on;
extern uint16_t key_lock_timer;
extern bool os_changed;
extern uint16_t os_change_timer;
extern uint8_t saved_rgb_mode;
extern uint8_t act_char_led_index;
extern bool is_winkey_held;
extern uint8_t winkey_scut_keys[];
extern uint8_t winkey_scut_altcolor[];
extern uint8_t winkey_scut_keys_size;
extern uint8_t winkey_scut_altcolor_size;
extern uint8_t super_scut_keys[];
extern uint8_t super_scut_altcolor[];
extern uint8_t super_scut_keys_size;
extern uint8_t super_scut_altcolor_size;
extern deferred_token wireless_mode_token;
extern bool battery_drain_mode;

bool process_record_userspace(uint16_t keycode, keyrecord_t *record);
bool process_leader_userspace(void);
bool key_should_fade(keytracker key, uint8_t layer);
bool is_capslock_shifted(uint8_t i);
bool is_capsword_shifted(uint8_t i);
void set_animation_if_lock_layr(void);
void rgb_set_sleep_mode(bool enable);
void wireless_transport_enable(bool enable);
void wakeup_if_not_connected(void);
void send_key_to_wakeup(uint8_t delay_ms);
void jiggle_mouse(void);
void dual_key(uint16_t std_keycode, uint16_t alt_keycode, uint8_t mod_mask);
void symbol_key_mac(const char *unicode, const char *shift_unicode);
void symbol_key_win(const char *alt_code, const char *shift_alt_code);
void symbol_key_linux(const char *hex_code, const char *shift_hex_code);
void type_numpad_keys_from_string(const char *stringnum);
bool is_mac_base(void);
bool is_base_layer(uint8_t layer);
bool dip_switch_update_userspace(uint8_t index, bool active);
bool app_switch_active(void);
uint32_t extra_primer_poke(uint32_t trigger_time, void* cb_arg);
uint32_t osl_macro_callback(uint32_t trigger_time, void *cb_arg);
uint32_t sim_osl_callback(uint32_t trigger_time, void* cb_arg);
uint32_t cmd_tab_callback(uint32_t trigger_time, void* cb_arg);
uint32_t leader_error_callback(uint32_t trigger_time, void* cb_arg);
uint32_t wireless_mode_callback(uint32_t trigger_time, void *cb_arg);
