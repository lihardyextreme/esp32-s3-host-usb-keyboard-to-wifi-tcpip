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
  bool mapped = true;

  if (key >= HID_KEY_A && key <= HID_KEY_Z) {
    value = (shifted ? 'A' : 'a') + (key - HID_KEY_A);
  } else if (key >= HID_KEY_1 && key <= HID_KEY_9) {
    static const char normal[] = "123456789";
    static const char upper[] = "!@#$%^&*(";
    value = shifted ? upper[key - HID_KEY_1] : normal[key - HID_KEY_1];
  } else {
    switch (key) {
      case HID_KEY_0: value = shifted ? ')' : '0'; break;
      case HID_KEY_ENTER: value = '\r'; break;
      case HID_KEY_ESCAPE: value = 0x1b; break;
      case HID_KEY_BACKSPACE: value = '\b'; break;
      case HID_KEY_TAB: value = '\t'; break;
      case HID_KEY_SPACE: value = ' '; break;
      case HID_KEY_MINUS: value = shifted ? '_' : '-'; break;
      case HID_KEY_EQUAL: value = shifted ? '+' : '='; break;
      case HID_KEY_OPEN_BRACKET: value = shifted ? '{' : '['; break;
      case HID_KEY_CLOSE_BRACKET: value = shifted ? '}' : ']'; break;
      case HID_KEY_BACKSLASH: value = shifted ? '|' : '\\'; break;
      case HID_KEY_NONUS_HASH: value = shifted ? '|' : '\\'; break;
      case HID_KEY_SEMICOLON: value = shifted ? ':' : ';'; break;
      case HID_KEY_APOSTROPHE: value = shifted ? '"' : '\''; break;
      case HID_KEY_GRAVE: value = shifted ? '~' : '`'; break;
      case HID_KEY_COMMA: value = shifted ? '<' : ','; break;
      case HID_KEY_PERIOD: value = shifted ? '>' : '.'; break;
      case HID_KEY_SLASH: value = shifted ? '?' : '/'; break;
      case HID_KEY_KEYPAD_DECIMAL: value = '.'; break;
      default: mapped = false; break;
    }
  }

  if (!mapped) return KeyMapResult::NotMapped;
  return appendByte(value, output, capacity, written)
      ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
}
