#include "layouts.h"

const KeyboardLayout* getKeyboardLayout(int layout)
{
  static const EnglishKeyboardLayout english;
  static const GermanKeyboardLayout german;

  if (layout == LAYOUT_EN) return &english;
  if (layout == LAYOUT_DE) return &german;
  return nullptr;
}

bool mapKeyToLayoutNumLock(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength, uint8_t& newkey)
{
  if (key == HID_KEY_KEYPAD_0)
  {
    newkey = HID_KEY_0;
    return false;
  }
  if (key == HID_KEY_KEYPAD_1)
  {
    newkey = HID_KEY_1;
    return false;
  }
  if (key == HID_KEY_KEYPAD_2)
  {
    newkey = HID_KEY_2;
    return false;
  }
  if (key == HID_KEY_KEYPAD_3)
  {
    newkey = HID_KEY_3;
    return false;
  }
  if (key == HID_KEY_KEYPAD_4)
  {
    newkey = HID_KEY_4;
    return false;
  }
  if (key == HID_KEY_KEYPAD_5)
  {
    newkey = HID_KEY_5;
    return false;
  }
  if (key == HID_KEY_KEYPAD_6)
  {
    newkey = HID_KEY_6;
    return false;
  }
  if (key == HID_KEY_KEYPAD_7)
  {
    newkey = HID_KEY_7;
    return false;
  }
  if (key == HID_KEY_KEYPAD_8)
  {
    newkey = HID_KEY_8;
    return false;
  }
  if (key == HID_KEY_KEYPAD_9)
  {
    newkey = HID_KEY_9;
    return false;
  }
  if (key == HID_KEY_KEYPAD_ENTER)
  {
    newkey = HID_KEY_ENTER;
    return false;
  }
  if (key == HID_KEY_KEYPAD_DELETE)
  {
    newkey = HID_KEY_KEYPAD_DECIMAL;
    return false;
  }
  if (key == HID_KEY_KEYPAD_DIV)
  {
    pressedKeys[countOfPressedKeys++] = '/';
    return true;
  }
  if (key == HID_KEY_KEYPAD_MUL)
  {
    pressedKeys[countOfPressedKeys++] = '*';
    return true;
  }
  if (key == HID_KEY_KEYPAD_SUB)
  {
    pressedKeys[countOfPressedKeys++] = '-';
    return true;
  }
  if (key == HID_KEY_KEYPAD_ADD)
  {
    pressedKeys[countOfPressedKeys++] = '+';
    return true;
  }
  return false;
}

bool mapKeyToLayoutXX(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength, uint8_t& newkey)
{
  if (key == HID_KEY_F1) // F1: 0x1B 0x4F 0x50
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x50;
    return true;
  }
  if (key == HID_KEY_F2) // F2: 0x1B 0x4F 0x51
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x51;
    return true;
  }
  if (key == HID_KEY_F3) // F3: 0x1B 0x4F 0x52
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x52;
    return true;
  }
  if (key == HID_KEY_F4) // F4: 0x1B 0x4F 0x53
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x53;
    return true;
  }
  if (key == HID_KEY_F5) // F5: 0x1B 0x5B 0x31 0x35 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x35;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F6) // F6: 0x1B 0x5B 0x31 0x37 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x37;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F7) // F7: 0x1B 0x5B 0x31 0x38 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x38;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F8) // F8: 0x1B 0x5B 0x31 0x39 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x39;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F9) // F9: 0x1B 0x5B 0x32 0x30 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x30;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F10) // F10: 0x1B 0x5B 0x32 0x31 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F11) // F11: 0x1B 0x5B 0x32 0x33 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x33;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F12) // F12: 0x1B 0x5B 0x32 0x34 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x34;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  return false;
}
