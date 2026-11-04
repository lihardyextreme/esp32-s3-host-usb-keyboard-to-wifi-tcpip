#include "KeyboardLayout.h"

KeyboardLayout::~KeyboardLayout() = default;

bool KeyboardLayout::appendByte(
    uint8_t byte,
    uint8_t *output,
    size_t capacity,
    size_t &written)
{
  if (output == nullptr || written >= capacity)
  {
    return false;
  }

  output[written++] = byte;
  return true;
}

bool KeyboardLayout::appendBytes(
    const uint8_t *bytes,
    size_t length,
    uint8_t *output,
    size_t capacity,
    size_t &written)
{
  // Avoid underflow if the caller passes an invalid current offset.
  if (written > capacity || length > capacity - written ||
      (length > 0 && (bytes == nullptr || output == nullptr)))
  {
    return false;
  }

  // Check the whole sequence first so escape codes or UTF-8 aren't partial.
  for (size_t i = 0; i < length; ++i)
  {
    output[written++] = bytes[i];
  }

  return true;
}

KeyMapResult KeyboardLayout::mapKey(
    uint8_t key,
    const hid_keyboard_input_report_boot_t &report,
    uint8_t *output,
    size_t capacity,
    size_t &written) const
{
  written = 0;

  switch (key)
  {
  case HID_KEY_RIGHT:
  case HID_KEY_KEYPAD_6:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x43};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_LEFT:
  case HID_KEY_KEYPAD_4:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x44};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_DOWN:
  case HID_KEY_KEYPAD_2:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x42};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_UP:
  case HID_KEY_KEYPAD_8:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x41};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_KEYPAD_5:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x45};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_INSERT:
  case HID_KEY_KEYPAD_0:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x7E};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_DELETE:
  case HID_KEY_KEYPAD_DELETE:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x33, 0x7E};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_HOME:
  case HID_KEY_KEYPAD_7:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x7E};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_END:
  case HID_KEY_KEYPAD_1:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x34, 0x7E};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_PAGEUP:
  case HID_KEY_KEYPAD_9:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x35, 0x7E};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  case HID_KEY_PAGEDOWN:
  case HID_KEY_KEYPAD_3:
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x36, 0x7E};
    return appendBytes(bytes, sizeof(bytes), output, capacity, written)
               ? KeyMapResult::Mapped
               : KeyMapResult::BufferTooSmall;
  }
  }
  return KeyMapResult::NotMapped;
}

bool KeyboardLayout::mapKeyFx(uint8_t &key, hid_keyboard_input_report_boot_t *input, uint8_t *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength) const
{
  if (key == HID_KEY_F1) // F1: 0x1B 0x4F 0x50
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x50};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F2) // F2: 0x1B 0x4F 0x51
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x51};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F3) // F3: 0x1B 0x4F 0x52
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x52};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F4) // F4: 0x1B 0x4F 0x53
  {
    static const uint8_t bytes[] = {0x1B, 0x4F, 0x53};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F5) // F5: 0x1B 0x5B 0x31 0x35 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x35, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F6) // F6: 0x1B 0x5B 0x31 0x37 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x37, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F7) // F7: 0x1B 0x5B 0x31 0x38 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x38, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F8) // F8: 0x1B 0x5B 0x31 0x39 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x39, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F9) // F9: 0x1B 0x5B 0x32 0x30 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x30, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F10) // F10: 0x1B 0x5B 0x32 0x31 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x31, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F11) // F11: 0x1B 0x5B 0x32 0x33 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x32, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_F12) // F12: 0x1B 0x5B 0x32 0x34 0x7E
  {
    static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x34, 0x7E};
    appendBytes(bytes, sizeof(bytes), pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  return false;
}

bool KeyboardLayout::premapKey(bool stateNumLock, uint8_t &key, hid_keyboard_input_report_boot_t *input, uint8_t *pressedKeys, size_t &countOfPressedKeys, size_t pressedKeysMaxLength) const
{
  if (mapKeyFx(key, input, pressedKeys, countOfPressedKeys, pressedKeysMaxLength))
    return true;
  if (key == HID_KEY_ENTER || key == HID_KEY_KEYPAD_ENTER)
  {
    appendByte('\r', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_TAB)
  {
    appendByte('\t', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_SPACE)
  {
    appendByte(' ', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_ESC)
  {
    appendByte(0x1b, pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_DEL)
  { // backspace
    appendByte('\b', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
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
  {
    appendByte('/', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_KEYPAD_MUL)
  {
    appendByte('*', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_KEYPAD_SUB)
  {
    appendByte('-', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  if (key == HID_KEY_KEYPAD_ADD)
  {
    appendByte('+', pressedKeys, pressedKeysMaxLength, countOfPressedKeys);
    return true;
  }
  return false;
}
