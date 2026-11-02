#ifndef __LAYOUTS_XX__
#define __LAYOUTS_XX__

#include <Arduino.h>
#include <hid_usage_keyboard.h>

#include "KeyboardLayout.h"
#include "KeyboardLayoutEN.h"
#include "KeyboardLayoutDE.h"

#define KEYBOARD_ENTER_MAIN_CHAR '\r'
typedef uint8_t TLayout[200][3];

#include "layout_xx.h"
#include "layout_en.h"
#include "layout_de.h"

const TLayout* getLayoutMap(int layout);
const KeyboardLayout* getKeyboardLayout(int layout);

#endif
