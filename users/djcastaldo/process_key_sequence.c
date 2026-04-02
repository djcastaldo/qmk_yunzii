// process_key_sequence.c
// @davex : Dave Castaldo
// Minimal queued key sequence engine (RDP-safe, non-blocking)

#include "process_key_sequence.h"
#include <string.h>

// ----------------------------
// configuration
// ----------------------------

#ifndef CONFIG_MAX_SEQ_QUEUE
    #define CONFIG_MAX_SEQ_QUEUE 3
#endif

// ----------------------------
// sequence state
// ----------------------------

typedef struct {
    key_action_t actions[CONFIG_MAX_SEQ_LEN];
    uint8_t len;
    uint8_t index;
    uint32_t next_time;
    bool active;
    uint16_t held_keys[CONFIG_MAX_KEYS_HELD];
    uint8_t held_count;
    bool shift_active;
} key_sequence_t;

static key_sequence_t seq_queue[CONFIG_MAX_SEQ_QUEUE];
static uint8_t seq_head = 0;
static uint8_t seq_tail = 0;
static int8_t active_seq = -1;

// ----------------------------
// helpers
// ----------------------------

static void release_all_held(key_sequence_t *s) {
    for (uint8_t i = 0; i < s->held_count; i++) {
        unregister_code(s->held_keys[i]);
    }
    s->held_count = 0;
    s->shift_active = false;
}

static bool queue_is_full(void) {
    return ((seq_tail + 1) % CONFIG_MAX_SEQ_QUEUE) == seq_head;
}

static bool queue_is_empty(void) {
    return seq_head == seq_tail;
}

// ----------------------------
// start sequence (enqueue)
// ----------------------------

void start_key_sequence(key_action_t *seq, uint8_t len) {
    if (len == 0) return;
    if (queue_is_full()) return; // drop if full (safe + simple)

    key_sequence_t *s = &seq_queue[seq_tail];

    memcpy(s->actions, seq, len * sizeof(key_action_t));
    s->len = len;
    s->index = 0;
    s->next_time = timer_read32();
    s->active = true;
    s->held_count = 0;
    s->shift_active = false;

    seq_tail = (seq_tail + 1) % CONFIG_MAX_SEQ_QUEUE;

    // if nothing running, start immediately
    if (active_seq == -1) {
        active_seq = seq_head;
    }
}

// ----------------------------
// engine (call from matrix_scan_user)
// ----------------------------

bool process_key_sequence(void) {
    // nothing running → try to start next
    if (active_seq == -1) {
        if (queue_is_empty()) return false;
        active_seq = seq_head;
    }

    key_sequence_t *s = &seq_queue[active_seq];

    if (!s->active) return false;

    // finished?
    if (s->index >= s->len) {
        release_all_held(s);
        s->active = false;

        // advance queue
        seq_head = (seq_head + 1) % CONFIG_MAX_SEQ_QUEUE;

        if (queue_is_empty()) {
            active_seq = -1;
        } else {
            active_seq = seq_head;
        }

        return false;
    }

    // timing gate
    if (timer_read32() < s->next_time)
        return true;

    key_action_t *a = &s->actions[s->index];

    if (a->press) {
        register_code(a->keycode);

        if (s->held_count < CONFIG_MAX_KEYS_HELD) {
            s->held_keys[s->held_count++] = a->keycode;
        }
    } else {
        unregister_code(a->keycode);

        for (uint8_t k = 0; k < s->held_count; k++) {
            if (s->held_keys[k] == a->keycode) {
                for (uint8_t m = k; m < s->held_count - 1; m++) {
                    s->held_keys[m] = s->held_keys[m + 1];
                }
                s->held_count--;
                break;
            }
        }
    }

    // compute delay BEFORE increment
    uint16_t delay = a->delay_ms;

    s->index++;

    s->next_time = timer_read32() + delay;

    return true;
}

// ----------------------------
// char -> actions
// ----------------------------

static uint8_t char_to_actions(char c, key_action_t *actions) {
    uint8_t count = 0;
    bool shift_needed = false;
    uint16_t keycode = KC_NO;

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

            default: return 0;
        }
    }

    // shift handling (local to sequence build)
    if (shift_needed) {
            actions[count++] = (key_action_t){KC_LSFT, true, CONFIG_RDP_DELAY_MOD};
    }

    actions[count++] = (key_action_t){keycode, true, CONFIG_RDP_DELAY_KEY};
    actions[count++] = (key_action_t){keycode, false, CONFIG_RDP_DELAY_KEY};

    if (shift_needed) {
        actions[count++] = (key_action_t){KC_LSFT, false, CONFIG_RDP_DELAY_MOD};
    }

    actions[count++] = (key_action_t){KC_NO, false, 10};

    return count;
}

// ----------------------------
// public API
// ----------------------------

void wait_for_queue_space(void) {
    while (queue_is_full()) {
        process_key_sequence();
    }
}

void rdp_send_string(const char *str) {

    while (*str) {
        key_action_t seq[CONFIG_MAX_SEQ_LEN];
        uint8_t seq_len = 0;

        // fill one chunk
        while (*str) {
            key_action_t temp[8];
            uint8_t count = char_to_actions(*str, temp);

            // stop BEFORE overflow
            if (seq_len + count >= CONFIG_MAX_SEQ_LEN) break;

            for (uint8_t i = 0; i < count; i++) {
                seq[seq_len++] = temp[i];
            }

            str++;
        }

        if (seq_len > 0) {
            wait_for_queue_space();   // 🔥 critical fix
            start_key_sequence(seq, seq_len);
        }
    }
}

void rdp_send_string_P(const char *str) {

    key_action_t seq[CONFIG_MAX_SEQ_LEN];
    uint8_t seq_len = 0;

    while (pgm_read_byte(str)) {

        key_action_t temp[8];
        uint8_t count = char_to_actions(pgm_read_byte(str), temp);

        // If adding this char would overflow → flush first
        if (seq_len + count >= CONFIG_MAX_SEQ_LEN) {

            start_key_sequence(seq, seq_len);
            seq_len = 0;

            // IMPORTANT: wait for queue space
            while (queue_is_full()) {
                process_key_sequence();
            }
        }

        for (uint8_t i = 0; i < count; i++) {
            seq[seq_len++] = temp[i];
        }

        str++;
    }

    // flush remaining
    if (seq_len > 0) {
        start_key_sequence(seq, seq_len);
    }
}

static uint8_t append_tab(key_action_t *actions) {
    uint8_t count = 0;

    actions[count++] = (key_action_t){KC_TAB, true, CONFIG_RDP_DELAY_KEY};
    actions[count++] = (key_action_t){KC_TAB, false, CONFIG_RDP_DELAY_KEY};

    // small pause after tab (important for RDP)
    actions[count++] = (key_action_t){KC_NO, false, 10};

    return count;
}

void rdp_send_fields_P(const char *a, const char *b, const char *c, const char *d) {

    const char *fields[] = {a, b, c, d};

    for (uint8_t f = 0; f < 4; f++) {

        const char *str = fields[f];

        while (pgm_read_byte(str)) {

            key_action_t seq[CONFIG_MAX_SEQ_LEN];
            uint8_t seq_len = 0;

            // build one chunk
            while (pgm_read_byte(str)) {

                key_action_t temp[8];
                uint8_t count = char_to_actions(pgm_read_byte(str), temp);

                if (seq_len + count >= CONFIG_MAX_SEQ_LEN) break;

                for (uint8_t i = 0; i < count; i++) {
                    seq[seq_len++] = temp[i];
                }

                str++;
            }

            if (seq_len > 0) {
                wait_for_queue_space();
                start_key_sequence(seq, seq_len);
            }
        }

        // append TAB between fields (as its own small sequence)
        if (f < 3) {
            key_action_t tab_seq[3];
            uint8_t tab_len = append_tab(tab_seq);

            wait_for_queue_space();
            start_key_sequence(tab_seq, tab_len);
        }
    }
}

