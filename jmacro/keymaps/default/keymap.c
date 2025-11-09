#include QMK_KEYBOARD_H
#include "unicode.h"
#include "config.h"
#include "private.h"

// Function with delays Right Alt + KC
#define RALT_LETTER(kc) \
    do { \
        register_code(KC_RSFT);\
        wait_ms(8); \
        register_code(KC_RALT); \
        wait_ms(8); \
        register_code(kc); \
        wait_ms(8); \
        unregister_code(kc); \
        wait_ms(5); \
        unregister_code(KC_RALT); \
        wait_ms(5); \
        unregister_code(KC_RSFT);\
    } while (0)


enum custom_keycodes {
    JMAC_ALT_SHIFT_SPACE = SAFE_RANGE, // change keyboard input
    NEXT_LAYER,
    EMAIL1,
    EMAIL2,
    EMAIL3,
    TRIPLE0,
    // Zotero-Obsidian Integration
    JMAC_CTRL_SH_C,
    JMAC_CTRL_SH_R,
    JMAC_CTRL_SH_I,
    // Layer2 Poker 21 keys
    JMAC_POK_1,
    JMAC_POK_2,
    JMAC_POK_3,
    JMAC_POK_4,
    JMAC_POK_5,
    JMAC_POK_6,
    JMAC_POK_7,
    JMAC_POK_8,
    JMAC_POK_9,
    JMAC_POK_10,
    JMAC_POK_11,
    JMAC_POK_12,
    JMAC_POK_13,
    JMAC_POK_14,
    JMAC_POK_15,
    JMAC_POK_16,
    JMAC_POK_17,
    JMAC_POK_18,
    JMAC_POK_19,
    JMAC_POK_20,
    JMAC_POK_21,


};

enum layers {
    _BASE,
    _LAYER1,
    _LAYER2
};

// Layer switch key
#define LAYER_SW MO(_LAYER1)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        JMAC_ALT_SHIFT_SPACE, KC_CALC, KC_PSLS, KC_PAST, KC_PMNS,
        KC_KP_7, KC_KP_8, KC_KP_9, KC_PPLS, EMAIL1,
        KC_KP_4, KC_KP_5, KC_KP_6, EMAIL2, KC_KP_1,
        KC_KP_2, KC_KP_3, KC_PENT, EMAIL3, KC_KP_0,
        TRIPLE0, KC_KP_DOT, KC_AUDIO_MUTE, NEXT_LAYER, KC_PENT  // R5C4 is KC_0 here, will cycle layers
    ),

    [_LAYER1] = LAYOUT(
        _______, UC(0x21AA), UC(0x21A9), UC(0x00A7), UC(0x2727),
        UC(0x211A), UC(0x2713), UC(0x2717), UC(0x06DE), JMAC_CTRL_SH_C,
        UC(0x210D), UC(0x2126), UC(0x2118), JMAC_CTRL_SH_R, UC(0x203B),
        UC(0x2145), UC(0x2059), _______, JMAC_CTRL_SH_I, UC(0x203D),
        UC(0x2235), UC(0x2234), _______, _______, _______
    ),

    [_LAYER2] = LAYOUT(
        JMAC_POK_1, JMAC_POK_2, JMAC_POK_3, JMAC_POK_4, JMAC_POK_5,
        JMAC_POK_6, JMAC_POK_7, JMAC_POK_8, JMAC_POK_9, JMAC_POK_10,
        JMAC_POK_11, JMAC_POK_12, JMAC_POK_13, JMAC_POK_14, JMAC_POK_15,
        JMAC_POK_16, JMAC_POK_17, _______, JMAC_POK_18, JMAC_POK_19,
        JMAC_POK_20, JMAC_POK_21, _______, _______, _______
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    // Encoder 0: always volume up/down
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_AUDIO_VOL_UP);
        } else {
            tap_code(KC_AUDIO_VOL_DOWN);
        }
    }
    // Encoder 1: changes function by layer
    else if (index == 1) {
        switch (get_highest_layer(layer_state)) {
            case _BASE: // Layer 0 → mouse wheel up/down
                if (clockwise) {
                    tap_code(MS_WHLD);
                } else {
                    tap_code(MS_WHLU);
                }
                break;

            case _LAYER1: // Layer 1 → mouse wheel up/down
                if (clockwise) {
                    tap_code(MS_WHLD);
                } else {
                    tap_code(MS_WHLU);
                }
                break;

            case _LAYER2: // Layer 2 → same as layer 1
                if (clockwise) {
                    tap_code(KC_RGHT);
                } else {
                    tap_code(KC_LEFT);
                }
                break;

            default:
                // Fallback if you're on some other layer
                if (clockwise) {
                    tap_code(KC_RGHT);
                } else {
                    tap_code(KC_LEFT);
                }
                break;
        }
    }

    return false;
}


// RGB LED pins
#define RED_PIN   D6
#define GREEN_PIN D4
#define BLUE_PIN  D5

layer_state_t layer_state_set_user(layer_state_t state) {
    // Turn off all LEDs first
    writePinLow(RED_PIN);
    writePinLow(GREEN_PIN);
    writePinLow(BLUE_PIN);

    switch (get_highest_layer(state)) {
        case _BASE:
            writePinHigh(GREEN_PIN);  // Green for base layer
            break;
        case _LAYER1:
            writePinHigh(BLUE_PIN);   // Blue for layer 1
            break;
        case _LAYER2:
            writePinHigh(RED_PIN);    // Red for layer 2
            break;
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case NEXT_LAYER:
            if (record->event.pressed) {
                uint8_t next = (get_highest_layer(layer_state) + 1) % 3;
                layer_move(next);
            }
            return false;


        case JMAC_ALT_SHIFT_SPACE:
            if (record->event.pressed) {
                register_code(KC_LALT);
                register_code(KC_LSFT);
                register_code(KC_SPACE);
                unregister_code(KC_LALT);
                unregister_code(KC_LSFT);
                unregister_code(KC_SPACE);

            }
            return false;

        case EMAIL1:
            if (record->event.pressed) {
                SEND_STRING(EMAILJ);
            }
            return false;
        case EMAIL2:
            if (record->event.pressed) {
                SEND_STRING(EMAILS);
            }
            return false;
        case EMAIL3:
            if (record->event.pressed) {
                SEND_STRING(EMAILG);
            }
            return false;
        case TRIPLE0:
            if (record->event.pressed) {
                SEND_STRING("000");
            }
            return false;
            // Zotero-Obsidian Integration

        case JMAC_CTRL_SH_C:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_C);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                unregister_code(KC_C);

            }
            return false;
        case JMAC_CTRL_SH_R:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_R);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                unregister_code(KC_R);

            }
            return false;
        case JMAC_CTRL_SH_I:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_LSFT);
                register_code(KC_I);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
                unregister_code(KC_I);

            }
            return false;
// Poker macros (Right Alt + letter)
case JMAC_POK_1:  if (record->event.pressed) RALT_LETTER(KC_A); return false;
case JMAC_POK_2:  if (record->event.pressed) RALT_LETTER(KC_B); return false;
case JMAC_POK_3:  if (record->event.pressed) RALT_LETTER(KC_C); return false;
case JMAC_POK_4:  if (record->event.pressed) RALT_LETTER(KC_D); return false;
case JMAC_POK_5:  if (record->event.pressed) RALT_LETTER(KC_E); return false;
case JMAC_POK_6:  if (record->event.pressed) RALT_LETTER(KC_F); return false;
case JMAC_POK_7:  if (record->event.pressed) RALT_LETTER(KC_G); return false;
case JMAC_POK_8:  if (record->event.pressed) RALT_LETTER(KC_H); return false;
case JMAC_POK_9:  if (record->event.pressed) RALT_LETTER(KC_I); return false;
case JMAC_POK_10: if (record->event.pressed) RALT_LETTER(KC_J); return false;
case JMAC_POK_11: if (record->event.pressed) RALT_LETTER(KC_K); return false;
case JMAC_POK_12: if (record->event.pressed) RALT_LETTER(KC_L); return false;
case JMAC_POK_13: if (record->event.pressed) RALT_LETTER(KC_M); return false;
case JMAC_POK_14: if (record->event.pressed) RALT_LETTER(KC_N); return false;
case JMAC_POK_15: if (record->event.pressed) RALT_LETTER(KC_O); return false;
case JMAC_POK_16: if (record->event.pressed) RALT_LETTER(KC_P); return false;
case JMAC_POK_17: if (record->event.pressed) RALT_LETTER(KC_Q); return false;
case JMAC_POK_18: if (record->event.pressed) RALT_LETTER(KC_R); return false;
case JMAC_POK_19: if (record->event.pressed) RALT_LETTER(KC_S); return false;
case JMAC_POK_20: if (record->event.pressed) RALT_LETTER(KC_T); return false;
case JMAC_POK_21: if (record->event.pressed) RALT_LETTER(KC_U); return false;

    }
    return true;
}

void keyboard_post_init_user(void) {
    setPinOutput(RED_PIN);
    setPinOutput(GREEN_PIN);
    setPinOutput(BLUE_PIN);
}

