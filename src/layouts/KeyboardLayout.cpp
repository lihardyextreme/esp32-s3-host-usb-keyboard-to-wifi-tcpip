#include "KeyboardLayout.h"

KeyboardLayout::~KeyboardLayout() = default;

bool KeyboardLayout::appendByte(String &buffer, uint8_t byte)
{
  buffer+=(char)byte;
  return true;
}

bool KeyboardLayout::appendBytes(String &buffer, const uint8_t *bytes, size_t length)
{
  for (size_t i = 0; i < length; ++i)
    buffer+=(char)bytes[i];
  return true;
}

bool KeyboardLayout::mapKey(
    uint8_t key,
    const hid_keyboard_input_report_boot_t &report,
    String &buffer) const
{
  switch (key)
  {
  case HID_KEY_RIGHT:
  case HID_KEY_KEYPAD_6:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x43};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_LEFT:
  case HID_KEY_KEYPAD_4:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x44};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_DOWN:
  case HID_KEY_KEYPAD_2:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x42};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_UP:
  case HID_KEY_KEYPAD_8:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x41};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_KEYPAD_5:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x45};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_INSERT:
  case HID_KEY_KEYPAD_0:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_DELETE:
  case HID_KEY_KEYPAD_DELETE:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x33, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_HOME:
  case HID_KEY_KEYPAD_7:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_END:
  case HID_KEY_KEYPAD_1:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x34, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_PAGEUP:
  case HID_KEY_KEYPAD_9:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x35, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  case HID_KEY_PAGEDOWN:
  case HID_KEY_KEYPAD_3:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x36, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  }
  return false;
}

bool KeyboardLayout::mapKeyFx(uint8_t &key, hid_keyboard_input_report_boot_t *input, String &buffer) const
{
  if (key == HID_KEY_F1) // F1: 0x1B 0x4F 0x50
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x50};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F2) // F2: 0x1B 0x4F 0x51
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x51};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F3) // F3: 0x1B 0x4F 0x52
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x52};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F4) // F4: 0x1B 0x4F 0x53
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x53};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F5) // F5: 0x1B 0x5B 0x31 0x35 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x35, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F6) // F6: 0x1B 0x5B 0x31 0x37 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x37, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F7) // F7: 0x1B 0x5B 0x31 0x38 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x38, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F8) // F8: 0x1B 0x5B 0x31 0x39 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x39, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F9) // F9: 0x1B 0x5B 0x32 0x30 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x30, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F10) // F10: 0x1B 0x5B 0x32 0x31 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x31, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F11) // F11: 0x1B 0x5B 0x32 0x33 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x32, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  if (key == HID_KEY_F12) // F12: 0x1B 0x5B 0x32 0x34 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x34, 0x7E};
    return appendBytes(buffer, bytes, sizeof(bytes));
  }
  return false;
}

bool KeyboardLayout::premapKey(bool stateNumLock, uint8_t &key, hid_keyboard_input_report_boot_t *input, String &buffer) const
{
  if (mapKeyFx(key, input, buffer))
    return true;
  if (key == HID_KEY_ENTER || key == HID_KEY_KEYPAD_ENTER)
    return appendByte(buffer, '\r');
  if (key == HID_KEY_TAB)
    return appendByte(buffer, '\t');
  if (key == HID_KEY_SPACE)
    return appendByte(buffer, ' ');
  if (key == HID_KEY_ESC)
    return appendByte(buffer, 0x1B);
  if (key == HID_KEY_DEL) // backspace
    return appendByte(buffer, '\b');
  if (stateNumLock)
  {
    if (key == HID_KEY_KEYPAD_0)
    {
      key = HID_KEY_0;
      return false;
    }
    if (key == HID_KEY_KEYPAD_1)
    {
      key = HID_KEY_1;
      return false;
    }
    if (key == HID_KEY_KEYPAD_2)
    {
      key = HID_KEY_2;
      return false;
    }
    if (key == HID_KEY_KEYPAD_3)
    {
      key = HID_KEY_3;
      return false;
    }
    if (key == HID_KEY_KEYPAD_4)
    {
      key = HID_KEY_4;
      return false;
    }
    if (key == HID_KEY_KEYPAD_5)
    {
      key = HID_KEY_5;
      return false;
    }
    if (key == HID_KEY_KEYPAD_6)
    {
      key = HID_KEY_6;
      return false;
    }
    if (key == HID_KEY_KEYPAD_7)
    {
      key = HID_KEY_7;
      return false;
    }
    if (key == HID_KEY_KEYPAD_8)
    {
      key = HID_KEY_8;
      return false;
    }
    if (key == HID_KEY_KEYPAD_9)
    {
      key = HID_KEY_9;
      return false;
    }
    if (key == HID_KEY_KEYPAD_DELETE)
    {
      key = HID_KEY_KEYPAD_DECIMAL;
      return false;
    }
  }
  if (key == HID_KEY_KEYPAD_ENTER)
  {
    key = HID_KEY_ENTER;
    return false;
  }
  if (key == HID_KEY_KEYPAD_DELETE)
  {
    key = HID_KEY_DELETE;
    return false;
  }
  if (key == HID_KEY_KEYPAD_DIV)
    return appendByte(buffer, '/');
  if (key == HID_KEY_KEYPAD_MUL)
    return appendByte(buffer, '*');
  if (key == HID_KEY_KEYPAD_SUB)
    return appendByte(buffer, '-');
  if (key == HID_KEY_KEYPAD_ADD)
    return appendByte(buffer, '+');
  return false;
}
