#include "KeyboardLayout.h"

KeyboardLayout::~KeyboardLayout() = default;

bool KeyboardLayout::appendByte(
    uint8_t byte,
    uint8_t* output,
    size_t capacity,
    size_t& written)
{
  if (output == nullptr || written >= capacity) {
    return false;
  }

  output[written++] = byte;
  return true;
}

bool KeyboardLayout::appendBytes(
    const uint8_t* bytes,
    size_t length,
    uint8_t* output,
    size_t capacity,
    size_t& written)
{
  // Avoid underflow if the caller passes an invalid current offset.
  if (written > capacity || length > capacity - written ||
      (length > 0 && (bytes == nullptr || output == nullptr))) {
    return false;
  }

  // Check the whole sequence first so escape codes or UTF-8 aren't partial.
  for (size_t i = 0; i < length; ++i) {
    output[written++] = bytes[i];
  }

  return true;
}
