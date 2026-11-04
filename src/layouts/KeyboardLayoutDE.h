#ifndef KEYBOARD_LAYOUT_DE_H
#define KEYBOARD_LAYOUT_DE_H

#include "KeyboardLayout.h"

class GermanKeyboardLayout : public KeyboardLayout
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
