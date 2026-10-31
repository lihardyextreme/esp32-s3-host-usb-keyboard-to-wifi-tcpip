#include <Arduino.h>
#include <hid_usage_keyboard.h>

#include <keyboard.h>

#define KEYBOARD_ENTER_MAIN_CHAR '\r' // 40

const uint8_t keycode2ascii[57][2] = {
    {0, 0},                                               /* HID_KEY_NO_PRESS        */
    {0, 0},                                               /* HID_KEY_ROLLOVER        */
    {0, 0},                                               /* HID_KEY_POST_FAIL       */
    {0, 0},                                               /* HID_KEY_ERROR_UNDEFINED */
    {'a', 'A'},                                           /* HID_KEY_A               */
    {'b', 'B'},                                           /* HID_KEY_B               */
    {'c', 'C'},                                           /* HID_KEY_C               */
    {'d', 'D'},                                           /* HID_KEY_D               */
    {'e', 'E'},                                           /* HID_KEY_E               */
    {'f', 'F'},                                           /* HID_KEY_F               */
    {'g', 'G'},                                           /* HID_KEY_G               */
    {'h', 'H'},                                           /* HID_KEY_H               */
    {'i', 'I'},                                           /* HID_KEY_I               */
    {'j', 'J'},                                           /* HID_KEY_J               */
    {'k', 'K'},                                           /* HID_KEY_K               */
    {'l', 'L'},                                           /* HID_KEY_L               */
    {'m', 'M'},                                           /* HID_KEY_M               */
    {'n', 'N'},                                           /* HID_KEY_N               */
    {'o', 'O'},                                           /* HID_KEY_O               */
    {'p', 'P'},                                           /* HID_KEY_P               */
    {'q', 'Q'},                                           /* HID_KEY_Q               */
    {'r', 'R'},                                           /* HID_KEY_R               */
    {'s', 'S'},                                           /* HID_KEY_S               */
    {'t', 'T'},                                           /* HID_KEY_T               */
    {'u', 'U'},                                           /* HID_KEY_U               */
    {'v', 'V'},                                           /* HID_KEY_V               */
    {'w', 'W'},                                           /* HID_KEY_W               */
    {'x', 'X'},                                           /* HID_KEY_X               */
    {'y', 'Y'},                                           /* HID_KEY_Y               */
    {'z', 'Z'},                                           /* HID_KEY_Z               */
    {'1', '!'},                                           /* HID_KEY_1               */
    {'2', '@'},                                           /* HID_KEY_2               */
    {'3', '#'},                                           /* HID_KEY_3               */
    {'4', '$'},                                           /* HID_KEY_4               */
    {'5', '%'},                                           /* HID_KEY_5               */
    {'6', '^'},                                           /* HID_KEY_6               */
    {'7', '&'},                                           /* HID_KEY_7               */
    {'8', '*'},                                           /* HID_KEY_8               */
    {'9', '('},                                           /* HID_KEY_9               */
    {'0', ')'},                                           /* HID_KEY_0               */
    {KEYBOARD_ENTER_MAIN_CHAR, KEYBOARD_ENTER_MAIN_CHAR}, /* HID_KEY_ENTER */
    {0, 0},                                               /* HID_KEY_ESC             */
    {'\b', 0},                                            /* HID_KEY_DEL             */
    {0, 0},                                               /* HID_KEY_TAB             */
    {' ', ' '},                                           /* HID_KEY_SPACE           */
    {'-', '_'},                                           /* HID_KEY_MINUS           */
    {'=', '+'},                                           /* HID_KEY_EQUAL           */
    {'[', '{'},                                           /* HID_KEY_OPEN_BRACKET    */
    {']', '}'},                                           /* HID_KEY_CLOSE_BRACKET   */
    {'\\', '|'},                                          /* HID_KEY_BACK_SLASH      */
    {'\\', '|'},
    /* HID_KEY_SHARP           */ // HOTFIX: for NonUS Keyboards repeat
                                  // HID_KEY_BACK_SLASH
    {';', ':'},                   /* HID_KEY_COLON           */
    {'\'', '"'},                  /* HID_KEY_QUOTE           */
    {'`', '~'},                   /* HID_KEY_TILDE           */
    {',', '<'},                   /* HID_KEY_LESS            */
    {'.', '>'},                   /* HID_KEY_GREATER         */
    {'/', '?'}                    /* HID_KEY_SLASH           */
};

Keyboard::Keyboard()
{
  memset(keystate, 0, sizeof(keystate));
}

uint8_t Keyboard::handleInput(hid_keyboard_input_report_boot_t *input, char* pressedKeys, const size_t pressedKeysMaxLength)
{
  Serial.printf("[AsyncServer] convertKBToChar: %d %d %d\r\n", input->modifier, input->reserved, input->key[0]);
  uint8_t released = 0;
  for (int ikey = 0; ikey < 256; ikey++)
  {
    if (keystate[ikey] != 0)
    { // was pressed
      bool found = false;
      for (int i = 0; i < sizeof(input->key); i++)
      {
        uint8_t key = input->key[i];
        if (key == ikey)
          found = true;
      }
      if (!found)
      {
        released = ikey;
        keystate[ikey] = 0;
        Serial.printf("[AsyncServer] released: %d\r\n", ikey);
      }
    }
  }
  uint8_t uppercase = input->modifier.left_shift || input->modifier.right_shift;
  uint8_t countOfPressedKeys = 0;
  uint8_t pressed = 0;
  for (int i = 0; i < sizeof(input->key); i++)
  {
    uint8_t key = input->key[i];
    if (key == 0)
      continue;
    if (keystate[key] == 0)
    {
      pressed = key;
      keystate[key] = 1; // pressed
      Serial.printf("[AsyncServer] pressed: %d\r\n", key);
      uint8_t pressedKey = 0;
      if ((pressed >= HID_KEY_A) && (pressed <= HID_KEY_SLASH)) {
        pressedKey = keycode2ascii[pressed][uppercase];
        if(countOfPressedKeys<pressedKeysMaxLength) {
          pressedKeys[countOfPressedKeys]=pressedKey;
          countOfPressedKeys++;
        } else {
          //ignore rest - buffer too small
        }
      } else {
        // ignore
      }
    }
  }
  return countOfPressedKeys;
}
