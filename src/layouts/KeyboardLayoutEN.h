#ifndef KEYBOARD_LAYOUT_EN_H
#define KEYBOARD_LAYOUT_EN_H

#include "KeyboardLayout.h"

class EnglishKeyboardLayout : public KeyboardLayout
{
public:
  const char *name() const override;
  KeyMapResult mapKey(
      uint8_t key,
      const hid_keyboard_input_report_boot_t &report,
      uint8_t *output,
      size_t capacity,
      size_t &written) const override;
};

#endif
