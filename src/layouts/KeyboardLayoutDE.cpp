#include <hid_usage_keyboard.h>
#include "KeyboardLayoutDE.h"

const char *GermanKeyboardLayout::name() const
{
  return "German (DE)";
}

bool GermanKeyboardLayout::mapKey(
    uint8_t key,
    const hid_keyboard_input_report_boot_t &report,
    String &buffer) const
{
  const bool shifted = report.modifier.left_shift || report.modifier.right_shift;
  const bool altGr = report.modifier.right_alt;
  uint8_t value = 0;

  if (key >= HID_KEY_A && key <= HID_KEY_Z)
  {
    value = (shifted ? 'A' : 'a') + (key - HID_KEY_A);
    if (key == HID_KEY_Y)
      value = shifted ? 'Z' : 'z';
    if (key == HID_KEY_Z)
      value = shifted ? 'Y' : 'y';
  }
  else if (key >= HID_KEY_1 && key <= HID_KEY_9)
  {
    static const char normal[] = "123456789";
    static const char upper[] = "!\"X$%&/()";
    if (key == HID_KEY_3 && shifted)
    {
      static const uint8_t section[] = {0xC2, 0xA7};
      return appendBytes(buffer, section, sizeof(section));
    }
    if (key == HID_KEY_7 && altGr)
      value = '{';
    else if (key == HID_KEY_8 && altGr)
      value = '[';
    else if (key == HID_KEY_9 && altGr)
      value = ']';
    else
      value = shifted ? upper[key - HID_KEY_1] : normal[key - HID_KEY_1];
  }
  else
  {
    switch (key)
    {
    case HID_KEY_0:
      if (altGr)
        value = '}';
      else
        value = shifted ? '=' : '0';
      break;
    case HID_KEY_MINUS:
      if (shifted)
        value = '?';
      else if (altGr)
        value = '\\';
      else
      {
        static const uint8_t sharpS[] = {0xC3, 0x9F};
        return appendBytes(buffer, sharpS, sizeof(sharpS));
      }
      break;
    case HID_KEY_EQUAL:
      if (shifted)
        value = '`';
      else
      {
        static const uint8_t acute[] = {0xC2, 0xB4};
        return appendBytes(buffer, acute, sizeof(acute));
      }
      break;
    case HID_KEY_OPEN_BRACKET:
    { // ü
      const uint8_t umlaut[] = {0xC3, static_cast<uint8_t>(shifted ? 0x9C : 0xBC)};
      return appendBytes(buffer, umlaut, sizeof(umlaut));
    }
    case HID_KEY_CLOSE_BRACKET:
      if (altGr)
        value = '~';
      else
        value = shifted ? '*' : '+';
      break;
    case HID_KEY_BACK_SLASH: // Trust
    case HID_KEY_SHARP: // HP - HOTFIX: for NonUS Keyboards repeat HID_KEY_BACK_SLASH
      value = shifted ? '\'' : '#';
      break;
    case HID_KEY_KEYPAD_SLASH: // 100
      if (altGr)
        value = '|';
      else
        value = shifted ? '>' : '<';
      break;
    case HID_KEY_COLON:
    { // ö
      const uint8_t umlaut[] = {0xC3, static_cast<uint8_t>(shifted ? 0x96 : 0xB6)};
      return appendBytes(buffer, umlaut, sizeof(umlaut));
    }
    case HID_KEY_QUOTE:
    { // ä
      const uint8_t umlaut[] = {0xC3, static_cast<uint8_t>(shifted ? 0x84 : 0xA4)};
      return appendBytes(buffer, umlaut, sizeof(umlaut));
    }
    case HID_KEY_TILDE: // top left ^°
      if (shifted)
      {
        static const uint8_t degree[] = {0xC2, 0xB0};
        return appendBytes(buffer, degree, sizeof(degree));
      }
      value = '^';
      break;
    case HID_KEY_LESS:
      value = shifted ? ';' : ',';
      break;
    case HID_KEY_GREATER:
      value = shifted ? ':' : '.';
      break;
    case HID_KEY_SLASH:
      value = shifted ? '_' : '-';
      break;
    case HID_KEY_KEYPAD_DECIMAL:
      value = ',';
      break;
    default:
      return KeyboardLayout::mapKey(key, report, buffer);
    }
  }
  return appendByte(buffer, value);
}
