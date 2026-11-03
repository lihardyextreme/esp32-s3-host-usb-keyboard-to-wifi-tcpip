#ifndef LAYOUTS_H
#define LAYOUTS_H

#include <Arduino.h>
#include <hid_usage_keyboard.h>

#define LAYOUT_EN 0
#define LAYOUT_DE 1

#include "KeyboardLayout.h"
#include "KeyboardLayoutEN.h"
#include "KeyboardLayoutDE.h"

#define KEYBOARD_ENTER_MAIN_CHAR '\r'

bool mapKeyToLayoutXX(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength, uint8_t& newkey);
bool mapKeyToLayoutNumLock(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength, uint8_t& newkey);

const KeyboardLayout* getKeyboardLayout(int layout);

#endif
