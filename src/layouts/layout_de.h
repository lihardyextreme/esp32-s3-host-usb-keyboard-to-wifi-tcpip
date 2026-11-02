#ifndef __LAYOUT_DE_XX__
#define __LAYOUT_DE_XX__

#define LAYOUT_DE 1

#include <Arduino.h>

#include "layouts.h"

size_t mapKeyToLayoutDE(uint8_t key, hid_keyboard_input_report_boot_t* input, char* pressedKeys, uint8_t & countOfPressedKeys, const size_t pressedKeysMaxLength);
bool mapSpecialKeyDE(uint8_t& key);
const TLayout* getLayoutMapDE();

#endif
