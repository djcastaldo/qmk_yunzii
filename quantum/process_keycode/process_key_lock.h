/* Copyright 2017 Fredric Silberberg
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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"

void cancel_key_lock(void);
bool process_key_lock(uint16_t *keycode, keyrecord_t *record);
void set_key_lock_watching(void);
bool is_key_lock_watching(void);
bool is_key_locked;
bool lctl_locked;
bool lalt_locked;
bool lgui_locked;
bool lsft_locked;
bool rsft_locked;
bool rctl_locked;
