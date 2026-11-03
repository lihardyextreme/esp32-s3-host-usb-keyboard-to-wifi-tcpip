#ifndef KEYBOARD_LAYOUT_H
#define KEYBOARD_LAYOUT_H

#include <Arduino.h>
#include <hid_usage_keyboard.h>
#include <hid_host.h>

enum class KeyMapResult : uint8_t {
  NotMapped,
  Mapped,
  BufferTooSmall
};

class KeyboardLayout {
public:
  virtual ~KeyboardLayout();

  virtual const char* name() const = 0;

  // Writes raw output bytes; output is not necessarily null-terminated.
  virtual KeyMapResult mapKey(
      uint8_t key,
      const hid_keyboard_input_report_boot_t& report,
      uint8_t* output,
      size_t capacity,
      size_t& written) const;

protected:
  static bool appendByte(
      uint8_t byte,
      uint8_t* output,
      size_t capacity,
      size_t& written);

  static bool appendBytes(
      const uint8_t* bytes,
      size_t length,
      uint8_t* output,
      size_t capacity,
      size_t& written);
};

#endif
