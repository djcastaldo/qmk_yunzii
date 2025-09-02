// 08/30/2025 @davex
// Dave Castaldo
// process_key_sequence.h

#pragma once
#include "quantum.h"

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

// convenience macro: automatically calculate length of a static array
#define START_KEY_SEQUENCE(seq) start_key_sequence(seq, sizeof(seq) / sizeof((seq)[0]))
