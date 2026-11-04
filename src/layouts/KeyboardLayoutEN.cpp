#include <hid_usage_keyboard.h>
#include "KeyboardLayoutEN.h"

const char* EnglishKeyboardLayout::name() const
{
  return "English (US)";
}

KeyMapResult EnglishKeyboardLayout::mapKey(
    uint8_t key,
    const hid_keyboard_input_report_boot_t& report,
    uint8_t* output,
    size_t capacity,
    size_t& written) const
{
  written = 0;
  const bool shifted = report.modifier.left_shift || report.modifier.right_shift;
  uint8_t value = 0;

  if (key >= HID_KEY_A && key <= HID_KEY_Z) {
    value = (shifted ? 'A' : 'a') + (key - HID_KEY_A);
  } else if (key >= HID_KEY_1 && key <= HID_KEY_9) {
    static const char normal[] = "123456789";
    static const char upper[] = "!@#$%^&*(";
    value = shifted ? upper[key - HID_KEY_1] : normal[key - HID_KEY_1];
  } else {
    switch (key) {
      case HID_KEY_0: value = shifted ? ')' : '0'; break;
      case HID_KEY_MINUS: value = shifted ? '_' : '-'; break;
      case HID_KEY_EQUAL: value = shifted ? '+' : '='; break;
      case HID_KEY_OPEN_BRACKET: value = shifted ? '{' : '['; break;
      case HID_KEY_CLOSE_BRACKET: value = shifted ? '}' : ']'; break;
      case HID_KEY_SHARP: value = shifted ? '|' : '\\'; break;
      case HID_KEY_COLON: value = shifted ? ':' : ';'; break;
      case HID_KEY_QUOTE: value = shifted ? '"' : '\''; break;
      case HID_KEY_TILDE: value = shifted ? '~' : '`'; break;
      case HID_KEY_LESS: value = shifted ? '<' : ','; break;
      case HID_KEY_GREATER: value = shifted ? '>' : '.'; break;
      case HID_KEY_SLASH: value = shifted ? '?' : '/'; break;
      case HID_KEY_KEYPAD_DECIMAL: value = '.'; break;
      default:
        return KeyboardLayout::mapKey(key, report, output, capacity, written);
    }
  }
  return appendByte(value, output, capacity, written)
      ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
}
