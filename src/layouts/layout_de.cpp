#include <Arduino.h>
#include <hid_usage_keyboard.h>
#include <hid_host.h>

#include "layouts.h"

const uint8_t keycode2ascii_de[200][3] = {
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
    {'z', 'Z'},                                           /* HID_KEY_Y               */
    {'y', 'Y'},                                           /* HID_KEY_Z               */
    {'1', '!'},                                           /* HID_KEY_1               */
    {'2', '"'},                                           /* HID_KEY_2               */
    {0xFF},                                               /* HID_KEY_3               */
    {'4', '$'},                                           /* HID_KEY_4               */
    {'5', '%'},                                           /* HID_KEY_5               */
    {'6', '&'},                                           /* HID_KEY_6               */
    {0xFF},                                     /* HID_KEY_7               */
    {0xFF},                                     /* HID_KEY_8               */
    {0xFF},                                     /* HID_KEY_9               */
    {0xFF},                                     /* HID_KEY_0               */
    {KEYBOARD_ENTER_MAIN_CHAR, KEYBOARD_ENTER_MAIN_CHAR}, /* HID_KEY_ENTER */
    {0, 0},                                               /* HID_KEY_ESC             */
    {'\b', 0},                                            /* HID_KEY_DEL             */
    {0, 0},                                               /* HID_KEY_TAB             */
    {' ', ' '},                                           /* HID_KEY_SPACE           */
    {0xFF},                                               /* HID_KEY_MINUS           */
    {0xFF},                                               /* HID_KEY_EQUAL           */
    {0xC3, 0xBC, 0x9C},                                   /* HID_KEY_OPEN_BRACKET    */
    {0xFF},                                               /* HID_KEY_CLOSE_BRACKET   */
    {'\\', '|'},                                          /* HID_KEY_BACK_SLASH      */
    {'#', '\''},
    /* HID_KEY_SHARP           */ // HOTFIX: for NonUS Keyboards repeat
                                  // HID_KEY_BACK_SLASH
    {0xC3, 0xB6, 0x96},           /* HID_KEY_COLON           */
    {0xC3, 0xA4, 0x84},           /* HID_KEY_QUOTE           */
    {0xFF},                       /* HID_KEY_TILDE           */
    {',', ';'},                   /* HID_KEY_LESS            */
    {'.', ':'},                   /* HID_KEY_GREATER         */
    {'-', '_'},                   /* HID_KEY_SLASH           */
    {0},                          // 57 HID_KEY_CAPS_LOCK
    {0},                          // 58 HID_KEY_F1
    {0},                          // 59 HID_KEY_F2
    {0},                          // 60 HID_KEY_F3
    {0},                          // 61 HID_KEY_F4
    {0},                          // 62 HID_KEY_F5
    {0},                          // 63 HID_KEY_F6
    {0},                          // 64 HID_KEY_F7
    {0},                          // 65 HID_KEY_F8
    {0},                          // 66 HID_KEY_F9
    {0},                          // 67 HID_KEY_F10
    {0},                          // 68 HID_KEY_F11
    {0},                          // 69 HID_KEY_F12
    {0},                          // 70 HID_KEY_PRINT_SCREEN
    {0},                          // 71 HID_KEY_SCROLL_LOCK
    {0},                          // 72 HID_KEY_PAUSE
    {0xFF},                       // 73 HID_KEY_INSERT
    {0xFF},                       // 74 HID_KEY_HOME
    {0xFF},                       // 75 HID_KEY_PAGEUP
    {0xFF},                       // 76 HID_KEY_DELETE
    {0xFF},                       // 77 HID_KEY_END
    {0xFF},                       // 78 HID_KEY_PAGEDOWN
    {0xFF},                       // 79 HID_KEY_RIGHT
    {0xFF},                       // 80 HID_KEY_LEFT
    {0xFF},                       // 81 HID_KEY_DOWN
    {0xFF},                       // 82 HID_KEY_UP
    {0},                          // 83 HID_KEY_NUM_LOCK
    {0},                          // 84 HID_KEY_KEYPAD_DIV
    {0},                          // 85 HID_KEY_KEYPAD_MUL
    {0},                          // 86 HID_KEY_KEYPAD_SUB
    {0},                          // 87 HID_KEY_KEYPAD_ADD
    {0},                          // 88 HID_KEY_KEYPAD_ENTER
    {0xFF},                       // 89 HID_KEY_KEYPAD_1
    {0xFF},                       // 90 HID_KEY_KEYPAD_2
    {0xFF},                       // 91 HID_KEY_KEYPAD_3
    {0xFF},                       // 92 HID_KEY_KEYPAD_4
    {0xFF},                       // 93 HID_KEY_KEYPAD_5
    {0xFF},                       // 94 HID_KEY_KEYPAD_6
    {0xFF},                       // 95 HID_KEY_KEYPAD_7
    {0xFF},                       // 96 HID_KEY_KEYPAD_8
    {0xFF},                       // 97 HID_KEY_KEYPAD_9
    {0xFF},                       // 98 HID_KEY_KEYPAD_0
    {0xFF},                       // 99 HID_KEY_KEYPAD_DELETE
    {0xFF},                       // 100 HID_KEY_KEYPAD_SLASH
    {0},                          // 101 HID_KEY_APPLICATION
};

size_t mapKeyToLayoutDE(uint8_t key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, uint8_t &countOfPressedKeys, const size_t pressedKeysMaxLength)
{
  Serial.printf("key %d %x\r\n", key, key);
  if(key == HID_KEY_3)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = 0xC2;
      pressedKeys[countOfPressedKeys++] = 0xA7;
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '3';
    }
  }
  if(key == HID_KEY_7)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '/';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = '{';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '7';
    }
  }
  if(key == HID_KEY_8)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '(';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = '[';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '8';
    }
  }
  if(key == HID_KEY_9)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = ')';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = ']';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '9';
    }
  }
  if(key == HID_KEY_0)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '=';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = '}';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '0';
    }
  }
  if (key == HID_KEY_CLOSE_BRACKET)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '*';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = '~';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '+';
    }
  }
  if (key == HID_KEY_TILDE)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = 0xC2;
      pressedKeys[countOfPressedKeys++] = 0xB0;
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '^';
    }
  }
  if (key == HID_KEY_MINUS)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '?';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = '\\';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = 0xC3;
      pressedKeys[countOfPressedKeys++] = 0x9f;
    }
  }
  if (key == HID_KEY_EQUAL)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '`';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = 0xC2;
      pressedKeys[countOfPressedKeys++] = 0xB4;
    }
  }
  if (key == HID_KEY_KEYPAD_SLASH)
  {
    if (input->modifier.left_shift || input->modifier.right_shift)
    {
      pressedKeys[countOfPressedKeys++] = '>';
    }
    else if (input->modifier.right_alt)
    {
      pressedKeys[countOfPressedKeys++] = '|';
    }
    else
    {
      pressedKeys[countOfPressedKeys++] = '<';
    }
  }
  if (key == HID_KEY_RIGHT || key == HID_KEY_KEYPAD_6)
  {
    Serial.println("HID_KEY_RIGHT");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x43;
  }
  if (key == HID_KEY_LEFT || key == HID_KEY_KEYPAD_4)
  {
    Serial.println("HID_KEY_LEFT");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x44;
  }
  if (key == HID_KEY_DOWN || key == HID_KEY_KEYPAD_2)
  {
    Serial.println("HID_KEY_DOWN");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x42;
  }
  if (key == HID_KEY_UP || key == HID_KEY_KEYPAD_8)
  {
    Serial.println("HID_KEY_UP");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x41;
  }
  // Numpad 5 - Begin/Clear
  if (key == HID_KEY_KEYPAD_5)
  {
    Serial.println("HID_KEY_KEYPAD_5");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x45;
  }
  // Insert
  if (key == HID_KEY_INSERT || key == HID_KEY_KEYPAD_0)
  {
    Serial.println("HID_KEY_INSERT");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x7E;
  }
  // Delete
  if (key == HID_KEY_DELETE || key == HID_KEY_KEYPAD_DELETE)
  {
    Serial.println("HID_KEY_DELETE");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x33;
    pressedKeys[countOfPressedKeys++] = 0x7E;
  }
  // Home
  if (key == HID_KEY_HOME || key == HID_KEY_KEYPAD_7)
  {
    Serial.println("HID_KEY_HOME");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x7E;
  }
  // End
  if (key == HID_KEY_END || key == HID_KEY_KEYPAD_1)
  {
    Serial.println("HID_KEY_END");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x34;
    pressedKeys[countOfPressedKeys++] = 0x7E;
  }
  // Page Up
  if (key == HID_KEY_PAGEUP || key == HID_KEY_KEYPAD_9)
  {
    Serial.println("HID_KEY_PAGEUP");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x35;
    pressedKeys[countOfPressedKeys++] = 0x7E;
  }
  // Page Down
  if (key == HID_KEY_PAGEDOWN || key == HID_KEY_KEYPAD_3)
  {
    Serial.println("HID_KEY_PAGEDOWN");
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x36;
    pressedKeys[countOfPressedKeys++] = 0x7E;
  }
  /*
• Backspace: 0x7F oder 0x08
• Tab: 0x09
• Enter: 0x0D (CR) oder 0x0A (LF)
• Escape: 0x1B
*/
  return 0;
}

bool mapSpecialKeyDE(uint8_t& key) {
  if(key==HID_KEY_DECIMAL_SEPARATOR) {
    key = ',';
    return true;
  }
  return false;
}

const TLayout* getLayoutMapDE()
{
    return &keycode2ascii_de;
}
