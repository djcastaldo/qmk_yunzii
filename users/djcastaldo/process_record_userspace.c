// process_record_userspace.c
// @davex 07/30/2025
// this is the start of moving some stuff to separate files to make it easier to move between keyboards 

#include "process_record_userspace.h"
#include "config.h"
#include "layers.h"
#include "keyindex.h"
#if defined(CONFIG_HAS_BASE_LAYER_TOGGLE)
#include "quantum.h"
#endif
#ifdef KEYBOARD_IS_BRIDGE
#include "wireless.h"
#endif
#if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
#include "wireless/battery.h"
#include "wireless/bat_level_animation.h"
#include "wireless/wireless.h"
#include "wireless/transport.h"
__attribute__((weak)) void bt_transport_enable(bool enable);
__attribute__((weak)) void p24g_transport_enable(bool enable);
#endif

user_config_t user_config;
#ifdef CONFIG_MACOS_BASE_LAYERS
const uint8_t macos_base_layers[] = CONFIG_MACOS_BASE_LAYERS;
#else
const uint8_t macos_base_layers[] = {};
#endif
#ifdef CONFIG_MACOS_BASE_LAYERS_COUNT
const uint8_t macos_base_layers_count = CONFIG_MACOS_BASE_LAYERS_COUNT;
#else
const uint8_t macos_base_layers_count = 0;
#endif
#ifdef CONFIG_ALL_BASE_LAYERS
const uint8_t all_base_layers[] = CONFIG_ALL_BASE_LAYERS;
#else
const uint8_t all_base_layers[] = {0};
#endif
#ifdef CONFIG_ALL_BASE_LAYERS_COUNT
const uint8_t all_base_layers_count = CONFIG_ALL_BASE_LAYERS_COUNT;
#else
const uint8_t all_base_layers_count = 1;
#endif
#ifdef CONFIG_RGB_LAYER_INDICATORS
const uint8_t rgb_layer_indicators[] = CONFIG_RGB_LAYER_INDICATORS;
#else
const uint8_t rgb_layer_indicators[] = {};
#endif
#ifdef CONFIG_RGB_LAYER_INDICATORS_COUNT
const uint8_t rgb_layer_indicators_count = CONFIG_RGB_LAYER_INDICATORS_COUNT;
#else
const uint8_t rgb_layer_indicators_count = 0;;
#endif
#ifdef CONFIG_EXTRA_BASE_LAYR_COLORS
const led_color_t extra_base_layr_colors[] = CONFIG_EXTRA_BASE_LAYR_COLORS;
#else
const led_color_t extra_base_layr_colors[] = {};
#endif
#ifdef CONFIG_EXTRA_BASE_LAYR_COLORS_COUNT
const uint8_t extra_base_layr_colors_count = CONFIG_EXTRA_BASE_LAYR_COLORS_COUNT;
#else
const uint8_t extra_base_layr_colors_count = 0;
#endif
#ifdef CONFIG_KEYLIGHT_ALT_COLOR_KEYS
const uint8_t keylight_alt_color_keys[] = CONFIG_KEYLIGHT_ALT_COLOR_KEYS;
#else
const uint8_t keylight_alt_color_keys[] = {};
#endif
#ifdef CONFIG_KEYLIGHT_ALT_COLOR_KEYS_COUNT
const uint8_t keylight_alt_color_keys_count = CONFIG_KEYLIGHT_ALT_COLOR_KEYS_COUNT;
#else
const uint8_t keylight_alt_color_keys_count = 0;
#endif
#ifdef CONFIG_LOCK_LAYR_EXTRA_FLASH_KEYS
const uint8_t lock_layr_extra_flash_keys[] = CONFIG_LOCK_LAYR_EXTRA_FLASH_KEYS;
#else
const uint8_t lock_layr_extra_flash_keys[] = {};
#endif
#ifdef CONFIG_LOCK_LAYR_EXTRA_FLASH_KEYS_COUNT
const uint8_t lock_layr_extra_flash_keys_count = CONFIG_LOCK_LAYR_EXTRA_FLASH_KEYS_COUNT;
#else
const uint8_t lock_layr_extra_flash_keys_count = 0;
#endif
#ifdef CONFIG_MAC_BASE_CHANGE_GROUP
const uint8_t mac_base_change_group[] = CONFIG_MAC_BASE_CHANGE_GROUP;
#else
const uint8_t mac_base_change_group[] = {};
#endif
#ifdef CONFIG_MAC_BASE_CHANGE_GROUP_COUNT
const uint8_t mac_base_change_group_count = CONFIG_MAC_BASE_CHANGE_GROUP_COUNT;
#else
const uint8_t mac_base_change_group_count = 0;
#endif
#ifdef CONFIG_WIN_BASE_CHANGE_GROUP
const uint8_t win_base_change_group[] = CONFIG_WIN_BASE_CHANGE_GROUP;
#else
const uint8_t win_base_change_group[] = {};
#endif
#ifdef CONFIG_WIN_BASE_CHANGE_GROUP_COUNT
const uint8_t win_base_change_group_count = CONFIG_WIN_BASE_CHANGE_GROUP_COUNT;
#else
const uint8_t win_base_change_group_count = 0;
#endif


__attribute__ ((weak))
bool rgb_matrix_indicators_keymap(uint8_t led_min, uint8_t led_max) {
  return false;
}

// setup keytracker
deferred_token key_token = INVALID_DEFERRED_TOKEN;
keytracker tracked_keys[20];
uint8_t tk_length = sizeof(tracked_keys) / sizeof(tracked_keys[0]);

// setup wide-text mode
uint8_t wide_text_mode = WIDE_STANDARD;
bool wide_firstchar = false;

// setup mouse jiggler
deferred_token jiggler_token = INVALID_DEFERRED_TOKEN;
report_mouse_t jiggler_report = {0};
// this was originally a static declaration in the switch case for MK_HOLD, but I also want to use it outside of
// that switch case to do rgb change, so am moving it here.
bool ms_btn_held;
bool is_lopt_held;
// for tracking whether to highlight home row keys f and j
bool fj_light;
// and for tracking if the full home row light is on
bool hrow_light;
// for disabling the keytracker, which will also disable key-reactive fade
bool enable_keytracker = true;
// if rgb color-test is requested, set this bool
bool color_test;
// and use a timer so that it can be turned off after a delay even if no further key is pressed
uint16_t color_test_timer;
// for tracking a recording macro
int8_t macro_direction;
bool macro_recording;
bool is_macro_led_on;
uint16_t macro_timer;
// and a delayed callback after playing a macro from osl
deferred_token osl_macro_token = INVALID_DEFERRED_TOKEN;
// for tracking if oneshot layer is active
bool oneshot_layer_active;
// setup a way to delay oneshot deactivation after a keycode that keeps oneshot active
// a one-shot extended time can be simulated by simply turning a layer on and using the
// callback to turn the layer off after a specified delay
bool sim_osl;
deferred_token sim_osl_token = INVALID_DEFERRED_TOKEN;
// for tracking cmd-tab app switching
bool is_cmd_tab_active;
bool is_cmd_shift_tab_active;
// setup cmd-tab app switching
deferred_token cmd_tab_token = INVALID_DEFERRED_TOKEN;
// for tracking if leader sequence is started
bool is_in_leader_sequence;
bool is_leader_led_on;
uint16_t leader_timer;
bool is_leader_error;
bool is_leader_error_led_on;
uint16_t leader_error_timer;
deferred_token leader_error_token = INVALID_DEFERRED_TOKEN;
extern bool leading;
extern uint8_t leader_sequence_size;
// for tracking whether to blink an led as an indicator, used to show which layer is active
bool is_led_on;
uint16_t layer_timer;
// for tracking layer lock in order to flash the layer lock indicator
bool is_layer_lock_led_on;
uint16_t layer_lock_timer;
// for tracking key lock blinking
bool is_key_lock_led_on;
uint16_t key_lock_timer;
// for tracking os and base layer changes
bool os_changed;
uint16_t os_change_timer;
#if defined(CONFIG_MAC_BASE_CHANGE_GROUP) || defined(CONFIG_WIN_BASE_CHANGE_GROUP)
// for tracking if the base layer was changed within a base layer group in order to flash some indicators
bool base_layer_changed_in_group;
static uint16_t base_change_timer;
#endif
// for tracking if an accent char tap dance should light up a particular key to show what the tap will send
uint8_t act_char_led_index = 0;
// use this to highlight keyboard shortcuts with rgb when winkey (or linux super) is held
// split some of these into another color since they are used rarely
bool is_winkey_held;
uint8_t winkey_scut_keys[] = {I_INDICATOR, I_N0, I_N1, I_N2, I_N3, I_N4, I_N5, I_N6, I_N7, I_N8, I_N9, I_TAB,
                              I_Q, I_E, I_R, I_A, I_D, I_L, I_X, I_B, I_N, I_LEFT, I_RIGHT, I_UP, I_DOWN};
uint8_t winkey_scut_altcolor[] = {I_MIN, I_PLUS, I_T, I_U, I_I, I_P, I_S, I_F, I_G, I_H, I_K, I_SEMI,
                                  I_Z, I_V, I_M, I_COMMA, I_DOT};
uint8_t winkey_scut_keys_size = sizeof(winkey_scut_keys) / sizeof(winkey_scut_keys[0]);
uint8_t winkey_scut_altcolor_size = sizeof(winkey_scut_altcolor) / sizeof(winkey_scut_altcolor[0]);
uint8_t super_scut_keys[] = {I_INDICATOR, I_N1, I_N2, I_N3, I_N4, I_N5, I_N6, I_N7, I_N8, I_N9, I_N0, I_TAB, I_Q, I_A, I_D, I_L, I_PGUP, I_PGDN, I_V};
uint8_t super_scut_altcolor[] = {I_GRV, I_UP, I_DOWN, I_LEFT, I_RIGHT};
uint8_t super_scut_keys_size = sizeof(super_scut_keys) / sizeof(super_scut_keys[0]);
uint8_t super_scut_altcolor_size = sizeof(super_scut_altcolor) / sizeof(super_scut_altcolor[0]);

#if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
#ifdef LK_WIRELESS_ENABLE
// setup this token to be used to create a delay from when wireless mode is changed until when key fade turns back on
// to see the wireless status indicator
deferred_token wireless_mode_token = INVALID_DEFERRED_TOKEN;
#endif
#elif defined(KEYBOARD_IS_BRIDGE)
#ifdef WIRELESS_ENABLE
// for tracking if a wireless indicator should show on any layer
// this gets set and elapsed time is tracked after a wireless or battery keycode is used
uint32_t wls_action_timer;
#endif
#endif
static bool rgb_indicators_enabled = true;
static bool housekeeping_restore_lock_anim = false;
#ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
static uint32_t lock_anim_timer = 0;
static bool lock_anim_active = false;
#endif
#ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
static uint32_t rgb_last_activity_timer = 0;
static bool rgb_reached_timeout = false;
static bool bt_is_on = true;
static bool warning_active = false;
static uint32_t last_blink = 0;
static bool warning_led_state = false;
#endif

#ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
void reset_rgb_timeout_timer(void) {
    rgb_last_activity_timer = timer_read32();

    #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
    if (get_highest_layer(layer_state) == LOCK_LAYR) {
        lock_anim_timer = timer_read32();
        lock_anim_active = true;
    }
    #endif

    // cancel warning if it was active
    if (warning_active) {
        warning_active = false;
    }

    // turn bt back on if it was off
    if (!bt_is_on) {
        #ifdef KEYBOARD_IS_BRIDGE
        wls_transport_enable(true);
        #elif defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        wakeup_if_not_connected();
        #endif
        #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        rgb_matrix_reload_from_eeprom(); // restore saved mode & brightness
        set_animation_if_lock_layr();
        #else
        suspend_wakeup_init_kb();
        #endif
        rgb_set_sleep_mode(false);
        bt_is_on = true;
    }
    // if not in suspend yet, but rgb is off, turn rgb back on
    else if (rgb_reached_timeout) {
        #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        rgb_matrix_reload_from_eeprom(); // restore saved mode & brightness
        set_animation_if_lock_layr();
        #else
        rgb_matrix_enable_noeeprom();
        #endif
        rgb_set_sleep_mode(false);
    }
}
#endif

#ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
void rgb_set_sleep_mode(bool enable) {
    rgb_reached_timeout = enable;
    rgb_indicators_enabled = !enable;
}
#endif

bool process_record_userspace(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;
#ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
    if (record->event.pressed) {
        reset_rgb_timeout_timer();
    }
#endif
    // record key index pressed for rgb reactive changes
    if (enable_keytracker && !is_macro_playing && keycode != QK_LEAD && keycode != KC_NO) {
        uint8_t key_idx = g_led_config.matrix_co[record->event.key.row][record->event.key.col];
#ifdef CONFIG_KEY_INDEX_MAX
        if (key_idx <= CONFIG_KEY_INDEX_MAX) {
#endif
#ifdef KEYBOARD_IS_YUNZII
        if (keycode != ENC_VOLU && keycode != ENC_VOLD && keycode != DUAL_ZOOMO && keycode != DUAL_ZOOMI &&
            keycode != ENC_SCROLLAPPL && keycode != ENC_SCROLLAPPR && keycode != ENC_RGBL && keycode != ENC_RGBR &&
            keycode != ENC_TSIZEL && keycode != ENC_TSIZER && keycode != ENC_MENUL && keycode != ENC_MENUR) {
#endif
            if (record->event.pressed) {
                dprintf("%u \n", key_idx);
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
            else {
                for (int i = 0; i < tk_length; i++) {
                    if (tracked_keys[i].index == key_idx) {
                        tracked_keys[i].press = false;
#ifdef CONFIG_KEYFADE_START_VAL
                        tracked_keys[i].fade = CONFIG_KEYFADE_START_VAL;
#else
                        tracked_keys[i].fade = 119;
#endif
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
#ifdef CONFIG_KEYFADE_CALLBACK_INTERVAL
                        return CONFIG_KEYFADE_CALLBACK_INTERVAL;
#else
                        return 12;  // Call the callback every 12ms
#endif
                    }
                    else {
                        return 0;
                    }
                }
#ifdef CONFIG_KEYFADE_START_DELAY
                key_token = defer_exec(CONFIG_KEYFADE_START_DELAY, keytracker_callback, NULL);
#else
                key_token = defer_exec(10, keytracker_callback, NULL);  // Schedule callback.
#endif
            }
#ifdef KEYBOARD_IS_YUNZII
        }
#endif
#ifdef CONFIG_KEY_INDEX_MAX
        }
#endif
    }

#ifndef CONFIG_CUSTOM_SLEEP_TIMEOUT
    #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
    // lock animation should come back if key pressed and then timeout again after set time
    if (get_highest_layer(layer_state) == LOCK_LAYR && record->event.pressed) {
        #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        wakeup_if_not_connected();
        #endif
        if (!rgb_matrix_is_enabled()) { // if currently off
            rgb_matrix_enable_noeeprom();
        }
        else if (!lock_anim_active) {
            rgb_matrix_reload_from_eeprom(); // restore saved mode & brightness
            set_animation_if_lock_layr();
        }
        lock_anim_timer = timer_read32();
        lock_anim_active = true;
        rgb_indicators_enabled = true;
    }
    #endif
#endif

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
            if (layer == TMUX_LAYR) {
                unregister_mods(mods); // temp remove mods
                tap_code16(C(KC_B));   // send ctrl-b before keycode processing
                register_mods(mods);   // reapply mods
            }

            // for some wide modes, should start with the spacing char
            if (layer == WIDE_LAYR && wide_firstchar) {
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
                    if (is_mac_base()) {
                        symbol_key_mac("00a6","");
                    }
                    else if (user_config.is_linux_base) {
                        symbol_key_linux("00a6","");
                        wait_ms(10);
                    }
                    else {
                        symbol_key_win("0166","");
                    }
                    break;
                default:
                    break;
                }
                register_mods(mods);   // reapply mods
                wide_firstchar = false;
            }

            // send keydown from the default layer
            register_code(keymap_key_to_keycode(biton32(default_layer_state), record->event.key));

            // if WIDE_TEXT_LAYER, add the extra spacing char
            if (layer == WIDE_LAYR) {
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
                    if (is_mac_base()) {
                        symbol_key_mac("00a6","");
                    }
                    else if (user_config.is_linux_base) {
                        symbol_key_linux("00a6","");
                    }
                    else {
                        symbol_key_win("0166","");
                    }
                    break;
                default:
                    tap_code16(KC_SPC);
                    break;
                }
                register_mods(mods);   // reapply mods
            }
        }
        else {
            unregister_code(keymap_key_to_keycode(biton32(default_layer_state), record->event.key));
        }
        return false;
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
        return false;
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
        return false;
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
        return false;
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
        return false;
    case WM_SYM:
        if (record->event.pressed) {
            if (is_mac_base()) {
                register_code(KC_LOPT);
                layer_on(MSYM_LAYR);
            }
            else {
                layer_on(WSYM_LAYR);
            }
        }
        else {
            if (is_mac_base()) {
                if (!is_layer_locked(MSYM_LAYR)) {
                    unregister_code(KC_LOPT);
                    layer_off(MSYM_LAYR);
                }
            }
            else {
                if (!is_layer_locked(WSYM_LAYR)) {
                    layer_off(WSYM_LAYR);
                }
            }
        }
        return false;
#if defined(CONFIG_MAC_BASE_CHANGE_GROUP) || defined(CONFIG_WIN_BASE_CHANGE_GROUP)
    case BASE_CHG:
        if (record->event.pressed) {
            if (is_mac_base()) {
                if (mac_base_change_group_count > 0) {
                    uint8_t current_base = get_highest_layer(default_layer_state);
                    for (uint8_t i = 0; i < mac_base_change_group_count; i++) {
                        if ((current_base == mac_base_change_group[i]) && (i + 1 < mac_base_change_group_count)) {
                            set_single_persistent_default_layer(mac_base_change_group[i + 1]);
                            layer_move(mac_base_change_group[i + 1]);
                            break;
                        }
                        else if (i + 1 == mac_base_change_group_count) {
                            set_single_persistent_default_layer(mac_base_change_group[0]);
                            layer_move(mac_base_change_group[0]);
                        }
                    }
                    base_layer_changed_in_group = true;
                }
            }
            else {
                if (win_base_change_group_count > 0) {
                    uint8_t current_base = get_highest_layer(default_layer_state);
                    for (uint8_t i = 0; i < win_base_change_group_count; i++) {
                        if ((current_base == win_base_change_group[i]) && (i + 1 < win_base_change_group_count)) {
                            set_single_persistent_default_layer(win_base_change_group[i + 1]);
                            layer_move(win_base_change_group[i + 1]);
                            break;
                        }
                        else if (i + 1 == win_base_change_group_count) {
                            set_single_persistent_default_layer(win_base_change_group[0]);
                            layer_move(win_base_change_group[0]);
                        }
                    }
                    base_layer_changed_in_group = true;
                }
            }
            return false;
        }
        break;
#endif
    case JIGGLE:
        if (record->event.pressed) {
            jiggle_mouse();
        }
        return false;
    case FJLIGHT:
        if (record->event.pressed) {
            // update the var used for f and j home key highlighting
            fj_light = !fj_light;
        }
        return false;
    case HROWLIGHT:
        if (record->event.pressed) {
        #ifndef CONFIG_HAS_LLOCK_KEY
            uint8_t layer = get_highest_layer(layer_state);
            if (get_mods() & MOD_MASK_CTRL) {
                if (!is_base_layer(layer)) {
                    if (is_layer_locked(layer))
                        layer_lock_off(layer);
                    else
                        layer_lock_on(layer);
                    return false;
                }
            }
        #endif
            // update the var used for full home row keys highlighting
            hrow_light = !hrow_light;
        }
        return false;
    case KTRACK:
        if (record->event.pressed) {
            // update the var used to enable/disable keytracker and per-key fade
            enable_keytracker = !enable_keytracker;
        }
        return false;
    case WAVE:  // Types ~=~=~=~=~=~ or <~>~<~>~<~>~<~>
        static deferred_token wave_token = INVALID_DEFERRED_TOKEN;
        static uint8_t wave_phase = 0;
        if (!record->event.pressed) {  // On release.
            const uint8_t mods = get_mods();
            cancel_deferred_exec(wave_token);
            wave_token = INVALID_DEFERRED_TOKEN;
            // ensure the pattern always ends on a ">"
            if (mods & (MOD_MASK_GUI | MOD_MASK_CTRL)) {  // cmd/ctl held?
                clear_mods();                  // remove mods
                if ((wave_phase & 1) == 0) {
                    send_string("<~>");
                }
                else {
                    tap_code16(KC_RABK);
                }
                register_mods(mods);           // restore mods
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
                if ((mods | oneshot_mods) & (MOD_MASK_GUI | MOD_MASK_CTRL)) {  // cmd/ctl held?
                    clear_mods();                    // remove mods
                    clear_oneshot_mods();            // remove oneshot mods
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
    case ARROW:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & (MOD_MASK_GUI | MOD_MASK_CTRL)) {  // cmd/ctl held?
                clear_mods();                   // remove mods
                clear_oneshot_mods();           // remove oneshot mods
                SEND_STRING("=>");
                register_mods(mods);            // restore mods
            } else {
                SEND_STRING("->");
            }
        }
        return false;
    case BSPCFAST:  // backspace with exponential repeating
        // initial delay before the first repeat
        static const uint8_t INIT_DELAY_MS = 250;
        // This array customizes the rate at which the Backspace key
        // repeats. The delay after the ith repeat is REP_DELAY_MS[i].
        // Values must be between 1 and 255.
        static const uint8_t REP_DELAY_MS[] PROGMEM = {
            99, 79, 65, 57, 49, 43, 40, 35, 33, 30, 28, 26, 25, 23, 22, 20,
            20, 19, 18, 17, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10};
        static deferred_token bspc_token = INVALID_DEFERRED_TOKEN;
        static uint8_t rep_count = 0;
        if (!record->event.pressed) {  // Backspace released: stop repeating
            cancel_deferred_exec(bspc_token);
            bspc_token = INVALID_DEFERRED_TOKEN;
        }
        else if (!bspc_token) {  // Backspace pressed: start repeating
            tap_code(KC_BSPC);  // Initial tap of Backspace key
            rep_count = 0;
            uint32_t bspc_callback(uint32_t trigger_time, void* cb_arg) {
              tap_code(KC_BSPC);
              if (rep_count < sizeof(REP_DELAY_MS)) { ++rep_count; }
              return pgm_read_byte(REP_DELAY_MS - 1 + rep_count);
            }
            bspc_token = defer_exec(INIT_DELAY_MS, bspc_callback, NULL);
        }
        return false;
    // tmux bound key list
    case TMUXLKEY:
        if (record->event.pressed) {
           //send_string(SS_LCTL("b") ":list-keys" SS_TAP(X_ENT));
           send_string(SS_LCTL("b") "?");
        }
        return false;
    // tmux command list
    case TMUXLCMD:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("list-commands\n");
        }
        return false;
    // tmux monitor window activity on
    case TMONON:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("setw monitor-activity on\n");
        }
        return false;
    // tmux monitor window activity off
    case TMONOF:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("setw monitor-activity off\n");
        }
        return false;
    // tmux visual activity alerts on
    case TVISON:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("setw -g visual-activity on\n");
        }
        return false;
    // tmux visual activity alerts off
    case TVISOF:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("setw -g visual-activity off\n");
        }
        return false;
    // tmux move window left 1 position
    case TWINLFT:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("swap-window -t -1\n");
           send_string_with_delay(SS_LCTL("b") "p",TMUX_DELAY);
        }
        return false;
    // tmux move window right 1 position
    case TWINRGT:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("swap-window -t +1\n");
           send_string_with_delay(SS_LCTL("b") "n",TMUX_DELAY);
        }
        return false;
    case TJPANE:
        if (record->event.pressed) {
           send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
           send_string("join-pane -t" SS_TAP(X_SPACE));
        }
        return false;
    // when encoder is pushed, either enable or disable window activity monitor
    case ENC_TMON:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            if (mods & MOD_MASK_CTRL) {
                unregister_mods(MOD_MASK_CTRL);             // remove control
                // turn on window actiivty monitor
                send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
                send_string("setw monitor-activity on\n");
                register_mods(mods);                        // add back mods
            }
            else {
                // turn off window actiivty monitor
                send_string_with_delay(SS_LCTL("b") ":",TMUX_DELAY);
                send_string("setw monitor-activity off\n");
            }
        }
        return false;
    // tmux encoder control
    // set this up to do resize l/r or u/d if control is held
    case ENC_TSIZEL:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            clear_mods();
            if (mods & MOD_MASK_CTRL) {
                if (mods & MOD_MASK_SHIFT) {
                    if (is_mac_base()) {
                        send_string_with_delay(SS_LCTL("b") ":", TMUX_DELAY);            // prefix with delay
                        send_string("resize-pane -U 1\n");                               // size up by 1
                    }
                    else {
                        send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_UP)),10);   // size up by 1
                    }
                }
                else {
                    send_string_with_delay(SS_LCTL("b") SS_LALT(SS_TAP(X_UP)),10);       // size up by 5
                }
            }
            else {
                if (mods & MOD_MASK_SHIFT) {
                    if (is_mac_base()) {
                        send_string_with_delay(SS_LCTL("b") ":", TMUX_DELAY);            // prefix with delay
                        send_string("resize-pane -L 1\n");                               // size left by 1
                    }
                    else {
                        send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_LEFT)),10); // size left by 1
                    }
                }
                else {
                    send_string_with_delay(SS_LCTL("b") SS_LALT(SS_TAP(X_LEFT)),10);     // size left by 5
                }
            }
            register_mods(mods);                                                         // add back mods
        }
        return false;
    case ENC_TSIZER:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            clear_mods();
            if (mods & MOD_MASK_CTRL) {
                if (mods & MOD_MASK_SHIFT) {
                    if (is_mac_base()) {
                        send_string_with_delay(SS_LCTL("b") ":", TMUX_DELAY);            // prefix with delay
                        send_string("resize-pane -D 1\n");                               // size down by 1
                    }
                    else {
                        send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_DOWN)),10); // size down by 1
                    }
                }
                else {
                    send_string_with_delay(SS_LCTL("b") SS_LALT(SS_TAP(X_DOWN)),10);     // size down by 5
                }
            }
            else {
                if (mods & MOD_MASK_SHIFT) {
                    if (is_mac_base()) {
                        send_string_with_delay(SS_LCTL("b") ":", TMUX_DELAY);             // prefix with delay
                        send_string("resize-pane -R 1\n");                                // size right by 1
                    }
                    else {
                        send_string_with_delay(SS_LCTL("b") SS_LCTL(SS_TAP(X_RIGHT)),10); // size right by 1
                    }
                }
                else {
                    send_string_with_delay(SS_LCTL("b") SS_LALT(SS_TAP(X_RIGHT)),10);     // size right by 5
                }
            }
            register_mods(mods);                                                          // add back mods
        }
        return false;
    case ENC_MUTEPLAY:
        //  insert delay if recording or playing a macro. otherwise, send mute or play/pause
        if (record->event.pressed) {
            if (macro_recording || is_macro_playing) {
                send_string(SS_DELAY(150));
            }
            else {
                dual_key(KC_MUTE,KC_MPLY,MOD_MASK_GUI | MOD_MASK_CTRL);
            }
        }
        break;
    case ENC_VOLD:
        if (record->event.pressed) {
            // standard: volume down, while command or control is held: keypad up
            dual_key(KC_VOLD,KC_UP,MOD_MASK_GUI | MOD_MASK_CTRL);
        }
        return false;
    case ENC_VOLU:
        if (record->event.pressed) {
            // standard: volume up, while command or control is held: keypad down
            dual_key(KC_VOLU,KC_DOWN,MOD_MASK_GUI | MOD_MASK_CTRL);
        }
        return false;
    case ENC_UNIMENU:
        if (record->event.pressed) {
            // sometimes this keycode is used on a layer where lopt might be held
            uint8_t mods = get_mods();
            // standard: unicode menu, while command or control is held: enter
            if (is_mac_base()) {
                if (mods & (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT))) {
                    del_mods(MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT));
                    dual_key(UNICODE,KC_ENT,MOD_MASK_GUI | MOD_MASK_CTRL);
                    set_mods(mods);
                }
                else {
                    dual_key(UNICODE,KC_ENT,MOD_MASK_GUI | MOD_MASK_CTRL);
                }
            }
            else if (user_config.is_linux_base) {
                dual_key(LCTL(KC_SCLN),KC_SPC,MOD_MASK_GUI | MOD_MASK_CTRL);
            }
            else {
                dual_key(LGUI(KC_SCLN),KC_SPC,MOD_MASK_GUI | MOD_MASK_CTRL);
            }
        }
        return false;
    case ENC_MENUL:
        if (record->event.pressed) {
            uint8_t mods = get_mods();
            if (mods & (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT))) {
                del_mods(MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT));
                dual_key(KC_UP,KC_LEFT,MOD_MASK_GUI | MOD_MASK_CTRL);
                set_mods(mods);
            }
            else {
                // standard: key up, while command or control is held: keypad left
                dual_key(KC_UP,KC_LEFT,MOD_MASK_GUI | MOD_MASK_CTRL);
            }
        }
        return false;
    case ENC_MENUR:
        if (record->event.pressed) {
            uint8_t mods = get_mods();
            if (mods & (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT))) {
                del_mods(MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT));
                dual_key(KC_DOWN,KC_RIGHT,MOD_MASK_GUI | MOD_MASK_CTRL);
                set_mods(mods);
            }
            else {
                // standard: key down, while command or control is held: keypad right
                dual_key(KC_DOWN,KC_RIGHT,MOD_MASK_GUI | MOD_MASK_CTRL);
            }
        }
        return false;
    case ENC_APPHIDE:
        if (record->event.pressed) {
            // standard or app switcher running: h (hide app windows), while cmd is held: mouse jiggler
            // or just hit enter if running windows or linux mode
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if (((mods | oneshot_mods) & (MOD_MASK_GUI | MOD_MASK_CTRL)) && !app_switch_active()) {
                jiggle_mouse();
            }
            else {
                // If token is already waiting to exec, cancel it.
                if (cmd_tab_token && app_switch_active()) {
                    cancel_deferred_exec(cmd_tab_token);
                    bool ctrl_removed = false;
                    if (mods & MOD_BIT(KC_LCTL)) {
                        del_mods(MOD_BIT(KC_LCTL));
                        ctrl_removed = true;
                    }
                    tap_code(is_mac_base() ? KC_H : KC_ENT);
                    if (ctrl_removed) {
                        register_mods(MOD_BIT(KC_LCTL));
                    }
                    cmd_tab_token = defer_exec(1000, cmd_tab_callback, NULL);  // Schedule callback.
                }
                else {   // if the button was pushed and appswitcher is not running, hide current app windows
                    send_string(SS_LCMD(SS_TAP(X_H)));
                }
            }
        }
        return false;
    case ENC_SCROLLAPPL:
        if (record->event.pressed) {
            // with command: app switch, standard: mouse wheel down
            const uint8_t mods = get_mods();
            if (mods & (MOD_MASK_GUI | MOD_BIT(KC_LCTL))) {
                bool ctrl_removed = false;
                if (mods & MOD_BIT(KC_LCTL)) {
                    del_mods(MOD_BIT(KC_LCTL));
                    ctrl_removed = true;
                }
                // If token is already waiting to exec, cancel it.
                if (cmd_tab_token) {
                    cancel_deferred_exec(cmd_tab_token);
                }
                if (!is_cmd_shift_tab_active) {
                    is_cmd_shift_tab_active = true;
                    is_cmd_tab_active = false;
                    register_code(is_mac_base() || user_config.is_linux_base ? KC_RCMD : KC_RALT);
                    register_code(KC_LSFT);
                }
                tap_code(KC_TAB);
                cmd_tab_token = defer_exec(1000, cmd_tab_callback, NULL);  // Schedule callback.
                if (ctrl_removed) {
                    register_mods(MOD_BIT(KC_LCTL));
                }
            }
            else {
                tap_code16(is_mac_base() ? KC_MS_WH_DOWN : KC_MS_WH_UP);
            }
        }
        return false;
    case ENC_SCROLLAPPR:
        if (record->event.pressed) {
            // with command: app switch, standard: mouse up
            const uint8_t mods = get_mods();
            if (mods & (MOD_MASK_GUI | MOD_BIT(KC_LCTL))) {
                bool ctrl_removed = false;
                if (mods  & MOD_BIT(KC_LCTL)) {
                    del_mods(MOD_BIT(KC_LCTL));
                    ctrl_removed = true;
                }
                // If token is already waiting to exec, cancel it.
                if (cmd_tab_token) {
                    cancel_deferred_exec(cmd_tab_token);
                }
                if (!is_cmd_tab_active) {
                    is_cmd_tab_active = true;
                    is_cmd_shift_tab_active = false;
                    register_code(is_mac_base() || user_config.is_linux_base ? KC_RCMD : KC_RALT);
                    unregister_code(KC_LSFT);
                }
                tap_code(KC_TAB);
                cmd_tab_token = defer_exec(1000, cmd_tab_callback, NULL);  // Schedule callback.
                if (ctrl_removed) {
                    register_mods(MOD_BIT(KC_LCTL));
                }
            }
            else {
                tap_code16(is_mac_base() ? KC_MS_WH_UP : KC_MS_WH_DOWN);
            }
        }
        return false;
    case ENC_RGBPUSH:
        if (record->event.pressed) {
            // standard: reset all, if control is held: hue defualt,
            // if opt/gui is held: sat default, if shift is held: speed defualt
            // if control and opt/gui are both held: mode default
            // get current mod states
            const uint8_t mods = get_mods();
            if (mods & MOD_MASK_SHIFT) {
                rgb_matrix_set_speed(RGB_MATRIX_DEFAULT_SPD);
            }
            else if ((mods & MOD_MASK_CTRL) && (mods & (MOD_MASK_ALT | MOD_MASK_GUI))) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_mode(RGB_MATRIX_DEFAULT_MODE);
            }
            else if (mods & MOD_MASK_CTRL) {
                rgb_matrix_sethsv(RGB_MATRIX_DEFAULT_HUE, rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
            }
            else if (mods & (MOD_MASK_ALT | MOD_MASK_GUI)) {
                rgb_matrix_sethsv(rgb_matrix_get_hue(), RGB_MATRIX_DEFAULT_SAT, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
            }
            // otherwise reset everything
            else {
                rgb_matrix_mode(RGB_MATRIX_DEFAULT_MODE);
                rgb_matrix_set_speed(RGB_MATRIX_DEFAULT_SPD);
                rgb_matrix_sethsv(RGB_MATRIX_DEFAULT_HUE, RGB_MATRIX_DEFAULT_SAT, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
            }
        }
        return false;
    // these have been further adjusted to do max brightness if anything other than brightness is
    // being adjusted.  This is good to best see the changes since the backlights are dimmed when
    // not on the base layer.
    case ENC_RGBL:
        if (record->event.pressed) {
            // standard: bri down, if control is held: hue down,
            // if opt/gui is held: sat down, if shift is held: speed down
            // if control and opt/gui are both held: mode reverse
            // get current mod states
            const uint8_t mods = get_mods();
            if (mods & MOD_MASK_SHIFT) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_decrease_speed();
            }
            else if ((mods & MOD_MASK_CTRL) && (mods & (MOD_MASK_ALT | MOD_MASK_GUI))) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_step_reverse();
            }
            else if (mods & MOD_MASK_CTRL) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_decrease_hue();
            }
            else if (mods & (MOD_MASK_ALT | MOD_MASK_GUI)) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_decrease_sat();
            }
            else {
                rgb_matrix_decrease_val();
            }
        }
        return false;
    case ENC_RGBR:
        if (record->event.pressed) {
            // standard: bri up, while control is held: hue up, while opt/gui is held: sat up
            // if shift is held: speed up, if control and opt/gui are both held: mode forward
            // get current mod states
            const uint8_t mods = get_mods();
            if (mods & MOD_MASK_SHIFT) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_increase_speed();
            }
            else if ((mods & MOD_MASK_CTRL) && (mods & (MOD_MASK_ALT | MOD_MASK_GUI))) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_step();
            }
            else if (mods & MOD_MASK_CTRL) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_increase_hue();
            }
            else if (mods & (MOD_MASK_ALT | MOD_MASK_GUI)) {
                rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
                rgb_matrix_increase_sat();
            }
            else {
                rgb_matrix_increase_val();
            }
        }
        return false;
    // form zoom reset
    case F_ZOOMR:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            clear_mods();
            tap_code16(is_mac_base() ? LCMD(KC_0) : LCTL(KC_0));
            register_mods(mods);
        }
        return false;
    // set up some different zoom (when control is used) so the zoom knob can be used with multiple apps
    // that support different ways to zoom
    case DUAL_ZOOMI:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                if (is_mac_base()) {
                    unregister_mods(MOD_MASK_CTRL);
                    tap_code16(LCMD(KC_EQL));
                    register_mods(mods);
                }
                else {
                    tap_code(KC_EQL);
                }
            }
            else {
                tap_code16(is_mac_base() ? LCMD(KC_MS_WH_DOWN) : LCTL(KC_MS_WH_UP));
            }
        }
        return false;
    case DUAL_ZOOMO:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                if (is_mac_base()) {
                    unregister_mods(MOD_MASK_CTRL);
                    tap_code16(LCMD(KC_MINS));
                    register_mods(mods);
                }
                else {
                    tap_code(KC_MINS);
                }
            }
            else {
                tap_code16(is_mac_base() ? LCMD(KC_MS_WH_UP) : LCTL(KC_MS_WH_DOWN));
            }
        }
        return false;
    // switch rgb modes with max brightness, since the KCTL_LAYR auto dims when initally switching to it
    case RGB_MOD:
    case RGB_RMOD:
    case RGB_SPD:
    case RGB_SPI:
        if (record->event.pressed)
            rgb_matrix_sethsv_noeeprom(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
        break;
    case DUAL_PLUSMIN:
        if (record->event.pressed) {
            // standard: plus symbol, while control is held: minus
            dual_key(KC_PPLS, KC_PMNS, MOD_MASK_CTRL);
        }
        return false;
    case DUAL_MULTDIV:
        if (record->event.pressed) {
            // standard: asterisk, while control is held: divide
            dual_key(KC_PAST, KC_PSLS, MOD_MASK_CTRL);
        }
        return false;
    case DUAL_ESC:
        if (record->event.pressed) {
            if (is_in_leader_sequence) {
                leading = false;
                leader_sequence_size = 0;
                is_in_leader_sequence = false;
            }
            else if (macro_recording) {
                // if macro is recording, stop it
                dynamic_macro_stop_recording();
            }
            else {
                // send escape
                register_code(KC_ESC);
            }
        }
        else {
            unregister_code(KC_ESC);
        }
        return false;
    // this is setup so I can use F12 key to insert a delay while recording a macro
    case DUAL_F12:
        if (record->event.pressed) {
           if (!macro_recording && !is_macro_playing) {
               // send F12
               tap_code(KC_F12);
           }
           else {
               // if macro is recording or playing, insert a delay
               // wait_ms(150); // this only works when wired
               send_string(SS_DELAY(150)); // this works wired and wireless
           }
        }
        return false;
    // this allows a running macro to be stopped using the macro key
    case DUAL_F13:
        if (record->event.pressed) {
           if (!macro_recording) {
               // send F13
               tap_code(KC_F13);
           }
           else {
               // if macro is recording, stop it
               dynamic_macro_stop_recording();
           }
        }
        return false;
    case DUAL_F14:
        if (record->event.pressed) {
           if (!macro_recording) {
               // send F14
               tap_code(KC_F14);
           }
           else {
               // if macro is recording, stop it
               dynamic_macro_stop_recording();
           }
        }
        return false;
    // this is setup so I can use numpad - to insert a delay while recording a macro
    case DUAL_PMNS:
        if (record->event.pressed) {
           if (!macro_recording && !is_macro_playing) {
               // send PMNS
               register_code(KC_PMNS);
           }
           else {
               // if macro is recording or playing, insert a delay
               // wait_ms(150); // this only works when wired
               send_string(SS_DELAY(150)); // this works wired and wireless
           }
        }
        else {
           if (!macro_recording && !is_macro_playing) {
               unregister_code(KC_PMNS);
           }
        }
        return false;
    case VI_REPLACE:
        if (record->event.pressed) {
           // start syntax for vi search and replace
           send_string(":%s///g" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
        }
        return false;
    case GIT_ADD:
        if (record->event.pressed) {
           // add updates to git commit
           send_string("git add -A" SS_TAP(X_ENT));
        }
        return false;
    case GIT_COMMIT:
        if (record->event.pressed) {
           // commit changes
           send_string("git commit -m \"\"" SS_TAP(X_LEFT));
        }
        return false;
    case GIT_PUSH:
        if (record->event.pressed) {
           // push commit to git repo
           send_string("git push" SS_TAP(X_ENT));
        }
        return false;
    case GIT_CHKOUT:
        if (record->event.pressed) {
           // checkout last commit from git repo
           send_string("git checkout ." SS_TAP(X_ENT));
        }
        return false;
    case GIT_LOG:
        if (record->event.pressed) {
           // show the git log
           send_string("git log" SS_TAP(X_ENT));
        }
        return false;
    case MK_HOLD:
        if (record->event.pressed) {
            if (!ms_btn_held) {
                register_code(KC_MS_BTN1);
                ms_btn_held = true;
            }
            else {
                unregister_code(KC_MS_BTN1);
                ms_btn_held = false;
            }
        }
        break;
    case KC_MS_BTN1:
        ms_btn_held = record->event.pressed;
        break; // this can continue processing
    case MK_ACCEL0:
        tap_code(record->event.pressed ? KC_MS_ACCEL0 : KC_MS_ACCEL1);
        return false;
    case MK_ACCEL2:
        tap_code(record->event.pressed ? KC_MS_ACCEL2 : KC_MS_ACCEL1);
        return false;
    case KC_MYCM:
        if (is_mac_base() && record->event.pressed) {
            // open new Finder home dir
            tap_code16(LCMD(LSFT(KC_H)));
            return false;
        }
        break;
    case KC_CALC:
        if (is_mac_base() && record->event.pressed) {
            // send command + control + * then delay and then h (setup to open calculator pro)
            send_string(SS_LCTL(SS_LCMD(SS_TAP(X_PAST))) SS_DELAY(35) "h");
            return false;
        }
        break;
    case KC_APP:
        if (is_mac_base() && record->event.pressed) {
            tap_code(KC_LPAD);
            return false;
        }
        break;
    case LOCKSCR:
        if (record->event.pressed) {
            if (is_mac_base()) {
                // send control + command + q
                send_string(SS_LCTL(SS_LCMD("q")) SS_DELAY(300) SS_LCTL(SS_LCMD("q")));
            }
            else {
                send_string(SS_LGUI("l"));
            }
        }
        return false;
    case BACKDIR:
        if (record->event.pressed) {
            // command to go back a dir in terminal
            send_string("cd .." SS_TAP(X_ENT));
        }
        return false;
    case HOMEDIR:
        if (record->event.pressed) {
            // command to go home in terminal
            send_string("cd ~" SS_TAP(X_ENT));
        }
        return false;
    case LSLTRAH:
        if (record->event.pressed) {
            // command to ls -ltrah in terminal
            send_string("ls -ltrah" SS_TAP(X_ENT));
        }
        return false;
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
    // volume up and down should be able to be pressed multiple times without cancelling a oneshot layer
    case KC_VOLD:
    case KC_VOLU:
        if (oneshot_layer_active || sim_osl) {
            reset_oneshot_layer();
            if (sim_osl_token) {
                cancel_deferred_exec(sim_osl_token);
                sim_osl_token = INVALID_DEFERRED_TOKEN;
            }
            if (record->event.pressed) {
                sim_osl = true;
                uint8_t layer = get_highest_layer(layer_state);
                layer_on(layer); // simulate that oneshot is still going
            }
            else {   // key release should use a delay for layer deactivation
                     // this turns off the layer if further volume controls are not used within 500ms
                sim_osl_token = defer_exec(500, sim_osl_callback, NULL);
            }
        }
        break;
    // functionality for opt keys that may use lopt with holds for mouse control or on MSYM_LAYR
    case KC_LOPT:
        if (is_mac_base() && !record->event.pressed) {
            is_lopt_held = false;
        }
    // intentionally no break here
    case KC_ROPT:
        if (is_mac_base()) {
            if (record->event.pressed) {
                if (get_highest_layer(layer_state) < 3) {
                    layer_on(MSYM_LAYR);
                }
            }
            else {
                if (is_layer_locked(MSYM_LAYR)) {
                    return false;
                }
                else {
                    layer_off(MSYM_LAYR);
                }
            }
        }
        break;
    case SSMENU:
        if (record->event.pressed) {
           // send shift + command + 5 (for screenshot with options menus)
           send_string(SS_LSFT(SS_LCMD("5")));
        }
        return false;
    case GNEWS:
        if (record->event.pressed) {
           // open browser tab to google news
           send_string_with_delay(SS_LCMD(SS_TAP(X_L)) SS_DELAY(150) "news.google.com" SS_DELAY(50) SS_LOPT(SS_TAP(X_ENT)),5);
        }
        return false;
    // move mouse cursor for per-monitor mission control
    case KC_UP:
    case KC_LEFT:
    case KC_RIGHT:
        if (record->event.pressed) {
           // check which control is being held and mouse mouse to monitor with CatchMouse
           if (get_mods() == MOD_BIT(KC_LCTL)) {
               send_string(SS_LOPT(SS_LCMD(SS_LSFT(SS_TAP(X_P1)))));
           }
           else if (get_mods() == MOD_BIT(KC_RCTL)) {
               send_string(SS_LOPT(SS_LCMD(SS_LSFT(SS_TAP(X_P2)))));
           }
        }
        break;
    // custom keycode to move cursor to left mon with CatchMouse
    case CURSORL:
        if (record->event.pressed) {
           send_string(SS_LCTL(SS_LOPT(SS_LCMD(SS_LSFT(SS_TAP(X_P1))))));
        }
        return false;
    // custom keycode to move cursor to right mon with CatchMouse
    case CURSORR:
        if (record->event.pressed) {
           send_string(SS_LCTL(SS_LOPT(SS_LCMD(SS_LSFT(SS_TAP(X_P2))))));
        }
        return false;
    case OPT_HOLD:
        if (record->event.pressed) {
            if (!is_lopt_held) {
                register_code(KC_LOPT);
                is_lopt_held = true;
            }
            else {
                unregister_code(KC_LOPT);
                is_lopt_held = false;
            }
        }
        return false;
    // use cmd + screenshot as key lock start
    case DUAL_SNAP:
        if (record->event.pressed) {
            if (get_mods() & MOD_MASK_GUI)
                set_key_lock_watching();
            else
                send_string(SS_LSFT(SS_LCMD(SS_TAP(X_4)))); // KC_SNAP wasn't working here
        }
        return false;
    case AP_GLOB:
        // setup for apple globe key to work
        host_consumer_send(record->event.pressed ? AC_NEXT_KEYBOARD_LAYOUT_SELECT : 0);
        return false;
    case COLORTEST:
        if (record->event.pressed) {
            color_test_timer = timer_read();
            color_test = true;
        }
        return false;
    // need this on msym layer for keyboards that don't have a grave key
    case MSYMGRV:
        if (record->event.pressed) {
            tap_code(KC_GRV);
        }
        return false;
    // the following OPT keycodes mimic a macos option os layer for symbols and accents
    // symbol_key_win() is a fn to type a windows alt code on the numpad
    // symbol_key_linux() uses hex codes to type the same symbols on linux
    case OPT2:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2122","20ac");
            }
            else {
                symbol_key_win("0153","0128");
            }
        }
        return false;
    case OPT3:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00a3","2039");
            }
            else {
                symbol_key_win("156","0139");
            }
        }
        return false;
    case OPT4:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00a2","203a");
            }
            else {
                symbol_key_win("155","0155");
            }
        }
        return false;
    case OPT5:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("221e","fb01");
            }
            else {
                symbol_key_win("236","64257");
            }
        }
        return false;
    case OPT6:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00a7","fb02");
            }
            else {
                symbol_key_win("21","64258");
            }
        }
        return false;
    case OPT7:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00b6","2021");
            }
            else {
                symbol_key_win("20","0135");
            }
        }
        return false;
    case OPT8:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2022","00b0");
            }
            else {
                symbol_key_win("7","248");
            }
        }
        return false;
    case OPT9:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00aa","00b7");
            }
            else {
                symbol_key_win("166","9");
            }
        }
        return false;
    case OPT0:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00ba","201a");
            }
            else {
                symbol_key_win("167","0164");
            }
        }
        return false;
    case OPTMIN:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2013","2014");
            }
            else {
                symbol_key_win("0151","22");
            }
        }
        return false;
    case OPTEQ:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2260","00b1");
            }
            else {
                symbol_key_win("8800","241");
            }
        }
        return false;
    case OPTQ:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("0153","0152");
            }
            else {
                symbol_key_win("0156","0140");
            }
        }
        return false;
    case OPTW:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2211","201e");
            }
            else {
                symbol_key_win("228","0132");
            }
        }
        return false;
    case OPTR:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00ae","2030");
            }
            else {
                symbol_key_win("0174","0137");
            }
        }
        return false;
    case OPTT:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2020","02c7");
            }
            else {
                symbol_key_win("0134","259");
            }
        }
        return false;
    case OPTY:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00a5","00c1");
            }
            else {
                symbol_key_win("157","0193");
            }
        }
        return false;
    case OPTO:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00f8","00d8");
            }
            else {
                symbol_key_win("0248","0216");
            }
        }
        return false;
    case OPTP:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("03c0","03a0");
            }
            else {
                symbol_key_win("227","928");
            }
        }
        return false;
    case OPTLBR:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("201c","201d");
            }
            else {
                symbol_key_win("0147","0148");
            }
        }
        return false;
    case OPTRBR:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2018","2019");
            }
            else {
                symbol_key_win("0145","0146");
            }
        }
        return false;
    case OPTBSL:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00ab","00bb");
            }
            else {
                symbol_key_win("174","175");
            }
        }
        return false;
    case OPTA:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00e5","00c5");
            }
            else {
                symbol_key_win("0229","0197");
            }
        }
        return false;
    case OPTS:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00df","00cd");
            }
            else {
                symbol_key_win("225","0205");
            }
        }
        return false;
    case OPTD:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2202","00ce");
            }
            else {
                symbol_key_win("8706","0206");
            }
        }
        return false;
    case OPTF:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("0192","00cf");
            }
            else {
                symbol_key_win("159","0207");
            }
        }
        return false;
    case OPTG:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00a9","02dd");
            }
            else {
                symbol_key_win("0169","0180");
            }
        }
        return false;
    case OPTH:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("02d9","00d3");
            }
            else {
                symbol_key_win("0183","0211");
            }
        }
        return false;
    case OPTJ:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2206","00d4");
            }
            else {
                symbol_key_win("30","0212");
            }
        }
        return false;
    case OPTK:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("02da","03f0");
            }
            else {
                symbol_key_win("0186","0208");
            }
        }
        return false;
    case OPTL:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00ac","00d2");
            }
            else {
                symbol_key_win("170","0210");
            }
        }
        return false;
    case OPTSEM:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2026","00da");
            }
            else {
                symbol_key_win("0133","0218");
            }
        }
        return false;
    case OPTAPO:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00e6","00c6");
            }
            else {
                symbol_key_win("0230","0198");
            }
        }
        return false;
    case OPTZ:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("03a9","00b8");
            }
            else {
                symbol_key_win("234","0184");
            }
        }
        return false;
    case OPTX:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2248","02db");
            }
            else {
                symbol_key_win("247","0215");
            }
        }
        return false;
    case OPTC:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00e7","00c7");
            }
            else {
                symbol_key_win("0231","0199");
            }
        }
        return false;
    case OPTV:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("221a","25ca");
            }
            else {
                symbol_key_win("251","4");
            }
        }
        return false;
    case OPTB:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("222b","0131");
            }
            else {
                symbol_key_win("8747","0305");
            }
        }
        return false;
    case OPTM:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("03bc","00c2");
            }
            else {
                symbol_key_win("230","0194");
            }
        }
        return false;
    case OPTCOM:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2264","00af");
            }
            else {
                symbol_key_win("243","0175");
            }
        }
        return false;
    case OPTDOT:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("2265","02d8");
            }
            else {
                symbol_key_win("242","0168");
            }
        }
        return false;
    case OPTSLS:
        if (record->event.pressed) {
            if (user_config.is_linux_base) {
                symbol_key_linux("00f7","00bf");
            }
            else {
                symbol_key_win("0247","168");
            }
        }
        return false;
    case SUITH:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("2665","2661");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("2665","2661");
            }
            else {
                symbol_key_win("3","3");
            }
        }
        return false;
    case SUITD:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("2666","2662");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("2666","2662");
            }
            else {
                symbol_key_win("4","4");
            }
        }
        return false;
    case SUITC:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("2663","2667");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("2663","2667");
            }
            else {
                symbol_key_win("5","5");
            }
        }
        return false;
    case SUITS:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("2660","2664");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("2660","2664");
            }
            else {
                symbol_key_win("6","6");
            }
        }
        return false;
    case SUP1:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("00b9","2074");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("00b9","2074");
            }
            else {
                symbol_key_win("0185","0185");
            }
        }
        return false;
    case SUP2:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("00b2","2075");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("00b2","2075");
            }
            else {
                symbol_key_win("0178","0178");
            }
        }
        return false;
    case SUP3:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("00b3","2076");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("00b3","2076");
            }
            else {
                symbol_key_win("0179","0179");
            }
        }
        return false;
    case NBSP:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("00a0","00a6");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("00a0","00a6");
            }
            else {
                symbol_key_win("0160","0166");
            }
        }
        return false;
    case CIRCL1:
        if (record->event.pressed) {
            // check mods first so can do a double-cirled number if alt is held
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24f5","24f5");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24f5","24f5");
                }
                else {
                    symbol_key_win("9461","9461");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2460","246a");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2460","246a");
                }
                else {
                    symbol_key_win("9312","9322");
                }
            }
        }
        return false;
    case CIRCL2:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24f6","24f6");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24f6","24f6");
                }
                else {
                    symbol_key_win("9462","9462");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2461","246b");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2461","246b");
                }
                else {
                    symbol_key_win("9313","9323");
                }
            }
        }
        return false;
    case CIRCL3:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24f7","24f7");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24f7","24f7");
                }
                else {
                    symbol_key_win("9463","9463");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2462","246c");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2462","246c");
                }
                else {
                    symbol_key_win("9314","9324");
                }
            }
        }
        return false;
    case CIRCL4:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24f8","24f8");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24f8","24f8");
                }
                else {
                    symbol_key_win("9464","9464");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2463","246d");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2463","246d");
                }
                else {
                    symbol_key_win("9315","9325");
                }
            }
        }
        return false;
    case CIRCL5:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24f9","24f9");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24f9","24f9");
                }
                else {
                    symbol_key_win("9465","9465");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2464","246e");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2464","246e");
                }
                else {
                    symbol_key_win("9316","9326");
                }
            }
        }
        return false;
    case CIRCL6:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24fa","24fa");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24fa","24fa");
                }
                else {
                    symbol_key_win("9466","9466");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2465","246f");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2465","246f");
                }
                else {
                    symbol_key_win("9317","9327");
                }
            }
        }
        return false;
    case CIRCL7:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24fb","24fb");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24fb","24fb");
                }
                else {
                    symbol_key_win("9467","9467");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2466","2470");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2466","2470");
                }
                else {
                    symbol_key_win("9318","9328");
                }
            }
        }
        return false;
    case CIRCL8:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24fc","24fc");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24fc","24fc");
                }
                else {
                    symbol_key_win("9468","9468");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2467","2471");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2467","2471");
                }
                else {
                    symbol_key_win("9319","9329");
                }
            }
        }
        return false;
    case CIRCL9:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) {
                    symbol_key_mac("24fd","24fd");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24fd","24fd");
                }
                else {
                    symbol_key_win("9469","9469");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("2468","2472");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("2468","2472");
                }
                else {
                    symbol_key_win("9320","9330");
                }
            }
        }
        return false;
    case CIRCL0:
        if (record->event.pressed) {
            const uint8_t mods = get_mods();
            const uint8_t oneshot_mods = get_oneshot_mods();
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                if (is_mac_base()) { // this one can do both double circle 10 and a circle 20
                    symbol_key_mac("24fe","2473");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24fe","2473");
                }
                else {
                    symbol_key_win("9470","9331");
                }
                register_mods(mods);
            } else {
                if (is_mac_base()) {
                    symbol_key_mac("24ea","2469");
                }
                else if (user_config.is_linux_base) {
                    symbol_key_linux("24ea","2469");
                }
                else {
                    symbol_key_win("9450","9321");
                }
            }
        }
        return false;
    case CIRCLQ:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e0","24c6");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e0","24c6");
            }
            else {
                symbol_key_win("9440","9414");
            }
        }
        return false;
    case CIRCLW:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e6","24cc");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e6","24cc");
            }
            else {
                symbol_key_win("9446","9420");
            }
        }
        return false;
    case CIRCLE:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d4","24ba");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d4","24ba");
            }
            else {
                symbol_key_win("9428","9402");
            }
        }
        return false;
    case CIRCLR:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e1","24c7");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e1","24c7");
            }
            else {
                symbol_key_win("9441","9415");
            }
        }
        return false;
    case CIRCLT:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e3","24c9");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e3","24c9");
            }
            else {
                symbol_key_win("9443","9417");
            }
        }
        return false;
    case CIRCLY:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e8","24ce");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e8","24ce");
            }
            else {
                symbol_key_win("9448","9422");
            }
        }
        return false;
    case CIRCLU:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e4","24ca");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e4","24ca");
            }
            else {
                symbol_key_win("9444","9418");
            }
        }
        return false;
    case CIRCLI:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d8","24be");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d8","24be");
            }
            else {
                symbol_key_win("9432","9406");
            }
        }
        return false;
    case CIRCLO:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24de","24c4");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24de","24c4");
            }
            else {
                symbol_key_win("9438","9412");
            }
        }
        return false;
    case CIRCLP:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24df","24c5");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24df","24c5");
            }
            else {
                symbol_key_win("9439","9413");
            }
        }
        return false;
    case CIRCLA:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d0","24b6");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d0","24b6");
            }
            else {
                symbol_key_win("9424","9398");
            }
        }
        return false;
    case CIRCLS:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e2","24c8");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e2","24c8");
            }
            else {
                symbol_key_win("9442","9416");
            }
        }
        return false;
    case CIRCLD:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d3","24b9");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d3","24b9");
            }
            else {
                symbol_key_win("9427","9401");
            }
        }
        return false;
    case CIRCLF:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d5","24bb");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d5","24bb");
            }
            else {
                symbol_key_win("9429","9403");
            }
        }
        return false;
    case CIRCLG:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d6","24bc");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d6","24bc");
            }
            else {
                symbol_key_win("9430","9404");
            }
        }
        return false;
    case CIRCLH:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d7","24bd");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d7","24bd");
            }
            else {
                symbol_key_win("9431","9405");
            }
        }
        return false;
    case CIRCLJ:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d9","24bf");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d9","24bf");
            }
            else {
                symbol_key_win("9433","9407");
            }
        }
        return false;
    case CIRCLK:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24da","24c0");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24da","24c0");
            }
            else {
                symbol_key_win("9434","9408");
            }
        }
        return false;
    case CIRCLL:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24db","24c1");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24db","24c1");
            }
            else {
                symbol_key_win("9435","9409");
            }
        }
        return false;
    case CIRCLZ:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e9","24cf");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e9","24cf");
            }
            else {
                symbol_key_win("9449","9423");
            }
        }
        return false;
    case CIRCLX:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e7","24cd");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e7","24cd");
            }
            else {
                symbol_key_win("9447","9421");
            }
        }
        return false;
    case CIRCLC:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d2","24b8");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d2","24b8");
            }
            else {
                symbol_key_win("9426","9400");
            }
        }
        return false;
    case CIRCLV:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24e5","24cb");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24e5","24cb");
            }
            else {
                symbol_key_win("9445","9419");
            }
        }
        return false;
    case CIRCLB:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24d1","24b7");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24d1","24b7");
            }
            else {
                symbol_key_win("9425","9399");
            }
        }
        return false;
    case CIRCLN:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24dd","24c3");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24dd","24c3");
            }
            else {
                symbol_key_win("9437","9411");
            }
        }
        return false;
    case CIRCLM:
        if (record->event.pressed) {
            if (is_mac_base()) {
                symbol_key_mac("24dc","24c2");
            }
            else if (user_config.is_linux_base) {
                symbol_key_linux("24dc","24c2");
            }
            else {
                symbol_key_win("9436","9410");
            }
        }
        return false;
    case E_SMILE:
	if (record->event.pressed) {
            symbol_key_mac("d83dde00","d83dde06");
	}
	return false;
    case E_SMIRK:
	if (record->event.pressed) {
            symbol_key_mac("d83dde0f","d83dde09");
	}
	return false;
    case E_ASTON:
	if (record->event.pressed) {
            symbol_key_mac("d83dde32","d83dde27");
	}
	return false;
    case E_SAD:
	if (record->event.pressed) {
            symbol_key_mac("d83dde1f","d83dde1e");
	}
	return false;
    case E_CAR:
	if (record->event.pressed) {
            symbol_key_mac("d83dde99","d83ddea2");
	}
	return false;
    case E_BUS:
	if (record->event.pressed) {
            symbol_key_mac("d83dde8e","d83ddee9");
	}
	return false;
    case E_TRAIN:
	if (record->event.pressed) {
            symbol_key_mac("d83dde8a","d83ddeb2");
	}
	return false;
    case E_TAXI:
	if (record->event.pressed) {
            symbol_key_mac("d83dde95","d83ddef4");
	}
	return false;
    case E_FLAGUS:
	if (record->event.pressed) {
            symbol_key_mac("d83cddfad83cddf8","d83cddeed83cddf9");
	}
	return false;
    case E_FLAGPI:
	if (record->event.pressed) {
            symbol_key_mac("d83cddf5d83cdded","d83cdde8d83cddf3");
	}
	return false;
    case E_GLOBE:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf0e","d83cdf10");
	}
	return false;
    case E_PIZZA:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf55","d83cdf69");
	}
	return false;
    case E_BURGER:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf54","d83cdf5f");
	}
	return false;
    case E_CHICKEN:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf57","d83cdf2d");
	}
	return false;
    case E_TACO:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf2e","d83cdf66");
	}
	return false;
    case E_SNAIL:
	if (record->event.pressed) {
            symbol_key_mac("d83ddc0c","26a1");
	}
	return false;
    case E_NINJA:
	if (record->event.pressed) {
            symbol_key_mac("d83edd77","d83ddc88");
	}
	return false;
    case E_STOP:
	if (record->event.pressed) {
            symbol_key_mac("d83dded1","270a");
	}
	return false;
    case E_HAND:
	if (record->event.pressed) {
            symbol_key_mac("d83ddd90","270c");
	}
	return false;
    case E_SHH:
	if (record->event.pressed) {
            symbol_key_mac("d83edd2b","d83dde34");
	}
	return false;
    case E_PACKAGE:
	if (record->event.pressed) {
            symbol_key_mac("d83ddce6","d83ddcec");
	}
	return false;
    case E_POINTU:
	if (record->event.pressed) {
            symbol_key_mac("d83ddc46","2b06fe0f");
	}
	return false;
    case E_POINTD:
	if (record->event.pressed) {
            symbol_key_mac("d83ddc47","2b07fe0f");
	}
	return false;
    case E_POINTL:
	if (record->event.pressed) {
            symbol_key_mac("d83ddc48","2b05fe0f");
	}
	return false;
    case E_POINTR:
	if (record->event.pressed) {
            symbol_key_mac("d83ddc49","27a1fe0f");
	}
	return false;
    case E_DRINK:
	if (record->event.pressed) {
            symbol_key_mac("2615","d83cdf7a");
	}
	return false;
    case E_CMARK:
	if (record->event.pressed) {
            symbol_key_mac("2705","2714");
	}
	return false;
    case E_EMARK:
	if (record->event.pressed) {
            symbol_key_mac("2757","2755");
	}
	return false;
    case E_QMARK:
	if (record->event.pressed) {
            symbol_key_mac("2753","2754");
	}
	return false;
    case E_BULB:
	if (record->event.pressed) {
            symbol_key_mac("d83ddca1","d83edeab");
	}
	return false;
    case E_SSHOT:
	if (record->event.pressed) {
            symbol_key_mac("d83dddbc","d83ddcf8");
	}
	return false;
    case E_MIC:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf99","d83cdfa4");
	}
	return false;
    case E_TRASH:
	if (record->event.pressed) {
            symbol_key_mac("d83dddd1","267bfe0f");
	}
	return false;
    case E_I:
	if (record->event.pressed) {
            symbol_key_mac("2139fe0f","2139");
	}
	return false;
    case E_K0:
	if (record->event.pressed) {
            symbol_key_mac("0030fe0f20e3","D83DDD59");
	}
	return false;
    case E_K1:
	if (record->event.pressed) {
            symbol_key_mac("0031fe0f20e3","D83DDD50");
	}
	return false;
    case E_K2:
	if (record->event.pressed) {
            symbol_key_mac("0032FE0F20E3","D83DDD51");
	}
	return false;
    case E_K3:
	if (record->event.pressed) {
            symbol_key_mac("0033FE0F20E3","D83DDD52");
	}
	return false;
    case E_K4:
	if (record->event.pressed) {
            symbol_key_mac("0034FE0F20E3","D83DDD53");
	}
	return false;
    case E_K5:
	if (record->event.pressed) {
            symbol_key_mac("0035FE0F20E3","D83DDD54");
	}
	return false;
    case E_K6:
	if (record->event.pressed) {
            symbol_key_mac("0036FE0F20E3","D83DDD55");
	}
	return false;
    case E_K7:
	if (record->event.pressed) {
            symbol_key_mac("0037FE0F20E3","D83DDD56");
	}
	return false;
    case E_K8:
	if (record->event.pressed) {
            symbol_key_mac("0038FE0F20E3","D83DDD57");
	}
	return false;
    case E_K9:
	if (record->event.pressed) {
            symbol_key_mac("0039FE0F20E3","D83DDD58");
	}
	return false;
    case E_STAR:
	if (record->event.pressed) {
            symbol_key_mac("002AFE0F20E3","2731");
	}
	return false;
    case E_HASH:
	if (record->event.pressed) {
            symbol_key_mac("0023FE0F20E3","d83ddcf1");
	}
	return false;
    case E_MINS:
	if (record->event.pressed) {
            symbol_key_mac("2796","D83DDD5A");
	}
	return false;
    case E_PLUS:
	if (record->event.pressed) {
            symbol_key_mac("2795","D83DDD5B");
	}
	return false;
    case E_DIV:
	if (record->event.pressed) {
            symbol_key_mac("2797","2716FE0F");
	}
	return false;
    case E_GRIM:
	if (record->event.pressed) {
            symbol_key_mac("d83dde2c","d83dde16");
	}
	return false;
    case E_KB:
	if (record->event.pressed) {
            symbol_key_mac("2328FE0F","D83DDDA5FE0F");
	}
	return false;
    case E_OK:
	if (record->event.pressed) {
            symbol_key_mac("d83ddc4c","d83ddc4d");
	}
	return false;
    case E_SGLASS:
	if (record->event.pressed) {
            symbol_key_mac("d83dde0e","D83DDD76FE0F");
	}
	return false;
    case E_BOTTLE:
	if (record->event.pressed) {
            symbol_key_mac("d83cdf7c","d83cdf7e");
	}
	return false;
    case KB_RESET:
        if (record->event.pressed) {
           // reset the keyboard
           eeconfig_init();
           soft_reset_keyboard();
        }
        return false;
#if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
    #ifdef LK_WIRELESS_ENABLE
    // for keychron or lemokey bt mode change, stop fade for a little while so can see the connection status lights
    case BT_HST1:
    case BT_HST2:
    case BT_HST3:
    case P2P4G:
        if (record->event.pressed) {
            if (wireless_mode_token) {
                cancel_deferred_exec(wireless_mode_token);
                wireless_mode_token = INVALID_DEFERRED_TOKEN;
            }
        }
        else if (enable_keytracker) {
            enable_keytracker = false;
            wireless_mode_token = defer_exec(4500, wireless_mode_callback, NULL);
        }
        break;
    #endif
#elif defined(KEYBOARD_IS_BRIDGE)
    #ifdef WIRELESS_ENABLE
    // these keycodes should start a timer to allow showing of indicators setup in bridge75.c
    case KC_USB:
    case KC_BT1:
    case KC_BT2:
    case KC_BT3:
    case KC_2G4:
    case KC_BATQ:
         if (record->event.pressed) {
            wls_action_timer = timer_read32();
         }
         break;
    #endif
#endif
#ifndef CONFIG_HAS_LLOCK_KEY
    case KC_ENT:
        if (record->event.pressed) {
            uint8_t layer = get_highest_layer(layer_state);
            if (get_mods() & MOD_MASK_CTRL) {
                if (!is_base_layer(layer)) {
                    if (is_layer_locked(layer))
                        layer_lock_off(layer);
                    else
                        layer_lock_on(layer);
                    return false;
                }
            }
        }
        break;
#endif
    case FLASH_KB:
        if (record->event.pressed) {
           // command to flash this keyboard
           send_string("qmk flash -j 0 -kb " QMK_KEYBOARD " -km " QMK_KEYMAP "\n");
        }
	return false;
    case BOOTLDR:
        if (record->event.pressed) {
            key_timer = timer_read32();
        } else if (timer_elapsed32(key_timer) >= 500) {
            reset_keyboard();
        }
	return false;
    }
    return true;
}

bool process_leader_userspace(void) {
    bool continue_leader_process = false;
    if (leader_sequence_two_keys(KC_O, KC_S)) {                   // show current os
        // just show which os is currently set
        os_changed = true;
    }
    else if (leader_sequence_three_keys(KC_M, KC_A, KC_C)) {      // change to mac os
        if (!is_mac_base()) {
        #ifdef CONFIG_DEFAULT_MAC_LAYR
            set_single_persistent_default_layer(CONFIG_DEFAULT_MAC_LAYR);
            layer_move(CONFIG_DEFAULT_MAC_LAYR);
        #else
            set_single_persistent_default_layer(MAC_BASE);
            layer_move(MAC_BASE);
        #endif
        }
        os_changed = true;
    }
    else if (leader_sequence_three_keys(KC_W, KC_I, KC_N)) {      // change to windows os
        if (is_mac_base()) {
        #ifdef CONFIG_DEFAULT_WIN_LAYR
            set_single_persistent_default_layer(CONFIG_DEFAULT_WIN_LAYR);
            layer_move(CONFIG_DEFAULT_WIN_LAYR);
        #else
            set_single_persistent_default_layer(WIN_BASE);
            layer_move(WIN_BASE);
        #endif
        }
        if (user_config.is_linux_base) {
            user_config.is_linux_base = false;
            eeconfig_update_user(user_config.raw);
        }
        os_changed = true;
    }
    else if (leader_sequence_three_keys(KC_L, KC_I, KC_N)) {      // change to linux os
        if (is_mac_base()) {
        #ifdef CONFIG_DEFAULT_WIN_LAYR
            set_single_persistent_default_layer(CONFIG_DEFAULT_WIN_LAYR);
            layer_move(CONFIG_DEFAULT_WIN_LAYR);
        #else
            set_single_persistent_default_layer(WIN_BASE);
            layer_move(WIN_BASE);
        #endif
        }
        if (!user_config.is_linux_base) {
            user_config.is_linux_base = true;
            eeconfig_update_user(user_config.raw);
        }
        os_changed = true;
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_F)) {      // layer lock FN_LAYR
        if (is_layer_locked(FN_LAYR)) {
            layer_lock_off(FN_LAYR);
        }
        else {
            layer_lock_on(FN_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_T)) {      // layer lock TMUX_LAYR
        if (is_layer_locked(TMUX_LAYR)) {
            layer_lock_off(TMUX_LAYR);
        }
        else {
            layer_lock_on(TMUX_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_S)) {      // layer lock SFT_LAYR
        if (is_layer_locked(SFT_LAYR)) {
            layer_lock_off(SFT_LAYR);
        }
        else {
            layer_lock_on(SFT_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_K)) {      // layer lock KCTL_LAYR
        if (is_layer_locked(KCTL_LAYR)) {
            layer_lock_off(KCTL_LAYR);
        }
        else {
            layer_lock_on(KCTL_LAYR);
        }
    }
    else if (leader_sequence_four_keys(KC_L, KC_L, KC_S, KC_M)) { // layer lock M/W SYM_LAYR
        if (is_mac_base()) {
            if (is_layer_locked(MSYM_LAYR)) {
                layer_lock_off(MSYM_LAYR);
            }
            else {
                layer_lock_on(MSYM_LAYR);
            }
        }
        else {
            if (is_layer_locked(WSYM_LAYR)) {
                layer_lock_off(WSYM_LAYR);
            }
            else {
                layer_lock_on(WSYM_LAYR);
            }
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_W)) {      // layer lock WIDE_LAYR
        if (is_layer_locked(WIDE_LAYR)) {
            layer_lock_off(WIDE_LAYR);
        }
        else {
            layer_lock_on(WIDE_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_C)) {      // layer lock CIRC_LAYR
        if (is_layer_locked(CIRC_LAYR)) {
            layer_lock_off(CIRC_LAYR);
        }
        else {
            layer_lock_on(CIRC_LAYR);
        }
    }
    else if (leader_sequence_three_keys(KC_L, KC_L, KC_E)) {      // layer lock EMO_LAYR
        if (is_mac_base()) {
            if (is_layer_locked(EMO_LAYR)) {
                layer_lock_off(EMO_LAYR);
            }
            else {
                layer_lock_on(EMO_LAYR);
            }
        }
        else {
            continue_leader_process = true;
        }
    }
    else if (leader_sequence_four_keys(KC_L, KC_O, KC_C, KC_K)) { // switch to LOCK_LAYR
        user_config.rgb_mode = rgb_matrix_get_mode();
        eeconfig_update_user(user_config.raw);
        layer_on(LOCK_LAYR);
        #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
        lock_anim_timer = timer_read32();
        lock_anim_active = true;
        #endif
        #ifdef CONFIG_LOCK_ANIMATION
        rgb_matrix_mode_noeeprom(CONFIG_LOCK_ANIMATION);
        #else
        rgb_matrix_mode_noeeprom(RGB_MATRIX_BAND_VAL);
        #endif
    }
#if defined(CONFIG_MAC_BASE_CHANGE_GROUP) || defined(CONFIG_WIN_BASE_CHANGE_GROUP)
    else if (leader_sequence_four_keys(KC_B, KC_A, KC_S, KC_E)) { // change base within a base group
        if (is_mac_base()) {
            if (mac_base_change_group_count > 0) {
                uint8_t current_base = get_highest_layer(default_layer_state);
                for (uint8_t i = 0; i < mac_base_change_group_count; i++) {
                    if ((current_base == mac_base_change_group[i]) && (i + 1 < mac_base_change_group_count)) {
                        set_single_persistent_default_layer(mac_base_change_group[i + 1]);
                        layer_move(mac_base_change_group[i + 1]);
                        break;
                    }
                    else if (i + 1 == mac_base_change_group_count) {
                        set_single_persistent_default_layer(mac_base_change_group[0]);
                        layer_move(mac_base_change_group[0]);
                    }
                }
                base_layer_changed_in_group = true;
            }
        }
        else {
            if (win_base_change_group_count > 0) {
                uint8_t current_base = get_highest_layer(default_layer_state);
                for (uint8_t i = 0; i < win_base_change_group_count; i++) {
                    if ((current_base == win_base_change_group[i]) && (i + 1 < win_base_change_group_count)) {
                        set_single_persistent_default_layer(win_base_change_group[i + 1]);
                        layer_move(win_base_change_group[i + 1]);
                        break;
                    }
                    else if (i + 1 == win_base_change_group_count) {
                        set_single_persistent_default_layer(win_base_change_group[0]);
                        layer_move(win_base_change_group[0]);
                    }
                }
                base_layer_changed_in_group = true;
            }
        }
    }
#endif
    else if (leader_sequence_two_keys(KC_L, KC_K)) {              // key lock watch for key to lock
        set_key_lock_watching();
    }
    else if (leader_sequence_five_keys(KC_C, KC_O, KC_L, KC_O, KC_R)) { // start the color test
        color_test_timer = timer_read();
        color_test = true;
    }
    else if (leader_sequence_two_keys(KC_R, KC_T)) {          // rpi temp monitor
        SEND_STRING("rpi/temperature.sh" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_R, KC_U)) {          // rpi uptime monitor
        SEND_STRING("rpi/uptime.sh" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_P, KC_R)) {          // restart pi-hole
        SEND_STRING("sudo service pihole-FTL restart" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_M, KC_A)) {          // sudo mount -a
        SEND_STRING("sudo mount -a" SS_TAP(X_ENT));
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
        if (is_mac_base()) {
          SEND_STRING("wget https://get.glennr.nl/unifi/update/unifi-update.sh && sudo bash unifi-update.sh --custom-url " SS_LCMD(SS_TAP(X_V)) SS_TAP(X_ENT));
        }
        else {
          SEND_STRING("wget https://get.glennr.nl/unifi/update/unifi-update.sh && sudo bash unifi-update.sh --custom-url " SS_LCTL(SS_TAP(X_V)) SS_TAP(X_ENT));
        }
    }
    else if (leader_sequence_two_keys(KC_V, KC_H)) {          // open hosts file in vi
        SEND_STRING("sudo vi /etc/hosts" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_V, KC_A)) {          // open tb authorized in vi
        SEND_STRING("sudo vi /sys/bus/thunderbolt/devices/0-3/authorized" SS_TAP(X_ENT));
    }
    else if (leader_sequence_two_keys(KC_W, KC_C)) {          // automator script to get word count for selected text
        if (is_mac_base()) {
            SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_W))));
        }
        else {
            continue_leader_process = true;
        }
    }
    else if (leader_sequence_two_keys(KC_M, KC_R)) {          // numbers move cell contents right (and 0 current cell)
        if (is_mac_base()) {
            SEND_STRING(SS_LCMD(SS_TAP(X_C)) SS_TAP(X_0) SS_TAP(X_RIGHT) SS_LCMD(SS_TAP(X_V)));
        }
        else {
            continue_leader_process = true;
        }
    }
    else if (leader_sequence_two_keys(KC_M, KC_L)) {          // numbers move cell contents left (and 0 current cell)
        if (is_mac_base()) {
            SEND_STRING(SS_LCMD(SS_TAP(X_C)) SS_TAP(X_0) SS_TAP(X_LEFT) SS_LCMD(SS_TAP(X_V)));
        }
        else {
            continue_leader_process = true;
        }
    }
    else if (leader_sequence_two_keys(KC_S, KC_W)) {          // select word
        if (is_mac_base()) {
            SEND_STRING(SS_LOPT(SS_TAP(X_LEFT) SS_LSFT(SS_TAP(X_RIGHT))));
        }
        else {
            SEND_STRING(SS_LCTL(SS_TAP(X_LEFT) SS_LSFT(SS_TAP(X_RIGHT))));
        }
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
        SEND_STRING("qmk compile -j 0 -kb shortcut/bridge75 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_F, KC_B)) {  // qmk flash shortcutstudio bridge75 firmware
        SEND_STRING("qmk flash -j 0 -kb shortcut/bridge75 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_C, KC_Y)) {  // qmk compile yunzii firmware
        SEND_STRING("qmk compile -j 0 -kb yunzii/al68 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_F, KC_Y)) {  // qmk flash yunzii firmware
        SEND_STRING("qmk flash -j 0 -kb yunzii/al68 -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_C, KC_L)) {  // qmk compile lemokey p1 firmware
        SEND_STRING("qmk compile -j 0 -kb lemokey/p1_pro/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_three_keys(KC_Q, KC_F, KC_L)) {  // qmk flash lemokey p1 firmware
        SEND_STRING("qmk flash -j 0 -kb lemokey/p1_pro/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_C, KC_K, KC_V)) {  // qmk compile keychron V6 firmware
        SEND_STRING("qmk compile -j 0 -kb keychron/v6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_F, KC_K, KC_V)) {  // qmk flash keychron V6 firmware
        SEND_STRING("qmk flash -j 0 -kb keychron/v6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_C, KC_K, KC_Q)) {  // qmk compile keychron Q6 firmware
        SEND_STRING("qmk compile -j 0 -kb keychron/q6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
    }
    else if (leader_sequence_four_keys(KC_Q, KC_F, KC_K, KC_Q)) {  // qmk flash keychron Q6 firmware
        SEND_STRING("qmk flash -j 0 -kb keychron/q6_max/ansi_encoder -km djcastaldo" SS_TAP(X_ENT));
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
    else if (leader_sequence_four_keys(KC_B, KC_O, KC_O, KC_T)) {  // reset to bootloader
        reset_keyboard();
    }
    else {
        continue_leader_process = true;
    }
    return continue_leader_process;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
#ifdef CONFIG_RGB_LAYER_INDICATORS
    // color an indicator
    for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
        switch (layer) {
            case FN_LAYR:
            #ifdef CONFIG_FN_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_FN_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_GREEN);
            #endif
                break;
            case SFT_LAYR:
            #ifdef CONFIG_SHIFT_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_SHIFT_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_ORANGE);
            #endif
                break;
            case KCTL_LAYR:
            #ifdef CONFIG_KCTL_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_KCTL_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_RED);
            #endif
                break;
            case TMUX_LAYR:
            #ifdef CONFIG_TMUX_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_TMUX_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_CYAN);
            #endif
                break;
            case WSYM_LAYR:
            case MSYM_LAYR:
            #ifdef CONFIG_SYM_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_SYM_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_BLUE);
            #endif
                break;
            case WIDE_LAYR:
            #ifdef CONFIG_WIDE_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_WIDE_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_PURPLE);
            #endif
                break;
            case CIRC_LAYR:
            #ifdef CONFIG_CIRC_LAYR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_CIRC_LAYR_COLOR);
            #else
                rgb_matrix_set_color(rgb_layer_indicators[i], RGB_CORAL);
            #endif
                break;
            case LOCK_LAYR:
                break;
            default:
            #ifdef CONFIG_DEFAULT_INDICATOR_COLOR
                rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_DEFAULT_INDICATOR_COLOR);
            #endif
                break;
        }
    }
#endif

    if (rgb_indicators_enabled
#ifdef CONFIG_HAS_KCLK_BATTERY
        && !bat_level_animiation_actived() && !battery_is_empty()
#endif
    ) {
        if (!is_base_layer(layer)) {
            for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
                for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                    uint8_t index = g_led_config.matrix_co[row][col];
        
                    if (index >= led_min && index < led_max && index != NO_LED &&
                    keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                        switch (layer) {
                        case FN_LAYR:
                            if (index == I_MREC1 || index == I_MREC2) { // macro recording keys
                            #ifdef CONFIG_MREC_KEY_COLOR
                                rgb_matrix_set_color(index, CONFIG_MREC_KEY_COLOR);
                            #else
                                rgb_matrix_set_color(index, RGB_MAGENTA);
                            #endif
                            }
                            else {
                            #ifdef CONFIG_FN_LAYR_COLOR
                                rgb_matrix_set_color(index, CONFIG_FN_LAYR_COLOR);
                            #else
                                rgb_matrix_set_color(index, RGB_GREEN);
                            #endif
                            }
                            break;
                        case SFT_LAYR:
                        #ifdef CONFIG_SHIFT_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_SHIFT_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_ORANGE);
                        #endif
                            break;
                        case KCTL_LAYR:
                        #ifdef CONFIG_KCTL_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_KCTL_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_RED);
                        #endif
                            break;
                        case TMUX_LAYR:
                        #ifdef CONFIG_TMUX_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_TMUX_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_CYAN);
                        #endif
                            break;
                        case WSYM_LAYR:
                        case MSYM_LAYR:
                        #ifdef CONFIG_SYM_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_SYM_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_BLUE);
                        #endif
                            break;
                        case WIDE_LAYR:
                        #ifdef CONFIG_WIDE_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_WIDE_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_PURPLE);
                        #endif
                            break;
                        case CIRC_LAYR:
                        #ifdef CONFIG_CIRC_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_CIRC_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_CORAL);
                        #endif
                            break;
                        case EMO_LAYR:
                        #ifdef CONFIG_EMO_LAYR_COLOR
                            rgb_matrix_set_color(index, CONFIG_EMO_LAYR_COLOR);
                        #else
                            rgb_matrix_set_color(index, RGB_YELLOW);
                        #endif
                            break;
                        case LOCK_LAYR:
                            break;
                        default:
                        #ifdef CONFIG_DEFUALT_LAYR_COLOR
                            rgb_matrix_set_color(index, 0x77, 0x77, 0x77);
                        #else
                            rgb_matrix_set_color(index, 0x77, 0x77, 0x77);
                        #endif
                            break;
                        }
                    }
                }
            }
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
            // custom colors for layer keys on fn layer
            if (layer == FN_LAYR) {
            #ifdef CONFIG_SHIFT_LAYR_COLOR
                rgb_matrix_set_color(I_LSFT, CONFIG_SHIFT_LAYR_COLOR); // left shift
                rgb_matrix_set_color(I_RSFT, CONFIG_SHIFT_LAYR_COLOR); // right shift
            #else
                rgb_matrix_set_color(I_LSFT, RGB_ORANGE); // left shift
                rgb_matrix_set_color(I_RSFT, RGB_ORANGE); // right shift
            #endif
            #ifdef CONFIG_KCTL_LAYR_COLOR
                rgb_matrix_set_color(I_LCMD, CONFIG_KCTL_LAYR_COLOR);  // left alt / left cmd
                #ifndef CONFIG_NO_RCMD_KEY
                rgb_matrix_set_color(I_RCMD, CONFIG_KCTL_LAYR_COLOR);  // right alt / right cmd 
                #elif !defined(CONFIG_NO_RCTL_KEY)
                rgb_matrix_set_color(I_RCTL, CONFIG_KCTL_LAYR_COLOR);  // right control
                #endif
            #else
                rgb_matrix_set_color(I_LCMD, RGB_RED);    // left alt / left cmd
                #ifndef CONFIG_NO_RCMD_KEY
                rgb_matrix_set_color(I_RCMD, RGB_RED);    // right alt / right cmd 
                #elif !defined(CONFIG_NO_RCTL_KEY)
                rgb_matrix_set_color(I_RCTL, RGB_RED);  // right control
                #endif
            #endif
            #ifdef CONFIG_TMUX_LAYR_COLOR
                rgb_matrix_set_color(I_TAB, CONFIG_TMUX_LAYR_COLOR);   // tab
            #else
                rgb_matrix_set_color(I_TAB, RGB_CYAN);    // tab
            #endif
            #ifdef CONFIG_SYM_LAYR_COLOR
                rgb_matrix_set_color(I_LGUI, CONFIG_SYM_LAYR_COLOR);   // left win / left opt 
                #ifdef CONFIG_HAS_ROPT_KEY
                rgb_matrix_set_color(I_ROPT, CONFIG_SYM_LAYR_COLOR);   // right win / right opt 
                #else
                rgb_matrix_set_color(I_FN, CONFIG_SYM_LAYR_COLOR);     // if no ropt, light fn
                #endif
            #else
                rgb_matrix_set_color(I_LGUI, RGB_BLUE);   // left win / left opt 
                #ifdef CONFIG_HAS_ROPT_KEY
                rgb_matrix_set_color(I_ROPT, RGB_BLUE);   // right win / right opt 
                #else
                rgb_matrix_set_color(I_FN, RGB_BLUE);     // if no ropt, light fn
                #endif
            #endif
            }
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ 
            // custom colors for tap dance keys on symbol layer
            else if (layer == WSYM_LAYR || layer == MSYM_LAYR) {
            #ifdef CONFIG_ACCENT_KEY_COLOR
                rgb_matrix_set_color(I_GRV, CONFIG_ACCENT_KEY_COLOR); // grave
                rgb_matrix_set_color(I_N1, CONFIG_ACCENT_KEY_COLOR);  // 1
                rgb_matrix_set_color(I_E, CONFIG_ACCENT_KEY_COLOR);   // e
                rgb_matrix_set_color(I_U, CONFIG_ACCENT_KEY_COLOR);   // u
                rgb_matrix_set_color(I_I, CONFIG_ACCENT_KEY_COLOR);   // i
                rgb_matrix_set_color(I_N, CONFIG_ACCENT_KEY_COLOR);   // n
            #else
                rgb_matrix_set_color(I_GRV, RGB_YELLOW); // grave
                rgb_matrix_set_color(I_N1, RGB_YELLOW);  // 1
                rgb_matrix_set_color(I_E, RGB_YELLOW);   // e
                rgb_matrix_set_color(I_U, RGB_YELLOW);   // u
                rgb_matrix_set_color(I_I, RGB_YELLOW);   // i
                rgb_matrix_set_color(I_N, RGB_YELLOW);   // n
            #endif
            }
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~
            // set up the layer key blinking indicator
            if (!layer_timer) {
                is_led_on = true;
                layer_timer = timer_read();
            }
            // toggle an led light every 500 ms while the layer is active
            else if (timer_elapsed(layer_timer) > 500)
            {
                is_led_on = !is_led_on;
                layer_timer = timer_read();
            }
            // the led and color to flash on each layer to show the layer is active
            if (is_led_on)
            {
                switch (layer) {
                case FN_LAYR:
                #ifdef CONFIG_FN_LAYR_COLOR
                    rgb_matrix_set_color(I_CAPS, CONFIG_FN_LAYR_COLOR);  // caps
                    rgb_matrix_set_color(I_FN, CONFIG_FN_LAYR_COLOR);    // fn
                #else
                    rgb_matrix_set_color(I_CAPS, RGB_GREEN);  // caps
                    rgb_matrix_set_color(I_FN, RGB_GREEN);    // fn
                #endif
                    break;
                case SFT_LAYR:
                #ifdef CONFIG_SHIFT_LAYR_COLOR
                    rgb_matrix_set_color(I_LSFT, CONFIG_SHIFT_LAYR_COLOR); // lshift
                    rgb_matrix_set_color(I_RSFT, CONFIG_SHIFT_LAYR_COLOR); // rshift
                #else
                    rgb_matrix_set_color(I_LSFT, RGB_ORANGE); // lshift
                    rgb_matrix_set_color(I_RSFT, RGB_ORANGE); // rshift
                #endif
                    break;
                case KCTL_LAYR:
                #ifdef CONFIG_KCTL_LAYR_COLOR
                    rgb_matrix_set_color(I_LALT, CONFIG_KCTL_LAYR_COLOR);  // lalt
                    #ifndef CONFIG_NO_RALT_KEY
                    rgb_matrix_set_color(I_RALT, CONFIG_KCTL_LAYR_COLOR);  // ralt
                    #elif !defined(CONFIG_NO_RCTL_KEY)
                    rgb_matrix_set_color(I_RCTL, CONFIG_KCTL_LAYR_COLOR);  // right control
                    #endif
                #else
                    rgb_matrix_set_color(I_LALT, RGB_RED);    // lalt
                    #ifndef CONFIG_NO_RALT_KEY
                    rgb_matrix_set_color(I_RALT, RGB_RED);    // ralt
                    #elif !defined(CONFIG_NO_RCTL_KEY)
                    rgb_matrix_set_color(I_RCTL, RGB_RED);    // right control
                    #endif
                #endif
                    break;
                case TMUX_LAYR:
                #ifdef CONFIG_TMUX_LAYR_COLOR
                    rgb_matrix_set_color(I_TAB, CONFIG_TMUX_LAYR_COLOR);   // tab
                #else
                    rgb_matrix_set_color(I_TAB, RGB_CYAN);    // tab
                #endif
                    break;
                case WSYM_LAYR:
                    if (timer_elapsed(layer_timer) > 250) {
                    #ifdef CONFIG_ACCENT_KEY_COLOR
                        #ifndef CONFIG_NO_RALT_KEY
                        rgb_matrix_set_color(I_RALT, CONFIG_ACCENT_KEY_COLOR); // ralt
                        #endif
                        rgb_matrix_set_color(I_LGUI, CONFIG_ACCENT_KEY_COLOR); // lgui
                        rgb_matrix_set_color(I_FN, CONFIG_ACCENT_KEY_COLOR);   // fn
                    #else
                        #ifndef CONFIG_NO_RALT_KEY
                        rgb_matrix_set_color(I_RALT, RGB_YELLOW); // ralt
                        #endif
                        rgb_matrix_set_color(I_LGUI, RGB_YELLOW); // lgui
                        rgb_matrix_set_color(I_FN, RGB_YELLOW);   // fn
                    #endif
                    }
                    else {
                    #ifdef CONFIG_SYM_LAYR_COLOR
                        #ifndef CONFIG_NO_RALT_KEY
                        rgb_matrix_set_color(I_RALT, CONFIG_SYM_LAYR_COLOR);   // ralt
                        #endif
                        rgb_matrix_set_color(I_LGUI, CONFIG_SYM_LAYR_COLOR);   // lgui
                        rgb_matrix_set_color(I_FN, CONFIG_SYM_LAYR_COLOR);     // fn
                    #else
                        #ifndef CONFIG_NO_RALT_KEY
                        rgb_matrix_set_color(I_RALT, RGB_BLUE);   // ralt
                        #endif
                        rgb_matrix_set_color(I_LGUI, RGB_BLUE);   // lgui
                        rgb_matrix_set_color(I_FN, RGB_BLUE);     // fn
                    #endif
                    }
                    break;
                case MSYM_LAYR:
                    if (timer_elapsed(layer_timer) > 250) {
                    #ifdef CONFIG_ACCENT_KEY_COLOR
                        rgb_matrix_set_color(I_LOPT, CONFIG_ACCENT_KEY_COLOR); // lopt
                        #ifdef CONFIG_HAS_ROPT_KEY
                        rgb_matrix_set_color(I_ROPT, CONFIG_ACCENT_KEY_COLOR); // ropt
                        #endif
                        rgb_matrix_set_color(I_FN, CONFIG_ACCENT_KEY_COLOR);   // fn
                    #else
                        rgb_matrix_set_color(I_LOPT, RGB_YELLOW); // lopt
                        #ifdef CONFIG_HAS_ROPT_KEY
                        rgb_matrix_set_color(I_ROPT, RGB_YELLOW); // ropt
                        #endif
                        rgb_matrix_set_color(I_FN, RGB_YELLOW);   // fn
                    #endif
                    }
                    else {
                    #ifdef CONFIG_SYM_LAYR_COLOR
                        rgb_matrix_set_color(I_LOPT, CONFIG_SYM_LAYR_COLOR);   // lopt
                        #ifdef CONFIG_HAS_ROPT_KEY
                        rgb_matrix_set_color(I_ROPT, CONFIG_SYM_LAYR_COLOR);   // ropt
                        #endif
                        rgb_matrix_set_color(I_FN, CONFIG_SYM_LAYR_COLOR);     // fn
                    #else
                        rgb_matrix_set_color(I_LOPT, RGB_BLUE);   // lopt
                        #ifdef CONFIG_HAS_ROPT_KEY
                        rgb_matrix_set_color(I_ROPT, RGB_BLUE);   // ropt
                        #endif
                        rgb_matrix_set_color(I_FN, RGB_BLUE);     // fn
                    #endif
                    }
                    break;
                case WIDE_LAYR:
                #ifdef CONFIG_WIDE_LAYR_COLOR
                    rgb_matrix_set_color(I_RSFT, CONFIG_WIDE_LAYR_COLOR);     // rshift
                #else
                    rgb_matrix_set_color(I_RSFT, RGB_PURPLE);     // rshift
                #endif
                    break;
                case CIRC_LAYR:
                #ifdef CONFIG_CIRC_LAYR_COLOR
                    rgb_matrix_set_color(I_RSFT, CONFIG_CIRC_LAYR_COLOR);      // rshift
                #else
                    rgb_matrix_set_color(I_RSFT, RGB_CORAL);      // rshift
                #endif
                    break;
                case EMO_LAYR:
                #ifdef CONFIG_EMO_LAYR_COLOR
                    #ifndef CONFIG_NO_RCMD_KEY
                    rgb_matrix_set_color(I_RCMD, CONFIG_EMO_LAYR_COLOR);      // rcmd
                    #endif
                    #ifdef CONFIG_HAS_ROPT_KEY
                    rgb_matrix_set_color(I_ROPT, CONFIG_EMO_LAYR_COLOR);      // ropt
                    #else
                    #ifndef CONFIG_NO_RCTL_KEY
                    rgb_matrix_set_color(I_RCTL, CONFIG_EMO_LAYR_COLOR);      // rctl
                    #endif
                    #endif
                #else
                    #ifndef CONFIG_NO_RCMD_KEY
                    rgb_matrix_set_color(I_RCMD, RGB_YELLOW);     // rcmd
                    #endif
                    #ifdef CONFIG_HAS_ROPT_KEY
                    rgb_matrix_set_color(I_ROPT, RGB_YELLOW);     // ropt
                    #else
                    #ifndef CONFIG_NO_RCTL_KEY
                    rgb_matrix_set_color(I_RCTL, RGB_YELLOW);     // rctl
                    #endif
                    #endif
                #endif
                    break;
                case LOCK_LAYR:
                    for (uint8_t i = 0; i < lock_layr_extra_flash_keys_count; i++) {
                    #ifdef CONFIG_LOCK_LAYR_EXTRA_FLASH_COLOR
                        rgb_matrix_set_color(lock_layr_extra_flash_keys[i], CONFIG_LOCK_LAYR_EXTRA_FLASH_COLOR);
                    #else
                        rgb_matrix_set_color(lock_layr_extra_flash_keys[i], RGB_RED);
                    #endif
                    }
                    break;
                default:
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
                #ifdef CONFIG_HAS_LLOCK_KEY
                    rgb_matrix_set_color(I_LLOCK, RGB_WHITE); // just make it white
                #else
                    for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                        rgb_matrix_set_color(rgb_layer_indicators[i], RGB_WHITE);
                    }
                #endif
                }
                else if ((timer_elapsed(layer_lock_timer) > 200 && timer_elapsed(layer_lock_timer) < 400) || 
                         (timer_elapsed(layer_lock_timer) > 600)) {
                #ifdef CONFIG_HAS_LLOCK_KEY
                    rgb_matrix_set_color(I_LLOCK, RGB_WHITE); // white alternate with layer color
                #else
                    for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                        rgb_matrix_set_color(rgb_layer_indicators[i], RGB_WHITE);
                    }
                #endif
                }
            }
        }
        else {
            for (uint8_t i = 0; i < extra_base_layr_colors_count; i++) {
                rgb_matrix_set_color(
                    extra_base_layr_colors[i].index,
                    extra_base_layr_colors[i].r,
                    extra_base_layr_colors[i].g,
                    extra_base_layr_colors[i].b
                );
            }
            // on windows or linux, highlight some possible winkey/super shortcuts while winkey/super is held
            if (is_winkey_held) {
                if (user_config.is_linux_base) {
                    for (int i = 0; i < super_scut_keys_size; i++) {
                        rgb_matrix_set_color(super_scut_keys[i], 102, 178, 255);   // gnome super shortcut keys
                    }
                    for (int i = 0; i < super_scut_altcolor_size; i++) {
                        rgb_matrix_set_color(super_scut_altcolor[i], RGB_BLUE);    // gnome super shortcut keys
                    }
                }
                else {
                    for (int i = 0; i < winkey_scut_keys_size; i++) {
                        rgb_matrix_set_color(winkey_scut_keys[i], 102, 178, 255);  // winkey shortcut keys
                    }
                    for (int i = 0; i < winkey_scut_altcolor_size; i++) {
                        rgb_matrix_set_color(winkey_scut_altcolor[i], RGB_BLUE);   // winkey shortcut keys
                    }
                }
            }    
            // color caps word keys
            if (is_caps_word_on()) {
                // set capitilized keys red when caps word is active
                for (uint8_t i = led_min; i < led_max; i++) {
                    if (is_capsword_shifted(i)) {
                        rgb_matrix_set_color(i, RGB_RED);
                    }
                }
            #ifdef CONFIG_CAPS_WORD_SHIFT_COLOR
                rgb_matrix_set_color(I_LSFT, CONFIG_CAPS_WORD_SHIFT_COLOR); // color left shift when caps_word is active
                rgb_matrix_set_color(I_RSFT, CONFIG_CAPS_WORD_SHIFT_COLOR); // color right shift when caps_word is active
            #else
                rgb_matrix_set_color(I_LSFT, 0x77, 0x77, 0x77); // color left shift when caps_word is active
                rgb_matrix_set_color(I_RSFT, 0x77, 0x77, 0x77); // color right shift when caps_word is active
            #endif
            }
            // if not caps word, nor caps lock, then do the hrow colors if hrow_light setting is turned on       
            else if (!host_keyboard_led_state().caps_lock) {
                if (hrow_light) {
                #ifdef CONFIG_HROWLIGHT_COLOR
                    rgb_matrix_set_color(I_A, CONFIG_HROWLIGHT_COLOR);    // a
                    rgb_matrix_set_color(I_S, CONFIG_HROWLIGHT_COLOR);    // s
                    rgb_matrix_set_color(I_D, CONFIG_HROWLIGHT_COLOR);    // d
                    rgb_matrix_set_color(I_F, CONFIG_HROWLIGHT_COLOR);    // f
                    rgb_matrix_set_color(I_J, CONFIG_HROWLIGHT_COLOR);    // j
                    rgb_matrix_set_color(I_K, CONFIG_HROWLIGHT_COLOR);    // k
                    rgb_matrix_set_color(I_L, CONFIG_HROWLIGHT_COLOR);    // l
                    rgb_matrix_set_color(I_SEMI, CONFIG_HROWLIGHT_COLOR); // ;
                #else
                    rgb_matrix_set_color(I_A, RGB_GREEN);    // a
                    rgb_matrix_set_color(I_S, RGB_GREEN);    // s
                    rgb_matrix_set_color(I_D, RGB_GREEN);    // d
                    rgb_matrix_set_color(I_F, RGB_GREEN);    // f
                    rgb_matrix_set_color(I_J, RGB_GREEN);    // j
                    rgb_matrix_set_color(I_K, RGB_GREEN);    // k
                    rgb_matrix_set_color(I_L, RGB_GREEN);    // l
                    rgb_matrix_set_color(I_SEMI, RGB_GREEN); // ;
                #endif
                }
                if (fj_light) {
                #ifdef CONFIG_FJLIGHT_COLOR
                    rgb_matrix_set_color(I_F, CONFIG_FJLIGHT_COLOR); // f
                    rgb_matrix_set_color(I_J, CONFIG_FJLIGHT_COLOR); // j
                #else
                    rgb_matrix_set_color(I_F, RGB_WHITE); // f
                    rgb_matrix_set_color(I_J, RGB_WHITE); // j
                #endif
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
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_YELLOW);
                }
            }
            else {
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_ORANGE);
                }
            }
        }
        // if key lock is activated, flash the indicator white and red
        else if (is_key_locked) {
            if (!key_lock_timer || timer_elapsed(key_lock_timer) > 500) {
                is_key_lock_led_on = !is_key_lock_led_on;;
                key_lock_timer = timer_read();
            }
            if (is_key_lock_led_on) {
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_WHITE);
                }
            }
            else {
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_RED);
                }
            }
            // and light up normal modifiers if they are currently locked
            if (lctl_locked) {
                rgb_matrix_set_color(I_LCTL, 0x77, 0x77, 0x77);       // lctl
            }
            if (lalt_locked) {
                if (is_mac_base()) {
                    rgb_matrix_set_color(I_LOPT, 0x77, 0x77, 0x77);   // lopt
                }
                else {
                    rgb_matrix_set_color(I_LALT, 0x77, 0x77, 0x77);   // lalt
                }
            }
            if (lsft_locked) {
                rgb_matrix_set_color(I_LSFT, 0x77, 0x77, 0x77);       // lsft
            }
            if (lgui_locked) {
                if (is_mac_base()) {
                    rgb_matrix_set_color(I_LCMD, 0x77, 0x77, 0x77);   // lcmd
                }
                else {
                    rgb_matrix_set_color(I_LGUI, 0x77, 0x77, 0x77);   // lgui
                }
            }
            if (rsft_locked) {
                rgb_matrix_set_color(I_RSFT, 0x77, 0x77, 0x77);       // rsft
            }
            #ifndef CONFIG_NO_RALT_KEY
            if (ralt_locked) {
                rgb_matrix_set_color(I_RALT, 0x77, 0x77, 0x77);       // ralt
            }
            #endif
            #ifndef CONFIG_NO_RCTL_KEY
            if (rctl_locked) {
                rgb_matrix_set_color(I_RCTL, 0x77, 0x77, 0x77);       // rctl
            }
            #endif
        }

    #if defined(CONFIG_MAC_BASE_CHANGE_GROUP) || defined(CONFIG_WIN_BASE_CHANGE_GROUP)
        // check if a base layer change happened, and flash some indicators to show a change occured
        if (base_layer_changed_in_group) {
            if (!base_change_timer || timer_elapsed(base_change_timer) > 1300) {
                base_change_timer = timer_read();
            }
            rgb_matrix_set_color(I_BASECHG1, RGB_WHITE);             // home/insert
            if (timer_elapsed(base_change_timer) > 300) {
                rgb_matrix_set_color(I_BASECHG2, RGB_WHITE);         // pgup/del
            }
            if (timer_elapsed(base_change_timer) > 600) {
                rgb_matrix_set_color(I_BASECHG3, RGB_WHITE);        // pgdn/pgup
            }
            if (timer_elapsed(base_change_timer) > 900) {
                rgb_matrix_set_color(I_BASECHG4, RGB_WHITE);         // end/pgdn
            }
            if (timer_elapsed(base_change_timer) > 1200) {
                base_layer_changed_in_group = false;
            }
        }
    #endif
        // check if os change happened, and flash some indicators to show the change
        if (os_changed) {
            // turn off all currently lit leds first
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
            }
            int os_key1 = is_mac_base() ? I_M : user_config.is_linux_base ? I_L : I_W;
            int os_key2 = is_mac_base() ? I_A : I_I;
            int os_key3 = is_mac_base() ? I_C : I_N;
            if (!os_change_timer || timer_elapsed(os_change_timer) > 1900) {
                os_change_timer = timer_read();
            }
            rgb_matrix_set_color(os_key1, RGB_WHITE);           // M | L | W
            if (timer_elapsed(os_change_timer) > 300) {
                rgb_matrix_set_color(os_key2, RGB_WHITE);       // A | I | I
            }
            if (timer_elapsed(os_change_timer) > 600) {
                rgb_matrix_set_color(os_key3, RGB_WHITE);       // C | N | N
            }
            if (timer_elapsed(os_change_timer) > 1800) {
                os_changed = false;
            }
        }

        if (is_in_leader_sequence) {
            if (!leader_timer || timer_elapsed(leader_timer) > 500) {
                is_leader_led_on = !is_leader_led_on;
                leader_timer = timer_read();
            }
            if (is_leader_led_on) {
            #ifdef CONFIG_LEADER_COLORA
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_LEADER_COLORA);
                }
                rgb_matrix_set_color(I_LEAD, CONFIG_LEADER_COLORA);         // l
            #else
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_TURQUOISE);
                }
                rgb_matrix_set_color(I_LEAD, RGB_TURQUOISE);         // l
            #endif
            }
            else {
            #ifdef CONFIG_LEADER_COLORB
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], CONFIG_LEADER_COLORB);
                }
                rgb_matrix_set_color(I_LEAD, CONFIG_LEADER_COLORB);
            #else
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_CYAN);
                }
                rgb_matrix_set_color(I_LEAD, RGB_CYAN);
            #endif
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

        // if any rgb key highlights are on, turn the setting keys white on layer KCTL_LAYR
        if (layer == KCTL_LAYR)
        {
            if (fj_light) {
                rgb_matrix_set_color(I_FJLIGHT, RGB_WHITE);     // home (fj highlight key)
            #ifdef CONFIG_HAS_SECOND_FJLIGHT_KEY
                rgb_matrix_set_color(I_FJLIGHT2, RGB_WHITE);     // home (fj highlight key)
            #endif
            }
            if (hrow_light) {
                rgb_matrix_set_color(I_HROWLIGHT, RGB_WHITE);   // end (hrow highlight key)
            #ifdef CONFIG_HAS_SECOND_HROWLIGHT_KEY
                rgb_matrix_set_color(I_HROWLIGHT2, RGB_WHITE);   // end (hrow highlight key)
            #endif
            }
            if (enable_keytracker) {
                rgb_matrix_set_color(I_KTRACK, RGB_WHITE);        // semi (keytracker set key)
            #ifdef CONFIG_HAS_SECOND_KTRACK_KEY
                rgb_matrix_set_color(I_KTRACK2, RGB_WHITE);        // semi (keytracker set key)
            #endif
            }
        }

        // calculate the reactive rgb for keypresses
        if (enable_keytracker) {
            for (int i = 0; i < tk_length; i++) {
                if (tracked_keys[i].press) {
                    static bool is_alt_color_key;
                    is_alt_color_key = false;
                    for (uint8_t j = 0; j < keylight_alt_color_keys_count; j++) {
                        if (tracked_keys[i].index == keylight_alt_color_keys[j]) {
                            is_alt_color_key = true;
                        #ifdef CONFIG_KEYLIGHT_ALT_COLOR_KEY_COLOR
                            rgb_matrix_set_color(keylight_alt_color_keys[j], CONFIG_KEYLIGHT_ALT_COLOR_KEY_COLOR);
                        #else
                            rgb_matrix_set_color(keylight_alt_color_keys[j], RGB_RED);
                        #endif
                            break;
                        }
                    }
                    if (!is_alt_color_key) {
                    #ifdef CONFIG_KEYLIGHT_STD_COLOR
                        rgb_matrix_set_color(tracked_keys[i].index, CONFIG_KEYLIGHT_STD_COLOR);
                    #else
                        rgb_matrix_set_color(tracked_keys[i].index, RGB_WHITE);
                    #endif
                    }
                }
            #ifdef CONFIG_KEYS_FADE_TO_BLUE
                // do the key fade if key should fade
                else if (key_should_fade(tracked_keys[i], layer)) {
                    if (tracked_keys[i].fade > 255) {
                        rgb_matrix_set_color(tracked_keys[i].index, RGB_WHITE);
                    }
                    else if (tracked_keys[i].fade > 200) {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, 255);
                    } else if (tracked_keys[i].fade > 175) {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, tracked_keys[i].fade + 55);
                    } else if (tracked_keys[i].fade >  115) {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, tracked_keys[i].fade + 80);
                    } else if (tracked_keys[i].fade > 80) {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, tracked_keys[i].fade + 100);
                    } else if (tracked_keys[i].fade > 35) {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, tracked_keys[i].fade + 150);
                    } else {
                        rgb_matrix_set_color(tracked_keys[i].index, 35, 24, 189);
                    }
                }
            #else
                // do the key fade if key should fade
                // this is a modified fade to get a smoother look and be mostly white but a bit blue
                else if (key_should_fade(tracked_keys[i], layer)) {
                    if (tracked_keys[i].fade > 255) {
                        rgb_matrix_set_color(tracked_keys[i].index, RGB_WHITE);
                    } 
                    else if (tracked_keys[i].fade > 230) {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, 255);
                    } else {
                        rgb_matrix_set_color(tracked_keys[i].index, tracked_keys[i].fade, tracked_keys[i].fade, tracked_keys[i].fade + 25);
                    }  
                }
            #endif
            }
        }
        if (macro_recording) {
            // flash the indicator if macro is recording
            if (timer_elapsed(macro_timer) > 250) {
                is_macro_led_on = !is_macro_led_on;;
                macro_timer = timer_read();
            }
            if (is_macro_led_on) {
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_RED); // indicators
                }
                if (macro_direction == 1) {
                    rgb_matrix_set_color(I_MREC1, RGB_RED);   // macro1 record key
                }
                else {
                    rgb_matrix_set_color(I_MREC2, RGB_RED);   // macro2 record key
                }
            }
            else {
                for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                    rgb_matrix_set_color(rgb_layer_indicators[i], RGB_BLACK); // indicators
                }
                if (macro_direction == 1) {
                    rgb_matrix_set_color(I_MREC1, RGB_BLACK);   // macro1 record key
                }
                else {
                    rgb_matrix_set_color(I_MREC2, RGB_BLACK);   // macro2 record key
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
        // track if mouse button is held on SFT_LAYR
        if (layer == SFT_LAYR && ms_btn_held) {
            rgb_matrix_set_color(I_MHLD, RGB_WHITE);         // mouse btn1 hold key
        }
        // track mode keys on WIDE_LAYR
        if (layer == WIDE_LAYR) {
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
            rgb_matrix_set_color(I_CAPS, RGB_WHITE);  // caps
            // if caps is on, turn alhpa keys red
            for (uint8_t i = led_min; i < led_max; i++) {
                if (is_capslock_shifted(i)) {
                    rgb_matrix_set_color(i, RGB_RED);
                }
            }
        }
        // track num_lock
        if (layer == SFT_LAYR && host_keyboard_led_state().num_lock) {
            rgb_matrix_set_color(I_NUMLOCK, RGB_WHITE);
        }
        // track scroll_lock
        if (layer == FN_LAYR && host_keyboard_led_state().scroll_lock) {
            rgb_matrix_set_color(I_SLOCK, RGB_WHITE);
        }
        #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
        // check if lock animation is on and have it timeout
        if (lock_anim_active && timer_elapsed32(lock_anim_timer) > CONFIG_LOCK_ANIMATION_TIMEOUT) {
            #ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
            rgb_last_activity_timer = timer_read32() - CONFIG_CUSTOM_SLEEP_TIMEOUT + CONFIG_CUSTOM_SLEEP_WARNING;
            #else
                #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
                rgb_matrix_sethsv_noeeprom(0, 0, 50);
                #else
                rgb_matrix_disable_noeeprom();
                #endif
            rgb_indicators_enabled = false;
            #endif
            lock_anim_active = false;
        }
        #endif
            
    #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        #ifdef LK_WIRELESS_ENABLE
        // show wireless connection if just switched modes or if on KCTL_LAYR in bt or 2.4g modes
        if (wireless_mode_token || layer == KCTL_LAYR)
        {
            if (wireless_get_state() == WT_CONNECTED) {
                // host_index is set to 24 for 2.4g, bt is 1,2,3
            #if defined(CONFIG_KCLK_24G_HOST_IDX) && defined(CONFIG_KCLK_BT_KEY_START_IDX)
                rgb_matrix_set_color(wireless_get_host_index() == CONFIG_KCLK_24G_HOST_IDX
                    ? I_N4
                    : wireless_get_host_index() + CONFIG_KCLK_BT_KEY_START_IDX, RGB_WHITE);
            #else
                rgb_matrix_set_color(wireless_get_host_index() == 24 ? I_N4 : wireless_get_host_index() + 19, RGB_WHITE);
            #endif
            }
        }
        #endif
    #elif defined(KEYBOARD_IS_BRIDGE)
        #ifdef WIRELESS_ENABLE
        // CTL_LAYR will alwys show connection indicator; other layers for 4 seconds after wireless/battery keycode is used
        if (layer == KCTL_LAYR || (wls_action_timer && timer_elapsed32(wls_action_timer) < 4000)) {
            return true;
        }
        #endif
    #endif
    #ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
        if (warning_active &&
        #ifdef CONFIG_CUSTOM_BLINK_INTERVAL
            timer_elapsed32(last_blink) > CONFIG_CUSTOM_BLINK_INTERVAL
        #else
            timer_elapsed32(last_blink) > 250
        #endif
        ) {
            last_blink        = timer_read32();
            warning_led_state = !warning_led_state;
        }

        if (warning_active) {
            if (I_ESC >= led_min && I_ESC < led_max) {
                if (warning_led_state) {
                    rgb_matrix_set_color(I_ESC, RGB_RED);
                    for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                        rgb_matrix_set_color(I_ESC, RGB_RED);
                    }
                } else {
                    rgb_matrix_set_color(I_ESC, RGB_BLACK);
                    for (uint8_t i = 0; i < rgb_layer_indicators_count; i++) {
                        rgb_matrix_set_color(I_ESC, RGB_BLACK);
                    }
                }
            }
        }
    #endif
    }
    return rgb_matrix_indicators_keymap(led_min, led_max);
}

#ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
void matrix_scan_user(void) {
    #ifdef CONFIG_CUSTOM_SLEEP_WARNING
    if (!rgb_reached_timeout && !warning_active &&
        timer_elapsed32(rgb_last_activity_timer) > CONFIG_CUSTOM_SLEEP_TIMEOUT - CONFIG_CUSTOM_SLEEP_WARNING) {
    #else
    if (!rgb_reached_timeout && !warning_active &&
        timer_elapsed32(rgb_last_activity_timer) > CONFIG_CUSTOM_SLEEP_TIMEOUT - 5000) {
    #endif
        warning_active = true;
        #ifdef CONFIG_CUSTOM_BLINK_INTERVAL
        last_blink = timer_read32() - CONFIG_CUSTOM_BLINK_INTERVAL;
        #else
        last_blink = timer_read32() - 250;
        #endif
        warning_led_state = false;
    }
    if (!rgb_reached_timeout && timer_elapsed32(rgb_last_activity_timer) > CONFIG_CUSTOM_SLEEP_TIMEOUT) {
        #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        rgb_matrix_sethsv_noeeprom(0, 0, 50);
        #else
        rgb_matrix_disable_noeeprom();
        #endif
        warning_active = false;
        rgb_set_sleep_mode(true);
    }
    #ifdef CONFIG_CUSTOM_BT_TURN_OFF_DELAY
    if (bt_is_on && timer_elapsed32(rgb_last_activity_timer) > CONFIG_CUSTOM_SLEEP_TIMEOUT + CONFIG_CUSTOM_BT_TURN_OFF_DELAY) {
    #else
    if (bt_is_on && timer_elapsed32(rgb_last_activity_timer) > CONFIG_CUSTOM_SLEEP_TIMEOUT + 5000) {
    #endif
        #ifdef KEYBOARD_IS_BRIDGE
        wls_transport_enable(false);
        wait_ms(50);
        #endif
        bt_is_on = false;
        #if !defined(KEYBOARD_IS_KEYCHRON) && !defined(KEYBOARD_IS_LEMOKEY)
        suspend_power_down();
        #endif
    }
}
#endif

// the custom sleep and lock animation timeout routines set most of these values alreday
// but putting it here also ensures that if the keyboard enters suspend for any other reason
// the values are what they need to be
void suspend_power_down_user(void) {
    dprintf("suspend_power_down_user. . .");
    if (rgb_matrix_is_enabled()) {
        #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
        rgb_matrix_sethsv_noeeprom(0, 0, 50);
        #else
        rgb_matrix_disable_noeeprom();
        #endif
    }
    rgb_indicators_enabled = false;
    #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
    lock_anim_active = false;
    #endif
    #ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
    warning_active = false;
    rgb_reached_timeout = true;
    #endif
}

void suspend_wakeup_init_user(void) {
    rgb_indicators_enabled = false;
    #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
    lock_anim_timer = timer_read32();
    #endif
    #ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
    rgb_last_activity_timer = timer_read32();
    #endif
    #if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
    wakeup_if_not_connected();
    #else
    wait_ms(50);
    #endif
    rgb_matrix_reload_from_eeprom(); // restore saved mode & brightness
    housekeeping_restore_lock_anim = true;
    rgb_indicators_enabled = true;
}

// --- housekeeping (runs after layers are fully restored) ---
void housekeeping_task_user(void) {
    if (housekeeping_restore_lock_anim) {
        set_animation_if_lock_layr();
        housekeeping_restore_lock_anim = false; // only once per wake
    }
}

void set_animation_if_lock_layr(void) {
    if (get_highest_layer(layer_state) == LOCK_LAYR) {
        #ifdef CONFIG_LOCK_ANIMATION
        rgb_matrix_mode_noeeprom(CONFIG_LOCK_ANIMATION);
        #else
        rgb_matrix_mode_noeeprom(RGB_MATRIX_BAND_VAL);
        #endif
    }
}

void send_key_to_host_after_wait(uint8_t ms) {
    if (get_highest_layer(layer_state) == LOCK_LAYR) {
        wait_ms(ms);
        tap_code(KC_CAPS);
        tap_code(KC_CAPS);
    }
}

// determine the current tap dance state
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
        if (!state->pressed) {
            return TRIPLE_TAP;
        } else {
            return TRIPLE_HOLD;
        }
    } else if (state->count == 4) {
        return QUAD_TAP;
    } else if (state->count == 5) {
        return PENT_TAP;
    } else if (state->count == 6) {
        return HEXA_TAP;
    }
    else return 10;
}

// initialize tap structure associated with each tap dance key
static tap caps_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap ralt_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap fn_tap_state = {
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
static tap lgui_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap rcmd_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap lopt_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap ropt_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap rctl_tap_state = {
    .is_press_action = true,
    .state = 0
};
static tap macl_tap_state = {
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
            if (layer_state_is(KCTL_LAYR)) {
                //if already set, then switch it off
                layer_lock_off(KCTL_LAYR);
            } else {
                //if not already set, then switch the layer on
                layer_lock_on(KCTL_LAYR);
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
            if (!is_mac_base()) {
                if (layer_state_is(WSYM_LAYR)) {
                    //if already set, then switch it off
                    layer_lock_off(WSYM_LAYR);
                } else {
                    //if not already set, then switch the layer on
                    layer_lock_on(WSYM_LAYR);
                }
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

// function for ralt tap dance
void ralt_finished (tap_dance_state_t *state, void *user_data) {
    ralt_tap_state.state = cur_dance(state);
    switch (ralt_tap_state.state) {
        case SINGLE_TAP:
            set_oneshot_layer(KCTL_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case SINGLE_HOLD:
            register_code(KC_RALT);
            break;
        case DOUBLE_TAP:
            set_oneshot_layer(WSYM_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case DOUBLE_HOLD:
            layer_on(WSYM_LAYR);
            break;
    }
}

void ralt_reset (tap_dance_state_t *state, void *user_data) {
    switch (ralt_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
            unregister_code(KC_RALT);
            break;
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            if (!is_layer_locked(WSYM_LAYR)) {
                layer_off(WSYM_LAYR);
            }
            break;
    }
    ralt_tap_state.state = 0;
}

// function for rcmd tap dance
void rcmd_finished (tap_dance_state_t *state, void *user_data) {
    rcmd_tap_state.state = cur_dance(state);
    switch (rcmd_tap_state.state) {
        case SINGLE_TAP:
            set_oneshot_layer(KCTL_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case SINGLE_HOLD:
            register_code(KC_RCMD);
            break;
        case DOUBLE_TAP:
            set_oneshot_layer(EMO_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case DOUBLE_HOLD:
            layer_on(EMO_LAYR);
            break;
    }
}

void rcmd_reset (tap_dance_state_t *state, void *user_data) {
    switch (rcmd_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
            unregister_code(KC_RCMD);
            break;
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            if (!is_layer_locked(EMO_LAYR)) {
                layer_off(EMO_LAYR);
            }
            break;
    }
    rcmd_tap_state.state = 0;
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
            if (is_mac_base()) {
                set_oneshot_layer(MSYM_LAYR, ONESHOT_START);
                add_oneshot_mods(MOD_BIT(KC_LOPT));
                clear_oneshot_layer_state(ONESHOT_PRESSED);
            }
            else {
                set_oneshot_layer(WSYM_LAYR, ONESHOT_START);
                clear_oneshot_layer_state(ONESHOT_PRESSED);
            }
            break;
        case DOUBLE_HOLD:
            if (is_mac_base()) {
                register_code(KC_LOPT);
                layer_on(MSYM_LAYR);
            }
            else {
                layer_on(WSYM_LAYR);
            }
            break;
    #ifdef CONFIG_NO_RCTL_KEY
        case TRIPLE_HOLD:
            register_code(KC_RCTL);
            break;
    #endif
    }
}

void fn_reset (tap_dance_state_t *state, void *user_data) {
    switch (fn_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
            if (!is_layer_locked(FN_LAYR)) {
                layer_off(FN_LAYR);
            }
            break;
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            if (is_mac_base()) {
                if (!is_layer_locked(MSYM_LAYR)) {
                    unregister_code(KC_LOPT);
                    layer_off(MSYM_LAYR);
                }
            }
            else {
                if (!is_layer_locked(WSYM_LAYR)) {
                    layer_off(WSYM_LAYR);
                }
            }
            break;
    #ifdef CONFIG_NO_RCTL_KEY
        case TRIPLE_HOLD:
            unregister_code(KC_RCTL);
            break;
    #endif
    }
    fn_tap_state.state = 0;
}

// function for each press of rsft
// this is needed so that pressing both shifts will activate caps_word, even when one of the shifts is a tap dance
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
            // activate WIDE_LAYR
            if (IS_LAYER_ON(WIDE_LAYR)) {
                layer_lock_off(WIDE_LAYR);
            }
            else {
                layer_lock_on(WIDE_LAYR);
                wide_firstchar = true;
            }
            break;
        case TRIPLE_TAP:
            // activate CIRC_LAYR
            if (IS_LAYER_ON(CIRC_LAYR)) {
                layer_lock_off(CIRC_LAYR);
            }
            else {
                layer_lock_on(CIRC_LAYR);
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
            layer_off(LOCK_LAYR); // three taps unlocks the LOCK_LAYR
            #ifdef CONFIG_LOCK_ANIMATION_TIMEOUT
            lock_anim_active = false;
            #endif
            // in case dip switch was changed while in lock mode
            layer_move(get_highest_layer(default_layer_state));
            rgb_matrix_mode_noeeprom(user_config.rgb_mode);
            break;
        case SINGLE_HOLD:
            break;
    }
}

void kbunlock_reset (tap_dance_state_t *state, void *user_data) {
    kbunlock_tap_state.state = 0;
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
            if (user_config.is_linux_base) {
                symbol_key_linux("00e0","00c0");
            }
            else {
                symbol_key_win("0224","0192"); // a
            }
            break;
        case DOUBLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00e8","00c8");
            }
            else {
                symbol_key_win("0232","0200"); // e
            }
            break;
        case TRIPLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00ec","00cc");
            }
            else {
                symbol_key_win("0236","0204"); // i
            }
            break;
        case QUAD_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00f2","00d2");
            }
            else {
                symbol_key_win("0242","0210"); // o
            }
            break;
        case PENT_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00f9","00d9");
            }
            else {
                symbol_key_win("0249","0217"); // u
            }
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
            if (user_config.is_linux_base) {
                symbol_key_linux("00a1","00bc");
            }
            else {
                symbol_key_win("173","0188");  // inverted ! or 1/4
            }
            break;
        case DOUBLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00bd","00bd");
            }
            else {
                symbol_key_win("0189","0189"); // 1/2
            }
            break;
        case TRIPLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00be","00be");
            }
            else {
                symbol_key_win("0190","0190"); // 3/4
            }
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
            if (user_config.is_linux_base) {
                symbol_key_linux("00e9","00c9");
            }
            else {
                symbol_key_win("0233","0201"); // e
            }
            break;
        case DOUBLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00e1","00c1");
            }
            else {
                symbol_key_win("0225","0193"); // a
            }
            break;
        case TRIPLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00ed","00cd");
            }
            else {
                symbol_key_win("0237","0205"); // i
            }
            break;
        case QUAD_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00f3","00d3");
            }
            else {
                symbol_key_win("0243","0211"); // o
            }
            break;
        case PENT_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00fa","00da");
            }
            else {
                symbol_key_win("0250","0218"); // u
            }
            break;
        case HEXA_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00fd","00dd");
            }
            else {
                symbol_key_win("0253","0221"); // y
            }
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
            if (user_config.is_linux_base) {
                symbol_key_linux("00e4","00c4");
            }
            else {
                symbol_key_win("0228","0196"); // a
            }
            break;
        case DOUBLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00eb","00cb");
            }
            else {
                symbol_key_win("0235","0203"); // e
            }
            break;
        case TRIPLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00ef","00cf");
            }
            else {
                symbol_key_win("0239","0207"); // i
            }
            break;
        case QUAD_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00f6","00d6");
            }
            else {
                symbol_key_win("0246","0214"); // o
            }
            break;
        case PENT_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00fc","00dc");
            }
            else {
                symbol_key_win("0252","0220"); // u
            }
            break;
        case HEXA_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00ff","0178");
            }
            else {
                symbol_key_win("0255","0159"); // y
            }
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
            if (user_config.is_linux_base) {
                symbol_key_linux("00ee","00ce");
            }
            else {
                symbol_key_win("0238","0206"); // i
            }
            break;
        case DOUBLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00e2","00c2");
            }
            else {
                symbol_key_win("0226","0194"); // a
            }
            break;
        case TRIPLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00ea","00ca");
            }
            else {
                symbol_key_win("0234","0202"); // e
            }
            break;
        case QUAD_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00f4","00d4");
            }
            else {
                symbol_key_win("0244","0212"); // o
            }
            break;
        case PENT_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00fb","00db");
            }
            else {
                symbol_key_win("0251","0219"); // u
            }
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
            if (user_config.is_linux_base) {
                symbol_key_linux("00f1","00d1");
            }
            else {
                symbol_key_win("164","165");   // n
            }
            break;
        case DOUBLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00e3","00c3");
            }
            else {
                symbol_key_win("0227","0195"); // a
            }
            break;
        case TRIPLE_TAP:
            if (user_config.is_linux_base) {
                symbol_key_linux("00f5","00d5");
            }
            else {
                symbol_key_win("0245","0213"); // o
            }
            break;
    }
}

void actn_reset (tap_dance_state_t *state, void *user_data) {
    act_char_led_index = 0;
    actn_tap_state.state = 0;
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
            if (!is_mac_base()) {
                is_winkey_held = true;
            }
            break;
        case DOUBLE_TAP:
            set_oneshot_layer(WSYM_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case DOUBLE_HOLD:
            layer_on(WSYM_LAYR);
            break;
    }
}

void lgui_reset (tap_dance_state_t *state, void *user_data) {
    switch (lgui_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
            unregister_code(KC_LGUI);
            if (!is_mac_base()) {
                is_winkey_held = false;
            }
            break;
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            if (!is_layer_locked(WSYM_LAYR)) {
                layer_off(WSYM_LAYR);
            }
            break;
    }
    lgui_tap_state.state = 0;
}

// function for lopt tap dance
void lopt_finished (tap_dance_state_t *state, void *user_data) {
    lopt_tap_state.state = cur_dance(state);
    switch (lopt_tap_state.state) {
        case SINGLE_TAP:
            tap_code(KC_LOPT);
            break;
        case SINGLE_HOLD:
        case DOUBLE_HOLD:
            register_code(KC_LOPT);
            if (get_highest_layer(layer_state) < FN_LAYR) {
                layer_on(MSYM_LAYR);
            }
            break;
        case DOUBLE_TAP:
            set_oneshot_layer(MSYM_LAYR, ONESHOT_START);
            add_oneshot_mods(MOD_BIT(KC_LOPT));
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
    }
}

void lopt_reset (tap_dance_state_t *state, void *user_data) {
    switch (lopt_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
        case DOUBLE_HOLD:
            if (!is_layer_locked(MSYM_LAYR)) {
                unregister_code(KC_LOPT);
                layer_off(MSYM_LAYR);
            }
            break;
        case DOUBLE_TAP:
            break;
    }
    lopt_tap_state.state = 0;
}

// function for ropt tap dance
void ropt_finished (tap_dance_state_t *state, void *user_data) {
    ropt_tap_state.state = cur_dance(state);
    switch (ropt_tap_state.state) {
        case SINGLE_TAP:
            set_oneshot_layer(MSYM_LAYR, ONESHOT_START);
            add_oneshot_mods(MOD_BIT(KC_LOPT));
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case SINGLE_HOLD:
            register_code(KC_LOPT);
            if (is_base_layer(get_highest_layer(layer_state))) {
                layer_on(MSYM_LAYR);
            }
            break;
        case DOUBLE_TAP:
            set_oneshot_layer(EMO_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case DOUBLE_HOLD:
            layer_on(EMO_LAYR);
            break;
    }
}

void ropt_reset (tap_dance_state_t *state, void *user_data) {
    switch (ropt_tap_state.state) {
        case SINGLE_TAP:
            break;
        case SINGLE_HOLD:
            if (!is_layer_locked(MSYM_LAYR)) {
                unregister_code(KC_LOPT);
                layer_off(MSYM_LAYR);
            }
            break;
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            if (!is_layer_locked(EMO_LAYR)) {
                layer_off(EMO_LAYR);
            }
            break;
    }
    ropt_tap_state.state = 0;
}

// function for rctl tap dance
void rctl_finished (tap_dance_state_t *state, void *user_data) {
    rctl_tap_state.state = cur_dance(state);
    switch (rctl_tap_state.state) {
        case SINGLE_TAP:
            set_oneshot_layer(KCTL_LAYR, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case SINGLE_HOLD:
            register_code(KC_RCTL);
            break;
        case DOUBLE_TAP:
            if (is_mac_base()) {
                set_oneshot_layer(EMO_LAYR, ONESHOT_START);
                clear_oneshot_layer_state(ONESHOT_PRESSED);
            }
            break;
        case DOUBLE_HOLD:
            if (is_mac_base()) {    
                layer_on(EMO_LAYR);
            } 
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
        case DOUBLE_TAP:
            break;
        case DOUBLE_HOLD:
            if (is_mac_base() && !is_layer_locked(EMO_LAYR)) {
                layer_off(EMO_LAYR);
            }
            break;
    }
    rctl_tap_state.state = 0;
}

// function for macl tap dance
void macl_finished (tap_dance_state_t *state, void *user_data) {
    macl_tap_state.state = cur_dance(state);
    switch (macl_tap_state.state) {
        case SINGLE_TAP:
        case SINGLE_HOLD:
            tap_code(KC_MS_ACCEL0);
            break;
        case DOUBLE_TAP:
            tap_code(KC_MS_ACCEL1);
            break;
        case TRIPLE_TAP:
            tap_code(KC_MS_ACCEL2);
            break;
    }
}

void macl_reset (tap_dance_state_t *state, void *user_data) {
    if (macl_tap_state.state == SINGLE_HOLD) {
        tap_code(KC_MS_ACCEL1);
    }
    macl_tap_state.state = 0;
}

// associate the tap dance keys with their funcitons
tap_dance_action_t tap_dance_actions[17] = {
    [CAPS_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, caps_finished, caps_reset),
    [FN_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_finished, fn_reset),
    [RALT_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ralt_finished, ralt_reset),
    [RSFT_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(rsft_each, rsft_finished, rsft_reset),
    [KB_UNLOCK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, kbunlock_finished, kbunlock_reset),
    [ACT_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(actgrv_each, actgrv_finished, actgrv_reset),
    [ACT_1] = ACTION_TAP_DANCE_FN_ADVANCED(act1_each, act1_finished, act1_reset),
    [ACT_E] = ACTION_TAP_DANCE_FN_ADVANCED(acte_each, acte_finished, acte_reset),
    [ACT_U] = ACTION_TAP_DANCE_FN_ADVANCED(actu_each, actu_finished, actu_reset),
    [ACT_I] = ACTION_TAP_DANCE_FN_ADVANCED(acti_each, acti_finished, acti_reset),
    [ACT_N] = ACTION_TAP_DANCE_FN_ADVANCED(actn_each, actn_finished, actn_reset),
    [LGUI_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lgui_finished, lgui_reset),
    [RCMD_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rcmd_finished, rcmd_reset),
    [LOPT_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lopt_finished, lopt_reset),
    [ROPT_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ropt_finished, ropt_reset),
    [RCTL_OSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rctl_finished, rctl_reset),
    [MOUSE_ACCEL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, macl_finished, macl_reset)
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

// jiggler to keep from screen timeout without adjusting power settings
void jiggle_mouse(void) {
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

// send mac unicode
void symbol_key_mac(const char *unicode, const char *shift_unicode) {
    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    clear_mods();
    tap_code16(C(A(G(KC_SPC)))); // switch os keyboard to unicode
    add_mods(MOD_MASK_ALT);
    send_string(((mods | oneshot_mods) & (MOD_MASK_SHIFT | MOD_MASK_GUI)) ? shift_unicode : unicode);
    del_mods(MOD_MASK_ALT);
    tap_code16(C(A(G(KC_SPC)))); // switch back from unicode
    register_mods(mods);
}

void symbol_key_win(const char *alt_code, const char *shift_alt_code) {
    // get current mod states
    const uint8_t mods = get_mods();
    const uint8_t weak_mods = get_weak_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    const char *ucode = ((mods | weak_mods | oneshot_mods) & MOD_MASK_SHIFT) ? shift_alt_code : alt_code;
    bool numlockChanged; // this numlock stuff lets this work in all scenarios
    numlockChanged = false;
    clear_mods();
    clear_weak_mods();
    if (ucode == NULL || *ucode == '\0') { // null or empty string
        return;
    }
    if (!host_keyboard_led_state().num_lock) {
         tap_code(KC_NUM);
         numlockChanged = true;
    }
    register_code(KC_LALT); // hold down alt
    type_numpad_keys_from_string(ucode); // send to fn to type numpad keys
    unregister_code(KC_LALT); // release alt
    if (numlockChanged) {
        tap_code(KC_NUM);
    }
    register_mods(mods);
}

void symbol_key_linux(const char *hex_code, const char *shift_hex_code) {
    // get current mod states
    const uint8_t mods = get_mods();
    const uint8_t weak_mods = get_weak_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();
    const char *ucode = ((mods | weak_mods | oneshot_mods) & MOD_MASK_SHIFT) ? shift_hex_code : hex_code;
    if (ucode == NULL || *ucode == '\0') { // null or empty string
        return;
    }
    clear_mods();
    clear_weak_mods();
    tap_code16(C(S(KC_U))); // start the unicode sequence
    // type the hex chars
    send_string_with_delay(ucode,5);
    // finish sequence
    tap_code(KC_SPC);
    register_mods(mods); // add back mods
}

// send_string doesn't use the numpad, so this fn was created to type numbers using the numpad
void type_numpad_keys_from_string(const char *stringnum) {
    // now need to loop through each char in the char pointer until a null terminator
    const char *ptr = stringnum;
    while (*ptr != '\0') {
        switch (*ptr) {
        case '0':
            dprintf("0 tap\n");
            tap_code(KC_P0);
            break;
        case '1':
            dprintf("1 tap\n");
            tap_code(KC_P1);
            break;
        case '2':
            dprintf("2 tap\n");
            tap_code(KC_P2);
            break;
        case '3':
            dprintf("3 tap\n");
            tap_code(KC_P3);
            break;
        case '4':
            dprintf("4 tap\n");
            tap_code(KC_P4);
            break;
        case '5':
            dprintf("5 tap\n");
            tap_code(KC_P5);
            break;
        case '6':
            dprintf("6 tap\n");
            tap_code(KC_P6);
            break;
        case '7':
            dprintf("7 tap\n");
            tap_code(KC_P7);
            break;
        case '8':
            dprintf("8 tap\n");
            tap_code(KC_P8);
            break;
        case '9':
            dprintf("9 tap\n");
            tap_code(KC_P9);
            break;
        default:
            dprintf("no number\n");
            break;
        }
    ptr++;
    }
}

// a function to check for if a key press should fade the rgb
bool key_should_fade(keytracker key, uint8_t layer) {
    bool should_fade = true;
    if ((key.fade < 1) ||
#ifdef CONFIG_CUSTOM_DO_NOT_FADE
        CONFIG_CUSTOM_DO_NOT_FADE ||
#endif
        ((layer == FN_LAYR || layer == SFT_LAYR || layer == WIDE_LAYR ||
        layer == CIRC_LAYR || is_caps_word_on()) &&
        (key.index == I_LSFT || key.index == I_RSFT)) ||                                              // l/r shift
        ((layer == FN_LAYR || layer == KCTL_LAYR) && 
                                (key.index == I_LOPT ||
                                #ifdef CONFIG_HAS_ROPT_KEY
                                 key.index == I_ROPT ||
                                #endif
                                 key.index == I_LCMD
                                #ifndef CONFIG_NO_RCMD_KEY
                                 || key.index == I_RCMD
                                #endif
                                )) ||                                                                 // l/r alt cmd
        (macro_recording && (key.index == I_MREC1 || key.index == I_MREC2)) ||                        // macro recording keys
#ifdef CONFIG_HAS_LLOCK_KEY
        (is_layer_locked(layer) && key.index == I_LLOCK) ||                                           // layer lock key
#endif
        (is_in_leader_sequence && key.index == I_LEAD) ||                                             // leader key
        (layer == SFT_LAYR && (key.index == I_NUMLOCK || key.index == I_MHLD)) ||                     // num lock, mouse hold
        (layer == FN_LAYR && key.index == I_SLOCK) ||                                                 // scroll lock
        (layer == WIDE_LAYR && (key.index == I_BARTEXT || key.index == I_STHRU ||
        key.index == I_UNDERLN || key.index == I_BBRTEXT)) ||                                         // wide-text toggles
        (layer == KCTL_LAYR && (key.index == I_FJLIGHT || key.index == I_HROWLIGHT || 
                                key.index == I_KTRACK
#ifdef CONFIG_HAS_SECOND_KTRACK_KEY 
                                || key.index == I_KTRACK2
#endif
#ifdef CONFIG_HAS_SECOND_FJLIGHT_KEY
                                || key.index == I_FJLIGHT2
#endif
#ifdef CONFIG_HAS_SECOND_HROWLIGHT_KEY
                                || key.index == I_HROWLIGHT2
#endif
                                )) ||                                                                 // ktrack/hrow/fj indicators
        (layer == KCTL_LAYR && (key.index >= I_N1 && key.index <= I_N4)) ||                           // wireless mode keys
        (os_changed) ||                                                                               // mac/win/lin change
        (layer == WSYM_LAYR && (key.index == I_GRV || key.index == I_N1 || key.index == I_E ||
                                key.index == I_I || key.index == I_U || key.index == I_N ||           // accent keys
                            #ifndef CONFIG_NO_RALT_KEY
                                key.index == I_RALT ||
                            #endif
                                key.index == I_LGUI)) ||                                              // sym_layr ralt, lgui
#ifdef CONFIG_HAS_ROPT_KEY
        (layer == MSYM_LAYR && (key.index == I_LOPT || key.index == I_ROPT)) ||                       // sym_layr lopt, ropt
        (layer == EMO_LAYR && (
                            #ifndef CONFIG_NO_RCMD_KEY
                                key.index == I_RCMD ||
                            #endif
                                 key.index == I_ROPT)) ||                                             // emo_layr rcmd, rpot
#else
        (layer == MSYM_LAYR && key.index == I_LOPT) ||                                                // sym_layr lopt, ropt
    #ifndef CONFIG_NO_RCMD_KEY
        (layer == EMO_LAYR && key.index == I_RCMD) ||                                                 // emo_layr rcmd, rpot
    #endif
#endif
        (key.index == I_CAPS || key.index == I_FN || key.index == I_TAB)) {                           // caps lock, fn, tab
            should_fade = false;
        }
    return should_fade;
}

// led indexes for keys that get capitalized when caps lock is on
bool is_capslock_shifted(uint8_t i) {
#ifdef CONFIG_CAPSLOCK_SHIFTED
    if (CONFIG_CAPSLOCK_SHIFTED) {
        return true;
    }
#endif
    return false;
}
// led indexes for keys that get shifted when caps word is on
bool is_capsword_shifted(uint8_t i) {
#ifdef CONFIG_CAPSWORD_EXTRA
    if (CONFIG_CAPSWORD_EXTRA || is_capslock_shifted(i)) {
#else
    if (is_capslock_shifted(i)) {
#endif
        return true;
    }
    return false;
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

// callback for when a mcaro on osl is run (to turn off the layer)
uint32_t osl_macro_callback(uint32_t trigger_time, void *cb_arg) {
    layer_off(FN_LAYR);
    return 0;
}
// callback to deactivate a simulated osl layer
uint32_t sim_osl_callback(uint32_t trigger_time, void* cb_arg) {
    dprintf("sim_osl_callback running\n");
    uint8_t layer = get_highest_layer(layer_state);
    if (!is_layer_locked(layer)) {
        layer_off(layer);
    }
    sim_osl = false;
    return 0;
}
// callback to turn off app-switch mode
uint32_t cmd_tab_callback(uint32_t trigger_time, void* cb_arg) {
    unregister_code(is_mac_base() || user_config.is_linux_base ? KC_RCMD : KC_RALT);
    unregister_code(KC_LSFT);
    is_cmd_tab_active = false;
    is_cmd_shift_tab_active = false;
    return 0;
}
// if a leader seuqence errored, rgbs can be set to blink for a time until this callback is used
uint32_t leader_error_callback(uint32_t trigger_time, void* cb_arg) {
    is_leader_error = false;
    is_leader_error_led_on = false;
    return 0;
}
#if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
// callback to return enbale keytracker after a delay to see the wireless status indicator
uint32_t wireless_mode_callback(uint32_t trigger_time, void *cb_arg) {
    enable_keytracker = true;
    wireless_mode_token = INVALID_DEFERRED_TOKEN;
    // get rid of any lingering fade
    for (int i = 0; i < tk_length; i++) {
        tracked_keys[i].press = false;
        tracked_keys[i].fade = 0;
    }
    return 0;
}
#endif

// setup to store vars when macro recording starts or ends. then can flash some rgb
#if defined(KEYBOARD_IS_BRIDGE) || defined(KEYBOARD_IS_YUNZII)
bool dynamic_macro_record_start_user(int8_t direction) {
#else
void dynamic_macro_record_start_user(int8_t direction) {
#endif
    macro_direction = direction;
    macro_recording = true;
    macro_timer = timer_read();
#if defined(KEYBOARD_IS_BRIDGE) || defined(KEYBOARD_IS_YUNZII)
    return true;
#endif
}
#if defined(KEYBOARD_IS_BRIDGE) || defined(KEYBOARD_IS_YUNZII)
bool dynamic_macro_record_end_user(int8_t direction) {
#else
void dynamic_macro_record_end_user(int8_t direction) {
#endif
    macro_direction = direction;
    macro_recording = false;
    is_macro_led_on = false;
    // this loop is needed to prevent a stuck led after a macro finishes recording
    for (int i = 0; i < tk_length; i++) {
        if (tracked_keys[i].index == I_MREC1 || tracked_keys[i].index == I_MREC2) {
            tracked_keys[i].press = false;
            tracked_keys[i].fade = 0;
        }
    }
#if defined(KEYBOARD_IS_BRIDGE) || defined(KEYBOARD_IS_YUNZII)
    return true;
#endif
}
// this is so the macro key lights don't get stuck when i play the macro
#if defined(KEYBOARD_IS_BRIDGE) || defined(KEYBOARD_IS_YUNZII)
bool dynamic_macro_play_user(int8_t direction) {
#else
void dynamic_macro_play_user(int8_t direction) {
#endif
    for (int i = 0; i < tk_length; i++) {
        if (tracked_keys[i].index == I_MPLY1 || tracked_keys[i].index == I_MPLY2) {
            tracked_keys[i].press = false;
            tracked_keys[i].fade = 0;
        }
    }
#if defined(KEYBOARD_IS_BRIDGE) || defined(KEYBOARD_IS_YUNZII)
    return true;
#endif
}

void oneshot_layer_changed_user(uint8_t layer) {
    if (layer) {
        oneshot_layer_active = true;
    }
    else {
        oneshot_layer_active = false;
    }
}

void leader_start_user(void) {
    is_in_leader_sequence = true;
}

// fade the rgb animation when layer is changed so that the layer keys are more prominent
layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_base_layer(get_highest_layer(state))) {
        rgb_matrix_reload_from_eeprom();
    }
    else if (get_highest_layer(state) != LOCK_LAYR) {
        HSV hsv = rgb_matrix_get_hsv();
        if (hsv.v >= 180) {
            rgb_matrix_sethsv_noeeprom(hsv.h, hsv.s, 180);
        }
    }
    return state;
}

void layer_lock_set_user(layer_state_t locked_layers) {
    static bool opt_is_held_for_symbol = false;
    reset_oneshot_layer();
    clear_oneshot_mods();
    if (is_layer_locked(MSYM_LAYR)) {
        register_code(KC_LOPT);
        opt_is_held_for_symbol = true;
    }
    else if (opt_is_held_for_symbol) {
        unregister_code(KC_LOPT);
        opt_is_held_for_symbol = false;
    }
}

// for tracking if base is mac
bool is_mac_base(void) {
    uint8_t base = get_highest_layer(default_layer_state);
    for (uint8_t i = 0; i < macos_base_layers_count; i++) {
        if (base == macos_base_layers[i]) {
            return true;
        }
    }
    return false;
}

// for checking if the layer is one of the possible base layers
bool is_base_layer(uint8_t layer) {
    for (uint8_t i = 0; i < all_base_layers_count; i++) {
        if (layer == all_base_layers[i]) {
            return true;
        }
    }
    return false;
}

bool app_switch_active(void) {
    return is_cmd_tab_active || is_cmd_shift_tab_active;
}

#if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
void wireless_transport_enable(bool enable) {
    switch (get_transport()) {
        case TRANSPORT_USB:
            usb_transport_enable(enable);
            if (enable) {
                bt_transport_enable(false);
                p24g_transport_enable(false);
            }
            break;

        case TRANSPORT_BLUETOOTH:
            bt_transport_enable(enable);
            if (enable) {
                usb_transport_enable(false);
                p24g_transport_enable(false);
            }
            break;

        case TRANSPORT_P2P4:
            p24g_transport_enable(enable);
            if (enable) {
                usb_transport_enable(false);
                bt_transport_enable(false);
            }
            break;

        default:
            usb_transport_enable(false);
            bt_transport_enable(false);
            p24g_transport_enable(false);
            break;
    }
}
#endif

#if defined(CONFIG_CUSTOM_SLEEP_TIMEOUT) || defined(CONFIG_LOCK_ANIMATION_TIMEOUT)
#if defined(KEYBOARD_IS_KEYCHRON) || defined(KEYBOARD_IS_LEMOKEY)
void wakeup_if_not_connected(void) {
    transport_t t = get_transport();
    wt_state_t state = wireless_get_state();
    // Check if any wireless transport is active and connected
    if ((t & TRANSPORT_WIRELESS) && state != WT_CONNECTED) {
        wireless_transport_enable(true);
        uint32_t timeout = timer_read32() + 400;  // max 400ms wait
        while (wireless_get_state() != WT_CONNECTED && timer_elapsed32(timeout) < 0) {
            wait_ms(5);
        }
        send_key_to_host_after_wait(50);
    }
}
#endif
#endif

#ifdef CONFIG_CUSTOM_SLEEP_TIMEOUT
void matrix_init_user(void) {
    rgb_last_activity_timer = timer_read32();
}
#endif

void keyboard_post_init_user(void) {
    // read the user config from EEPROM
    user_config.raw = eeconfig_read_user();
    // and set this so layers switch correctly on user's first os change
    layer_state_set(default_layer_state);
    rgb_matrix_mode(user_config.rgb_mode);
}

void eeconfig_init_user(void) {  // EEPROM is getting reset!
    user_config.raw = 0;
#ifdef CONFIG_EEPROM_RESET_DEFAULT_IS_LINUX
    user_config.is_linux_base = true; // set default here
#else
    user_config.is_linux_base = false; // set default here
#endif
    user_config.rgb_mode = RGB_MATRIX_DEFAULT_MODE;
    eeconfig_update_user(user_config.raw); // write default value to EEPROM now
#if defined(CONFIG_HAS_BASE_LAYER_TOGGLE)
    #ifdef CONFIG_SWITCH_PIN
    if (!readPin(CONFIG_SWITCH_PIN)) {
    #else
    if (!readPin(B12)) {
    #endif
    #ifdef CONFIG_DEFAULT_WIN_LAYR
        set_single_persistent_default_layer(CONFIG_DEFAULT_WIN_LAYR);
    #else
        set_single_persistent_default_layer(1);
    #endif
    }
    else {
    #ifdef CONFIG_DEFAULT_MAC_LAYR
        set_single_persistent_default_layer(CONFIG_DEFAULT_MAC_LAYR);
    #else
        set_single_persistent_default_layer(0);
    #endif
    }
#elif defined(CONFIG_EEPROM_RESET_DEFAULT_LAYER)
    set_single_persistent_default_layer(CONFIG_EEPROM_RESET_DEFAULT_LAYER);
#else
    set_single_persistent_default_layer(0);
#endif
}
