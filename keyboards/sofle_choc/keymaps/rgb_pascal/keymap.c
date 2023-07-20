/* Copyright 2023 Brian Low
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

enum sofle_layers {
    _QWERTY = 0,
    _DEFAULTS = 1,
    _DVORAK = 1,
    _INTL,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NUMPAD
};

enum custom_keycodes {
    KC_QWERTY = QK_USER,
    KC_DVORAK,
    KC_INTL,
    KC_LOWER,
    KC_RAISE,
    KC_ADJUST,
    KC_D_MUTE
};

static bool is_intl_layer = false;

#define KC_REDO LCTL(LSFT(KC_Z))
#define KC_SAVE LCTL(KC_S)
#define KC_CLOSE LCTL(KC_Q)
#define KC_WNSFT LSFT(KC_LGUI)
#undef KC_UNDO
#define KC_UNDO LCTL(KC_Z)
#undef KC_CUT
#define KC_CUT LCTL(KC_X)
#undef KC_COPY
#define KC_COPY LCTL(KC_C)
#undef KC_PASTE
#define KC_PASTE LCTL(KC_V)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LShift|   A  |   S  |   D  |   F  |   G  | .------.  ,------. |   H  |   J  |   K  |   L  |   ;  | Del  |
 * |------+------+------+------+------+------| | MUTE |  |MOUSE | |------+------+------+------+------+------|
 * |LCtrl |   Z  |   X  |   C  |   V  |   B  | '------'  '------' |   N  |   M  |   ,  |   .  |   /  | Enter|
 * `-----------------------------------------'  ________  _______ '-----------------------------------------'
 *               | LGui | LAlt | INTL |/LOWER| /       /  \      \RAISE \| RCTR |LOWER | RGUI |
 *               |      |      |      |      |/ Space /    \Enter \      \      |      |      |
 *               `-----------------------------------'     '------''------------'------'------'
 */
  [_QWERTY] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
LT(_NUMPAD,KC_ESC),KC_1,KC_2,KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,   KC_8,    KC_9,    KC_0,    KC_GRV,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                       KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_LSFT,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                       KC_H,    KC_J,   KC_K,    KC_L,    KC_SCLN, KC_DEL,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  KC_LCTL, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_MUTE,    KC_MPLY, KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 KC_LGUI, KC_LALT, KC_INTL, KC_LOWER, KC_SPC,     KC_ENT, KC_RAISE, KC_RCTL,KC_LOWER, KC_SLSH
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
/*
 * Dvorak
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |  1!  |  2@  |  3#  |  4$  |  5%  |                    |  6^  |  7&  |  8*  |  9(  |  0)  | Del  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |  '"  |  ,<  |  .>  |   P  |   Y  |                    |   F  |   G  |   C  |   R  |  /?  | Bspc |
 * |------+------+------+------+------+------| <VOLUME>           |------+------+------+------+------+------|
 * |LShift|   A  |   O  |   E  |   U  |   I  | .------.  ,------. |   D  |   H  |   T  |   N  |   S  |   L  |
 * |------+------+------+------+------+------| | MUTE |  |MOUSE | |------+------+------+------+------+------|
 * | LCTR |  ;:  |   Q  |   J  |   K  |   X  | '------'  '------' |   B  |   M  |   W  |   V  |   Z  |  \|  |
 * `-----------------------------------------'  ________  _______ '-----------------------------------------'
 *               | LGui | LAlt | INTL |/LOWER| /       /  \      \RAISE \| RCTR |LOWER | Enter|
 *               |      |      |      |      |/ Enter /    \Space \      \      |      |      |
 *               `-----------------------------------'     '------''------------'------'------'
 */
  [_DVORAK] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
LT(_NUMPAD,KC_ESC),KC_1,KC_2,KC_3,    KC_4,    KC_5,                        KC_6,   KC_7,   KC_8,    KC_9,    KC_0,   KC_DEL,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
    KC_TAB, KC_QUOT, KC_COMM, KC_DOT, KC_P,    KC_Y,                        KC_F,   KC_G,   KC_C,    KC_R,    KC_SLSH,KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_LSFT,  KC_A,   KC_O,    KC_E,    KC_U,    KC_I,                        KC_D,   KC_H,   KC_T,    KC_N,    KC_S,   KC_L,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  KC_LCTL,  KC_SCLN,KC_Q,    KC_J,    KC_K,    KC_X,  KC_MUTE,    XXXXXXX,  KC_B,   KC_M,   KC_W,    KC_V,    KC_Z,   KC_BSLS,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 KC_LGUI, KC_LALT, KC_INTL, KC_LOWER, KC_ENT,     KC_SPC, KC_RAISE, KC_RCTL, KC_LOWER, KC_ENT
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),

/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ____ |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ |Scrnsh|WinSft| Close| Save |      |                    |      |      |  UP  |      | F11  | ____ |
 * |------+------+------+------+------+------| <NEXTPRV>          |------+------+------+------+------+------|
 * | ____ | Cut  | Copy | Paste| WH_U | PGUP | .------.  ,------. |      | LEFT | DOWN |RIGHT | F12  |      |
 * |------+------+------+------+------+------| |PLAYPS|  | SCRL | |------+------+------+------+------+------|
 * | ____ | Undo | Redo | Lshft| WH_D | PGDN | '------'  '------' |      | HOME |      | END  |      |      |
 * `-----------------------------------------'  ________  _______ '-----------------------------------------'
 *               | ____ | ____ | ____ |/ ____| /       /  \      \ ____ \| ____ | ____ | ____ |
 *               |      |      |      |      |/ ____ /    \ ____ \      \      |      |      |
 *               `-----------------------------------'     '------''------------'------'------'
 */
[_LOWER] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______,  KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,                      KC_F6,  KC_F7,   KC_F8,   KC_F9,   KC_F10, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______,  KC_PSCR,KC_WNSFT,KC_CLOSE,KC_SAVE, KC_NO,                      KC_NO,  KC_NO,   KC_UP,   KC_NO,   KC_F11, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______,  KC_CUT, KC_COPY, KC_PASTE,KC_WH_U, KC_PGUP,                    KC_NO,  KC_LEFT, KC_DOWN, KC_RGHT, KC_F12, KC_NO,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______,  KC_UNDO,KC_REDO, KC_LSFT, KC_WH_D, KC_PGDN,KC_MPLY,   _______, KC_NO,  KC_HOME, KC_NO,   KC_END , KC_NO,  KC_NO,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),

/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ____ | ____ | ____ | ____ | ____ | ____ |                    | ____ | ____ | ____ | ____ | ____ | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ | Ins  | Pscr | Menu |      |      |                    |   :  |   "  |   (  |   )  |   *  | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ | LAt  | LCtl |LShift|      | Caps | .------.  ,------. |   -  |   +  |   {  |   }  |   |  |   `  |
 * |------+------+------+------+------+------| | ____ |  | ____ | |------+------+------+------+------+------|
 * | ____ | Undo |  Cut | Copy | Paste|      | '------'  '------' |   _  |   =  |   [  |   ]  |   ~  | TOGG NUMP| 
 * `-----------------------------------------'  ________  _______ '-----------------------------------------'
 *               | ____ | ____ | ____ |/ ____| /       /  \      \ ____ \| ____ | ____ | ____ |
 *               |      |      |      |      |/ Space /    \Enter \      \      |      |      |
 *               `-----------------------------------'     '------''------------'------'------'
 */
[_RAISE] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, _______ , _______ , _______ , _______ , _______,               _______, _______,_______, _______, _______, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______,  KC_INS,  KC_PSCR, KC_APP, XXXXXXX, XXXXXXX,                   KC_COLN, KC_DQUO,KC_LPRN, KC_RPRN, KC_ASTR, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______, KC_LALT,  KC_LCTL, KC_LSFT,XXXXXXX, KC_CAPS,                   KC_MINS, KC_PLUS,KC_LCBR, KC_RCBR, KC_PIPE, KC_GRV,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______, KC_UNDO,  KC_CUT,  KC_COPY,KC_PASTE, XXXXXXX,_______,  _______,KC_UNDS, KC_EQL, KC_LBRC, KC_RBRC, KC_TILD, TG(_NUMPAD), 
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                 _______, _______, _______, _______, KC_SPC,     KC_ENT , _______, _______, _______, _______
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | RESET| Spd+ | Val+ | Sat+ | Hue+ |Mode+ |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Spd- | Val- | Sat- | Hue- |Mode- |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * | mode |      |      |      |DVORAK|QWERTY|-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------'  ________  _______ '----------------------------------------'
 *               | ____ | ____ | ____ |/ ____| /       /  \      \ ____ \| ____ | ____ | ____ |
 *               |      |      |      |      |/ ____  /    \ ____ \      \      |      |      |
 *               `-----------------------------------'     '------''------------'------'------'
 */
[_ADJUST] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    QK_BOOT, RGB_SPI, RGB_VAI, RGB_SAI, RGB_HUI, RGB_MODE_FORWARD,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, RGB_SPD, RGB_VAD, RGB_SAD, RGB_HUD, RGB_MODE_REVERSE,                  XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
    CG_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, KC_DVORAK,KC_QWERTY,      _______, _______, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
),
/* NUMPAD
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ____ |      |      |      |      |      |                    |NumLck|      |      |      |      | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ |      |      |      |      |      |                    |   ^  |   7  |   8  |   9  |   *  | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ |      |      |      |      |      | .------.  ,------. |   -  |   4  |   5  |   6  |   /  |   |  |
 * |------+------+------+------+------+------| | ____ |  | ____ | |------+------+------+------+------+------|
 * | ____ |      |      |      |      |      | '------'  '------' |   +  |   1  |   2  |   3  |   =  | TOGG NUMP|
 * `-----------------------------------------'  ________  _______ '-----------------------------------------'
 *               | ____ | ???? | ____ |/____| /       /  \       \ ____ \|   0  |  .,  | ENTER |
 *               |      |      |      |      |/ ____ /    \ ____  \      \      |      |       |
 *               `-----------------------------------'     '------''------------'------'-------'
 */
[_NUMPAD] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_NUM,  XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_CIRC, KC_P7,  KC_P8,   KC_P9,   KC_ASTR, _______,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   KC_MINS, KC_P4,  KC_P5,   KC_P6,   KC_SLSH, KC_PIPE,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,_______,   _______,KC_PLUS, KC_P1,  KC_P2,   KC_P3,   KC_EQL,  TG(_NUMPAD),
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
              _______, OSM(MOD_MEH), _______, _______, _______,   _______, _______,  KC_P0,   KC_PDOT,   KC_ENT
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_DVORAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DVORAK);
            }
            return false;
        case KC_LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case KC_RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case KC_ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
        case KC_INTL:
            is_intl_layer = record->event.pressed;
            return false;

        default:
            if(is_intl_layer && record->event.pressed) {    // Would be nicer with lambda that determines press/unpress
                switch (keycode) {
                    case KC_A:
                        tap_code16(RALT(KC_Q)); // for ä at position a
                        return false;
                    case KC_S:
                        tap_code16(RALT(KC_S)); // for ß at position s
                        return false;
                    case KC_U:
                        tap_code16(RALT(KC_Y)); // for ü at position u
                        return false;
                    case KC_O:
                        tap_code16(RALT(KC_P)); // for ö at position o
                        return false;
                    case KC_E:
                        tap_code16(RALT(KC_5)); // for € at position e
                        return false;
                    case KC_PDOT:
                        tap_code(KC_COMM); 		// for , when pressing . (for Numpad)
                        return false;
                    default:
                        break; // Don't modify
                }
            }
    }

    return true;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),   ENCODER_CCW_CW(KC_MPRV, KC_MNXT)  },
    [_DVORAK] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),   ENCODER_CCW_CW(KC_MPRV, KC_MNXT)  },
    [_LOWER]  = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),   ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },
    [_RAISE]  = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),   ENCODER_CCW_CW(RGB_SPD, RGB_SPI)  },
    [_ADJUST] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),  ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
    [_NUMPAD] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),  ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
};
#endif

#ifdef OLED_ENABLE

void write_int_ln(const char* prefix, uint8_t value) {
    oled_write_P(prefix, false);
    oled_write(get_u8_str(value, ' '), false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n\n"), false);

    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Qwrt "), false);
            break;
        case _DVORAK:
            oled_write_P(PSTR("DVORK"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
        case _DVORAK:
            oled_write_ln_P(PSTR(" Base"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("  Adj"), false);
            break;
        case _NUMPAD:
            oled_write_ln_P(PSTR(" Nump"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    
    if(is_intl_layer) {
        oled_write_ln_P(PSTR("Intl"), false);
    } else {
        oled_write_ln_P(PSTR(" "), false);
    }
    
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    }

    return false;
}

#endif
