#include "layouts.h"

#include "KeyboardLayoutEN.h"
#include "KeyboardLayoutDE.h"

const KeyboardLayout *getKeyboardLayout(int layout)
{
  static const EnglishKeyboardLayout english;
  static const GermanKeyboardLayout german;

  if (layout == LAYOUT_EN)
    return &english;
  if (layout == LAYOUT_DE)
    return &german;
  return nullptr;
}
