#ifndef __LAYOUT_XX__
#define __LAYOUT_XX__

#include <Arduino.h>
#include <hid_usage_keyboard.h>
#include <hid_host.h>

#include "layouts.h"

bool mapKeyToLayoutXX(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, uint8_t &countOfPressedKeys, const size_t pressedKeysMaxLength, uint8_t& newkey);
bool mapKeyToLayoutNumLock(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, uint8_t &countOfPressedKeys, const size_t pressedKeysMaxLength, uint8_t& newkey);

#endif
