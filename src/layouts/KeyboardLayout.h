#ifndef KEYBOARD_LAYOUT_H
#define KEYBOARD_LAYOUT_H

#include <Arduino.h>
#include <hid_usage_keyboard.h>
#include <hid_host.h>

class KeyboardLayout
{
public:
  virtual ~KeyboardLayout();
  virtual const char *name() const = 0;

  bool premapKey(bool stateNumLock, uint8_t &key, hid_keyboard_input_report_boot_t *input, String &buffer) const;
  bool mapKeyFx(uint8_t &key, hid_keyboard_input_report_boot_t *input, String &buffer) const;

  virtual bool mapKey(
      uint8_t key,
      const hid_keyboard_input_report_boot_t &report,
      String &buffer) const;

protected:
  static bool appendByte(String &buffer, uint8_t byte);
  static bool appendBytes(String &buffer, const uint8_t *bytes, size_t length);
};

#endif
