#include "KeyboardLayoutDE.h"

const char* GermanKeyboardLayout::name() const
{
  return "German (DE)";
}

KeyMapResult GermanKeyboardLayout::mapKey(
    uint8_t key,
    const hid_keyboard_input_report_boot_t& report,
    uint8_t* output,
    size_t capacity,
    size_t& written) const
{
  written = 0;
  const bool shifted = report.modifier.left_shift || report.modifier.right_shift;
  const bool altGr = report.modifier.right_alt;
  uint8_t value = 0;
  bool mapped = true;

  if (key >= HID_KEY_A && key <= HID_KEY_Z) {
    value = (shifted ? 'A' : 'a') + (key - HID_KEY_A);
    if (key == HID_KEY_Y) value = shifted ? 'Z' : 'z';
    if (key == HID_KEY_Z) value = shifted ? 'Y' : 'y';
  } else if (key >= HID_KEY_1 && key <= HID_KEY_9) {
    static const char normal[] = "123456789";
    static const char upper[] = "!\"§$%&/ (";
    if (key == HID_KEY_3 && shifted) {
      static const uint8_t section[] = {0xC2, 0xA7};
      return appendBytes(section, sizeof(section), output, capacity, written)
          ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
    }
    if (key == HID_KEY_7 && altGr) value = '{';
    else if (key == HID_KEY_8 && altGr) value = '[';
    else if (key == HID_KEY_9 && altGr) value = ']';
    else value = shifted ? upper[key - HID_KEY_1] : normal[key - HID_KEY_1];
    // Correct German shifted number-row values for 7, 8 and 9.
    if (shifted && key == HID_KEY_7) value = '/';
    if (shifted && key == HID_KEY_8) value = '(';
    if (shifted && key == HID_KEY_9) value = ')';
  } else {
    switch (key) {
      case HID_KEY_0:
        if (altGr) value = '}';
        else value = shifted ? '=' : '0';
        break;
      case HID_KEY_ENTER: value = '\r'; break;
      case HID_KEY_ESCAPE: value = 0x1b; break;
      case HID_KEY_BACKSPACE: value = '\b'; break;
      case HID_KEY_TAB: value = '\t'; break;
      case HID_KEY_SPACE: value = ' '; break;
      case HID_KEY_MINUS:
        if (shifted) value = '?';
        else if (altGr) value = '\\';
        else {
          static const uint8_t sharpS[] = {0xC3, 0x9F};
          return appendBytes(sharpS, sizeof(sharpS), output, capacity, written)
              ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
        }
        break;
      case HID_KEY_EQUAL:
        if (shifted) value = '`';
        else {
          static const uint8_t acute[] = {0xC2, 0xB4};
          return appendBytes(acute, sizeof(acute), output, capacity, written)
              ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
        }
        break;
      case HID_KEY_OPEN_BRACKET: {
        const uint8_t umlaut[] = {0xC3, static_cast<uint8_t>(shifted ? 0x9C : 0xBC)};
        return appendBytes(umlaut, sizeof(umlaut), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
      }
      case HID_KEY_CLOSE_BRACKET: value = shifted ? '*' : '+'; break;
      case HID_KEY_BACKSLASH: value = shifted ? '\\' : '#'; break;
      case HID_KEY_NONUS_HASH: value = shifted ? '\'' : '<'; break;
      case HID_KEY_SEMICOLON: {
        const uint8_t umlaut[] = {0xC3, static_cast<uint8_t>(shifted ? 0x96 : 0xB6)};
        return appendBytes(umlaut, sizeof(umlaut), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
      }
      case HID_KEY_APOSTROPHE: {
        const uint8_t umlaut[] = {0xC3, static_cast<uint8_t>(shifted ? 0x84 : 0xA4)};
        return appendBytes(umlaut, sizeof(umlaut), output, capacity, written)
            ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
      }
      case HID_KEY_GRAVE:
        if (shifted) {
          static const uint8_t degree[] = {0xC2, 0xB0};
          return appendBytes(degree, sizeof(degree), output, capacity, written)
              ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
        }
        value = '^';
        break;
      case HID_KEY_COMMA: value = shifted ? ';' : ','; break;
      case HID_KEY_PERIOD: value = shifted ? ':' : '.'; break;
      case HID_KEY_SLASH: value = shifted ? '_' : '-'; break;
      case HID_KEY_KEYPAD_DECIMAL: value = ','; break;
      default: mapped = false; break;
    }
  }

  if (!mapped) return KeyMapResult::NotMapped;
  return appendByte(value, output, capacity, written)
      ? KeyMapResult::Mapped : KeyMapResult::BufferTooSmall;
}
