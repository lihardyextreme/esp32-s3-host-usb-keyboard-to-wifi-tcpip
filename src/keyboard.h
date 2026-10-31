#include <Arduino.h>
#include <hid_usage_keyboard.h>

class Keyboard
{
private:
  uint8_t keystate[256];

public:
  Keyboard();
  uint8_t handleInput(hid_keyboard_input_report_boot_t *input,
    char* pressedKeys, const size_t pressedKeysMaxLength);
};
