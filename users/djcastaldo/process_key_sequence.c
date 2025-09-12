// process_key_sequence.c
// @davex : Dave Castaldo
// 08/30/2025
// this is an alternative way to send a sequence of keycodes with delays
// created because send_string_with_delay() wasn't working reliably over a windows remote desktop connection

#include "process_key_sequence.h"

#ifndef CONFIG_MAX_SEQ_LEN
    #define CONFIG_MAX_SEQ_LEN   32
#endif
#ifndef CONFIG_MAX_SEQ_QUEUE
    #define CONFIG_MAX_SEQ_QUEUE 10
#endif
#ifndef CONFIG_MAX_KEYS_HELD
    #define CONFIG_MAX_KEYS_HELD 8
#endif

typedef struct {
    key_action_t actions[CONFIG_MAX_SEQ_LEN];
    uint8_t len;
    uint8_t index;
    uint16_t timer;
    bool active;
    bool waiting;
    uint16_t held_keys[CONFIG_MAX_KEYS_HELD];
    uint8_t held_count;
} key_sequence_t;

static key_sequence_t sequences[CONFIG_MAX_SEQ_QUEUE];
static int active_seq = -1;  // which sequence currently owns the keyboard

// --- helpers ---
static void release_all_held(key_sequence_t *s) {
    for (uint8_t i = 0; i < s->held_count; i++) {
        unregister_code(s->held_keys[i]);
    }
    s->held_count = 0;
}

// start a new sequence
void start_key_sequence(key_action_t *seq, uint8_t len) {
    for (int i = 0; i < CONFIG_MAX_SEQ_QUEUE; i++) {
        if (!sequences[i].active && !sequences[i].waiting) {
            memcpy(sequences[i].actions, seq, len * sizeof(key_action_t));
            sequences[i].len = len;
            sequences[i].index = 0;
            sequences[i].timer = timer_read();
            sequences[i].active = true;
            sequences[i].waiting = false;
            sequences[i].held_count = 0;
            // if no active owner, this one becomes active immediately
            if (active_seq == -1) {
                active_seq = i;
            } else {
                sequences[i].waiting = true; // wait until current finishes
            }
            return;
        }
    }
}

// --- engine ---
void process_key_sequence(void) {
    if (active_seq == -1) return;

    key_sequence_t *s = &sequences[active_seq];
    if (!s->active) return;

    if (timer_elapsed(s->timer) >= s->actions[s->index].delay_ms) {
        key_action_t *a = &s->actions[s->index];

        if (a->press) {
            register_code(a->keycode);
            if (s->held_count < CONFIG_MAX_KEYS_HELD) {
                s->held_keys[s->held_count++] = a->keycode;
            }
        } else {
            unregister_code(a->keycode);
            // loops to move items in held_keys to close the gap, and decrement held_count
            for (uint8_t k = 0; k < s->held_count; k++) {
                if (s->held_keys[k] == a->keycode) {
                    for (uint8_t m = k; m < s->held_count - 1; m++) {
                        s->held_keys[m] = s->held_keys[m+1];
                    }
                    s->held_count--;
                    break;
                }
            }
        }

        s->index++;
        s->timer = timer_read();

        if (s->index >= s->len) {
            release_all_held(s);
            s->active = false;
            active_seq = -1;

            // hand control to the next waiting sequence
            for (int i = 0; i < CONFIG_MAX_SEQ_QUEUE; i++) {
                if (sequences[i].active && sequences[i].waiting) {
                    sequences[i].waiting = false;
                    active_seq = i;
                    break;
                }
            }
        }
    }
}
