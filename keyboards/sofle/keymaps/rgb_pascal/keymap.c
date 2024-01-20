
 /* Copyright 2021 Dane Evans, PP
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
  // SOFLE RGB
#include <stdio.h>

#include <string.h>     //memcpy
#include "lib/lib8tion/lib8tion.h"  // mouse smoothing

#include QMK_KEYBOARD_H
#include <rgblight.h>

#define INDICATOR_BRIGHTNESS 10
#define UNDERGLOW_BRIGHTNESS 100

#define HSV_OVERRIDE_HELP(h, s, v, Override) h, s , Override
#define HSV_OVERRIDE(hsv, Override) HSV_OVERRIDE_HELP(hsv,Override)

#define NUM_INDICATORS 1
#define NUM_UNDERGLOW 6
#define START_BACKLIGHT (NUM_INDICATORS + NUM_UNDERGLOW)
#define START_OUTER_FN_KEYS (START_BACKLIGHT + 1) // +1!
#define NUM_OUTER_FN_KEYS 4
#define START_INNER_KEYS (START_BACKLIGHT)

#define START_THUMB (START_BACKLIGHT + 19)		// why 19? Tja

#define NUM_BACKLIGHT 29
#define LEFTRIGHT_SPLIT (NUM_INDICATORS + NUM_UNDERGLOW + NUM_BACKLIGHT + 1)

// Light combinations
#define SET_NUMPAD(hsv)     \
	  {LEFTRIGHT_SPLIT+START_BACKLIGHT+9, 5, hsv},\
	  {LEFTRIGHT_SPLIT+START_BACKLIGHT+16, 3, hsv},\
	  {LEFTRIGHT_SPLIT+START_BACKLIGHT+21, 3, hsv}

#define SET_NUMSW(hsv)		\
      {LEFTRIGHT_SPLIT+START_OUTER_FN_KEYS, 1, hsv}

#define SET_NUMROW(hsv) \
	    {10, 2, hsv}, \
		{20, 2, hsv}, \
		{30, 2, hsv}, \
	  {LEFTRIGHT_SPLIT+ 10, 2, hsv}, \
	  {LEFTRIGHT_SPLIT+ 20, 2, hsv}, \
	  {LEFTRIGHT_SPLIT+ 30, 2, hsv}
#define SET_INNER_COL(hsv)	\
	{33, 4, hsv}, \
	  {LEFTRIGHT_SPLIT+ 33, NUM_OUTER_FN_KEYS, hsv}

#define SET_OUTER_COL(hsv) \
	{START_OUTER_FN_KEYS, NUM_OUTER_FN_KEYS, hsv}, \
	  {LEFTRIGHT_SPLIT + START_OUTER_FN_KEYS, NUM_OUTER_FN_KEYS, hsv}

#define SET_THUMB_CLUSTER(hsv) 	\
	{START_THUMB, 2, hsv}, \
	  {LEFTRIGHT_SPLIT+ START_THUMB, 2, hsv}

#define SET_LAYER_ID(hsv) 	\
	{0, NUM_INDICATORS, HSV_OVERRIDE_HELP(hsv,INDICATOR_BRIGHTNESS)}, \
    {LEFTRIGHT_SPLIT+0, NUM_INDICATORS, HSV_OVERRIDE_HELP(hsv,INDICATOR_BRIGHTNESS)}, \
		{NUM_INDICATORS, NUM_UNDERGLOW, HSV_OVERRIDE_HELP(hsv,UNDERGLOW_BRIGHTNESS)}, \
    {LEFTRIGHT_SPLIT+NUM_INDICATORS, NUM_UNDERGLOW, HSV_OVERRIDE_HELP(hsv,UNDERGLOW_BRIGHTNESS)}, \
	{START_OUTER_FN_KEYS, NUM_OUTER_FN_KEYS, hsv}, \
	  {LEFTRIGHT_SPLIT + START_OUTER_FN_KEYS, NUM_OUTER_FN_KEYS, hsv}, \
		{START_THUMB, 2, hsv}, \
	  {LEFTRIGHT_SPLIT+ START_THUMB, 2, hsv}

#define QUERTY_COLOR HSV_SPRINGGREEN
#define DVORAK_COLOR HSV_RORANGE
#define LOWER_COLOR HSV_TEAL
#define RAISE_COLOR HSV_YELLOW
#define ADJUST_COLOR HSV_PURPLE
#define NUMPAD_COLOR HSV_ORANGE
#define NUMPAD_DETAIL_COLOR HSV_BLUE
#define TRACKBALL_DEFAULT_COLOR RGB_ORANGE,0x20
#define TRACKBALL_SCROLLING_COLOR RGB_GREEN,0x50
#define TRACKBALL_RIGHTCLICK_COLOR RGB_BLUE,0xA0

enum sofle_layers {
    _DEFAULTS = 0,
    _QWERTY = 0,
    _DVORAK,
    _INTL,
    _LOWER,
    _RAISE,
    _ADJUST,
    _NUMPAD,
    _SWITCH
};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_DVORAK,
    KC_INTL,
    KC_LOWER,
    KC_RAISE,
    KC_ADJUST,
    KC_D_MUTE
};

static bool is_intl_layer = false;
static bool mouse_is_scrolling  = false;
#define DEFAULT_CPI 8000
#define SCROLLING_CPI 1500

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
#define RESET QK_REBOOT

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
LT(_NUMPAD,KC_ESC),KC_1,KC_2,KC_3,    KC_4,    KC_5,             LT(_SWITCH,KC_6), KC_7,   KC_8,    KC_9,    KC_0,    KC_GRV,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_LSFT,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,   KC_K,    KC_L,    KC_SCLN, KC_DEL,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  KC_LCTL,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_MUTE,    KC_MPLY, KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
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
LT(_NUMPAD,KC_ESC),KC_1,KC_2,KC_3,    KC_4,    KC_5,             LT(_SWITCH,KC_6),   KC_7,   KC_8,    KC_9,    KC_0,   KC_DEL,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
    KC_TAB, KC_QUOT, KC_COMM, KC_DOT, KC_P,    KC_Y,                        KC_F,    KC_G,   KC_C,    KC_R,    KC_SLSH,KC_BSPC,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_LSFT,  KC_A,   KC_O,    KC_E,    KC_U,    KC_I,                        KC_D,    KC_H,   KC_T,    KC_N,    KC_S,   KC_L,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  KC_LCTL,  KC_SCLN,KC_Q,    KC_J,    KC_K,    KC_X,  KC_MUTE,    XXXXXXX,  KC_B,    KC_M,   KC_W,    KC_V,    KC_Z,   KC_BSLS,
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
 * | ERASE|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | RESET|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |RGB_TOG|hue^ |sat ^ | bri ^|      |      | .------.  ,------. |desk <|      |      |desk >|      |      |
 * |------+------+------+------+------+------| | ____ |  | ____ | |------+------+------+------+------+------|
 * | mode |hue dn|sat d |bri dn|DVORAK|QWERTY| '------'  '------' |      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------'  ________  _______ '-----------------------------------------'
 *               | ____ | ____ | ____ |/ ____| /       /  \      \ ____ \| ____ | ____ | ____ |
 *               |      |      |      |      |/ ____  /    \ ____ \      \      |      |      |
 *               `-----------------------------------'     '------''------------'------'------'
 */
  [_ADJUST] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  RESET,   XXXXXXX,XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  RGB_TOG, RGB_HUI,RGB_SAI, RGB_VAI,  XXXXXXX, XXXXXXX,              C(G(KC_LEFT)),KC_NO,KC_NO,C(G(KC_RGHT)),  XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  RGB_MOD, RGB_HUD,RGB_SAD, RGB_VAD, KC_DVORAK,KC_QWERTY,_______, _______, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                   _______, _______, _______, _______, _______,     _______,  _______,  _______, _______, XXXXXXX
    //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/
),
/* NUMPAD
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ____ |      |      |      |      |      |                    |NumLck|      |      |      |      | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ |      |      |      |      |      |                    |   ^  |   7  |   8  |   9  |   *  | ____ |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ____ |      |      |      |      |      | .------.  ,------. |   -  |   4  |   5  |   6  |  /:  |   |  |
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
),

/* SWITCH
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | qwer | cole |col_dh| low  | raise| adj  |                    |numpad|      |      |      |      |RESET |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |EE_CLR|
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * | SLEEP|      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |      |      |      |      | /       /       \      \  |      |      |      |      |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  // layer switcher
[_SWITCH] = LAYOUT(
  //,------------------------------------------------.                    ,---------------------------------------------------.
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  TO(0),   TO(1),   TO(2),   TO(3),   TO(4),   TO(5),                      TO(6),   KC_NO,   KC_NO,   KC_NO,   KC_NO,   RESET,
  //|------+-------+--------+--------+--------+------|                   |--------+-------+--------+--------+--------+---------|
  KC_NO,   KC_NO, KC_BRIU,   KC_NO,   KC_NO,   KC_NO,                      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   EE_CLR,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
  KC_SYSTEM_SLEEP,KC_NO,KC_NO,KC_NO,  KC_NO,   KC_NO, KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  //|------+-------+--------+--------+--------+------|  ===  |   |  ===  |--------+-------+--------+--------+--------+---------|
                  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
  //            \--------+--------+--------+---------+-------|   |--------+---------+--------+---------+-------/

	),
};

#ifdef RGBLIGHT_ENABLE

#define HSV_RORANGE  10,255,255

char layer_state_str[70];
// Now define the array of layers. Later layers take precedence

// QWERTY,
// Light on inner column and underglow
const rgblight_segment_t PROGMEM layer_qwerty_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(QUERTY_COLOR)

);
const rgblight_segment_t PROGMEM layer_dvorak_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(DVORAK_COLOR)
);

// _NUM (LOWER)
const rgblight_segment_t PROGMEM layer_num_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(LOWER_COLOR),
	   // Arrow keys
	  {LEFTRIGHT_SPLIT+START_BACKLIGHT+12, 1, HSV_RED},\
	  {LEFTRIGHT_SPLIT+START_BACKLIGHT+16, 2, HSV_RED},\
	  {LEFTRIGHT_SPLIT+START_BACKLIGHT+22, 1, HSV_RED}
);
// _SYMBOL (RAISE)
const rgblight_segment_t PROGMEM layer_symbol_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(RAISE_COLOR),
	SET_NUMSW(NUMPAD_DETAIL_COLOR)
    );
// _COMMAND, (Adj)
const rgblight_segment_t PROGMEM layer_command_lights[] = RGBLIGHT_LAYER_SEGMENTS(
  SET_LAYER_ID(ADJUST_COLOR)
);

//_NUMPAD
const rgblight_segment_t PROGMEM layer_numpad_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(NUMPAD_COLOR),
	SET_NUMPAD(NUMPAD_DETAIL_COLOR),
	SET_NUMSW(RAISE_COLOR)
    );
// _SWITCHER   // light up top row
const rgblight_segment_t PROGMEM layer_switcher_lights[] = RGBLIGHT_LAYER_SEGMENTS(
	SET_LAYER_ID(HSV_GREEN),
	SET_NUMROW(HSV_GREEN)
);

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(

    layer_qwerty_lights,
	layer_dvorak_lights,
	layer_num_lights,        // overrides layer 1
	layer_symbol_lights,
    layer_command_lights,
	layer_numpad_lights,
	layer_switcher_lights
);

layer_state_t layer_state_set_user(layer_state_t state) {
    unsigned l = 0;

	rgblight_set_layer_state(l++, layer_state_cmp(state, _DEFAULTS) && layer_state_cmp(default_layer_state,_QWERTY));
	rgblight_set_layer_state(l++, layer_state_cmp(state, _DEFAULTS) && layer_state_cmp(default_layer_state,_DVORAK));

	rgblight_set_layer_state(l++, layer_state_cmp(state, _LOWER));
	rgblight_set_layer_state(l++, layer_state_cmp(state, _RAISE));
	rgblight_set_layer_state(l++, layer_state_cmp(state, _ADJUST));
	rgblight_set_layer_state(l++, layer_state_cmp(state, _NUMPAD));
	rgblight_set_layer_state(l++, layer_state_cmp(state, _SWITCH));

    return state;
}
void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;

	rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL);// haven't found a way to set this in a more useful way

}
#endif

#ifdef OLED_ENABLE

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
        case _SWITCH:
            oled_write_ln_P(PSTR(" Swit"), false);
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
    } else {
        //render_logo();    // De-soldered right OLED
    }
    return false;
}

#endif

/// MOUSE STUFF

void pointing_device_init(void) {
    i2c_init();
    pimoroni_trackball_set_rgbw(TRACKBALL_DEFAULT_COLOR);
    pimoroni_trackball_set_cpi(DEFAULT_CPI);
}

/**
void smooth_mouse_movement(report_mouse_t* mouse_report) {
    static report_mouse_t last_mouse_report   = {0};
    if (has_mouse_report_changed(&last_mouse_report, mouse_report)) {
        memcpy(&last_mouse_report, mouse_report, sizeof(report_mouse_t));
    }

    // Linear interpolation and ease-in-out
    static const fract8 fract = 0.5;

    mouse_report->x = ease8InOutApprox(lerp8by8(last_mouse_report.x, mouse_report->x, fract));
    mouse_report->y = ease8InOutApprox(lerp8by8(last_mouse_report.y, mouse_report->y, fract));
    mouse_report->h = ease8InOutApprox(lerp8by8(last_mouse_report.h, mouse_report->h, fract));
    mouse_report->v = ease8InOutApprox(lerp8by8(last_mouse_report.v, mouse_report->v, fract));
}
**/


#    if defined(POINTING_DEVICE_LEFT)
#        define POINTING_DEVICE_THIS_SIDE is_keyboard_left()
#    elif defined(POINTING_DEVICE_RIGHT)
#        define POINTING_DEVICE_THIS_SIDE !is_keyboard_left()
#    elif defined(POINTING_DEVICE_COMBINED)
#        define POINTING_DEVICE_THIS_SIDE true
#    endif

void trackball_set_rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
    if(POINTING_DEVICE_THIS_SIDE)
        pimoroni_trackball_set_rgbw(r, g, b ,w);
    // others currently not supported or i am too dumb to understand
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

    bool is_scrolling_layer = false;
    switch (get_highest_layer(layer_state)) {
        case _LOWER:
            is_scrolling_layer = true;
            break;
        default:
            break;
    }

    if(mouse_is_scrolling != is_scrolling_layer) {
        mouse_is_scrolling = is_scrolling_layer;
        if(mouse_is_scrolling) {
            trackball_set_rgbw(TRACKBALL_SCROLLING_COLOR);
            pointing_device_set_cpi(SCROLLING_CPI);
        } else {
            if(is_intl_layer) {
                trackball_set_rgbw(TRACKBALL_RIGHTCLICK_COLOR);
            } else {
                trackball_set_rgbw(TRACKBALL_DEFAULT_COLOR);
            }
            pointing_device_set_cpi(DEFAULT_CPI);
        }
    }

    if (mouse_is_scrolling) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    if(is_intl_layer) {
        mouse_report.buttons = (mouse_report.buttons & 1) << 1; // right-click
    }

    //smooth_mouse_movement(&mouse_report);

    return mouse_report;
}

//

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
                    case KC_SLSH:
                        tap_code16(KC_COLN); 		// for - when pressing / (for Numpad)
                        return false;
                    default:
                        break; // Don't modify
                }
            }
    }

    return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
		switch (get_highest_layer(layer_state)) {
		case _QWERTY:
		case _DVORAK:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        break;
		case _LOWER:
				if (clockwise) {
					tap_code(KC_MNXT);
				} else {
					tap_code(KC_MPRV);
				}
			break;
		default:
		    break;
        }
	}
    return true;
}

#endif

