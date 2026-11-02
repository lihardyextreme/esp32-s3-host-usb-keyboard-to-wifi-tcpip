#include <Arduino.h>

#include "layouts.h"
#include "layout_de.h"
#include "layout_en.h"

const TLayout* getLayoutMap(int layout)
{
  if (layout == LAYOUT_EN) return getLayoutMapEN();
  if (layout == LAYOUT_DE) return getLayoutMapDE();
  return nullptr;
}

const KeyboardLayout* getKeyboardLayout(int layout)
{
  static const EnglishKeyboardLayout english;
  static const GermanKeyboardLayout german;

  if (layout == LAYOUT_EN) return &english;
  if (layout == LAYOUT_DE) return &german;
  return nullptr;
}
