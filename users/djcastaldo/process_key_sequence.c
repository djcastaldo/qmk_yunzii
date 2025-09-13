// process_key_sequence.c
// @davex : Dave Castaldo
// 08/30/2025
// this is an alternative way to send a sequence of keycodes with delays
// created because send_string_with_delay() wasn't working reliably over a windows remote desktop connection

#include "process_key_sequence.h"

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
static bool is_shift_pressed_for_seq = false;

// --- helpers ---
static void release_all_held(key_sequence_t *s) {
    for (uint8_t i = 0; i < s->held_count; i++) {
        unregister_code(s->held_keys[i]);
    }
    s->held_count = 0;
    is_shift_pressed_for_seq = false;
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

// --- convert a char to key actions safely ---
static uint8_t char_to_actions(char c, key_action_t *actions) {
    uint8_t count = 0;
    bool shift_needed = false;
    uint16_t keycode = KC_NO;

    // Determine base keycode
    if (c >= 'a' && c <= 'z') {
        keycode = KC_A + (c - 'a');
    } else if (c >= 'A' && c <= 'Z') {
        keycode = KC_A + (c - 'A');
        shift_needed = true;
    } else if (c >= '1' && c <= '9') {
        keycode = KC_1 + (c - '1');
    } else {
        switch (c) {
            case '0': keycode = KC_0; break;
            case ' ': keycode = KC_SPC; break;
            case '\n': keycode = KC_ENT; break;
            case '\t': keycode = KC_TAB; break;
            case '!': keycode = KC_1; shift_needed = true; break;
            case '@': keycode = KC_2; shift_needed = true; break;
            case '#': keycode = KC_3; shift_needed = true; break;
            case '$': keycode = KC_4; shift_needed = true; break;
            case '%': keycode = KC_5; shift_needed = true; break;
            case '^': keycode = KC_6; shift_needed = true; break;
            case '&': keycode = KC_7; shift_needed = true; break;
            case '*': keycode = KC_8; shift_needed = true; break;
            case '(': keycode = KC_9; shift_needed = true; break;
            case ')': keycode = KC_0; shift_needed = true; break;
            case '-': keycode = KC_MINS; break;
            case '_': keycode = KC_MINS; shift_needed = true; break;
            case '=': keycode = KC_EQL; break;
            case '+': keycode = KC_EQL; shift_needed = true; break;
            case '[': keycode = KC_LBRC; break;
            case '{': keycode = KC_LBRC; shift_needed = true; break;
            case ']': keycode = KC_RBRC; break;
            case '}': keycode = KC_RBRC; shift_needed = true; break;
            case '\\': keycode = KC_BSLS; break;
            case '|': keycode = KC_BSLS; shift_needed = true; break;
            case ';': keycode = KC_SCLN; break;
            case ':': keycode = KC_SCLN; shift_needed = true; break;
            case '\'': keycode = KC_QUOT; break;
            case '"': keycode = KC_QUOT; shift_needed = true; break;
            case ',': keycode = KC_COMM; break;
            case '<': keycode = KC_COMM; shift_needed = true; break;
            case '.': keycode = KC_DOT; break;
            case '>': keycode = KC_DOT; shift_needed = true; break;
            case '/': keycode = KC_SLSH; break;
            case '?': keycode = KC_SLSH; shift_needed = true; break;
            case '`': keycode = KC_GRV; break;
            case '~': keycode = KC_GRV; shift_needed = true; break;
            default: keycode = KC_NO; break;
        }
    }

    if (keycode == KC_NO) return 0;

    // Press Shift first if needed
    if (shift_needed) {
        if (!is_shift_pressed_for_seq) {
            actions[count++] = (key_action_t){.keycode = KC_LSFT, .press = true, .delay_ms = CONFIG_RDP_DELAY_MOD};
            is_shift_pressed_for_seq = true;
        }
    } // release shift if it isn't needed and was pressed in a previous action
    else if (is_shift_pressed_for_seq) {
        actions[count++] = (key_action_t){.keycode = KC_LSFT, .press = false, .delay_ms = CONFIG_RDP_DELAY_MOD};
        is_shift_pressed_for_seq = false;
    }

    // Press the key
    actions[count++] = (key_action_t){.keycode = keycode, .press = true, .delay_ms = CONFIG_RDP_DELAY_KEY};

    // Release the key
    actions[count++] = (key_action_t){.keycode = keycode, .press = false, .delay_ms = CONFIG_RDP_DELAY_KEY};

    // Add tiny "post-key" pause to ensure OS registers properly
    actions[count++] = (key_action_t){.keycode = KC_NO, .press = false, .delay_ms = 10};

    return count;
}

void rdp_send_string(const char *str) {
    for (int i = 0; i < CONFIG_MAX_SEQ_QUEUE; i++) release_all_held(&sequences[i]);

    while (*str) {
        key_action_t seq[CONFIG_MAX_SEQ_LEN];
        uint8_t seq_len = 0;

        // fill sequence by characters, never splitting one character's actions
        while (*str) {
            key_action_t temp[7]; // max actions per char
            uint8_t count = char_to_actions(*str, temp);

            // if current sequence cannot fit this character, break and start a new sequence
            if (seq_len + count > CONFIG_MAX_SEQ_LEN) break;

            for (uint8_t i = 0; i < count; i++) seq[seq_len++] = temp[i];
            str++;
        }

        if (seq_len > 0) start_key_sequence(seq, seq_len);

        // **add a tiny pause between sequences**
        key_action_t pause_seq[] = { { KC_NO, false, 50 } }; // 50 ms pause
        START_KEY_SEQUENCE(pause_seq);
    }
}
