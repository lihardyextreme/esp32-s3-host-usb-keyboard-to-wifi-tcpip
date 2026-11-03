#include "KeyboardLayout.h"

KeyboardLayout::~KeyboardLayout() = default;

bool KeyboardLayout::appendByte(
    uint8_t byte,
    uint8_t* output,
    size_t capacity,
    size_t& written)
{
  if (output == nullptr || written >= capacity) {
    return false;
  }

  output[written++] = byte;
  return true;
}

bool KeyboardLayout::appendBytes(
    const uint8_t* bytes,
    size_t length,
    uint8_t* output,
    size_t capacity,
    size_t& written)
{
  // Avoid underflow if the caller passes an invalid current offset.
  if (written > capacity || length > capacity - written ||
      (length > 0 && (bytes == nullptr || output == nullptr))) {
    return false;
  }

  // Check the whole sequence first so escape codes or UTF-8 aren't partial.
  for (size_t i = 0; i < length; ++i) {
    output[written++] = bytes[i];
  }

  return true;
}

KeyMapResult KeyboardLayout::mapKey(
    uint8_t key,
    const hid_keyboard_input_report_boot_t& report,
    uint8_t* output,
    size_t capacity,
    size_t& written) const
{
  written = 0;

  switch (key) {
    case HID_KEY_RIGHT:
    case HID_KEY_KEYPAD_6: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x43};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_LEFT:
    case HID_KEY_KEYPAD_4: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x44};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_DOWN:
    case HID_KEY_KEYPAD_2: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x42};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_UP:
    case HID_KEY_KEYPAD_8: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x41};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_KEYPAD_5: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x45};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_INSERT:
    case HID_KEY_KEYPAD_0: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x32, 0x7E};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_DELETE:
    case HID_KEY_KEYPAD_DELETE: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x33, 0x7E};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_HOME:
    case HID_KEY_KEYPAD_7: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x31, 0x7E};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_END:
    case HID_KEY_KEYPAD_1: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x34, 0x7E};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_PAGEUP:
    case HID_KEY_KEYPAD_9: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x35, 0x7E};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    case HID_KEY_PAGEDOWN:
    case HID_KEY_KEYPAD_3: {
        static const uint8_t bytes[] = {0x1B, 0x5B, 0x36, 0x7E};
        return appendBytes(bytes, sizeof(bytes), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
  }
  return KeyMapResult::NotMapped;
}
