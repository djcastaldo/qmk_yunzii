// 08/30/2025 @davex
// Dave Castaldo
// process_key_sequence.h

#pragma once
#include "quantum.h"

#ifndef CONFIG_MAX_SEQ_LEN
    #define CONFIG_MAX_SEQ_LEN   128
#endif
#ifndef CONFIG_MAX_SEQ_QUEUE
    #define CONFIG_MAX_SEQ_QUEUE 10
#endif
#ifndef CONFIG_MAX_KEYS_HELD
    #define CONFIG_MAX_KEYS_HELD 8
#endif
// --- configuration for RDP delays ---
#ifndef CONFIG_RDP_DELAY_KEY
    #define CONFIG_RDP_DELAY_KEY 15    // ms between key press/release
#endif
#ifndef CONFIG_RDP_DELAY_MOD
    #define CONFIG_RDP_DELAY_MOD 20    // ms for modifiers
#endif

//
// public types
//
typedef struct {
    uint16_t keycode;   // qmk keycode, e.g. KC_LCTL, KC_A
    bool press;         // true = press, false = release
    uint16_t delay_ms;  // ms wait before this action
} key_action_t;

//
// public functions
//

// start a sequence (non-blocking).
void start_key_sequence(key_action_t *seq, uint8_t len);

// run the sequencer.
void process_key_sequence(void);

// for converting a string to sequeunce actions
void rdp_send_string(const char *str);

// convenience macro: automatically calculate length of a static array
#define START_KEY_SEQUENCE(seq) start_key_sequence(seq, sizeof(seq) / sizeof((seq)[0]))
