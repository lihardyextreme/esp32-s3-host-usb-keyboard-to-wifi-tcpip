#include <Arduino.h>
#include <hid_usage_keyboard.h>

#include "Keyboard.h"
#include "layouts/layouts.h"

#define NO_NEW_LED_STATE 0xFF

Keyboard::Keyboard()
{
  newLedStateRequested = NO_NEW_LED_STATE;
  hid_device_handle = nullptr;
  theServerContext = nullptr;
  memset(keystate, 0, sizeof(keystate));
  stateNumLock = false;
  stateCapsLock = false;
  stateScrollLock = false;
}

int Keyboard::getLayout()
{
  if(theServerContext==nullptr)
    return KEYBOARD_LAYOUT;
  return theServerContext->getLayout();
}

void Keyboard::setLayout(int layout)
{
  if(theServerContext==nullptr)
    return;
  theServerContext->setLayout(layout);
}

void Keyboard::setServerContext(ServerContext* context)
{
  theServerContext = context;
}

void Keyboard::setDeviceHandle(hid_host_device_handle_t hid_device_handle)
{
  this->hid_device_handle = hid_device_handle;
  applyLedState();
}

uint8_t Keyboard::handleInput(hid_keyboard_input_report_boot_t *input, char* pressedKeys, const size_t pressedKeysMaxLength)
{
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
      }
    }
  }
  bool magicCombo = input->modifier.left_ctr && input->modifier.left_alt && input->modifier.left_gui;
  if(magicCombo) {
    if(input->key[1]==0) {
      if(input->key[0]==HID_KEY_ESC) {
        if(theServerContext==nullptr) return 0;
        AsyncClient* client = theServerContext->getClient();
        if(client==nullptr) return 0;
        if (client->connected())
          client->close();
        return 0;
      }
      if(input->key[0]==HID_KEY_PAUSE) {
        ESP.restart();
        return 0;
      }
      if(input->key[0]==HID_KEY_E) {
        setLayout(LAYOUT_EN);
        return 0;
      }
      if(input->key[0]==HID_KEY_D) {
        setLayout(LAYOUT_DE);
        return 0;
      }
    }
  }
  size_t countOfPressedKeys = 0;
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
      Serial.printf("[AsyncServer] pressed: %d 0x%x\r\n", key, key);
      uint8_t pressedKey = 0;
      if(pressed==HID_KEY_CAPS_LOCK) {
        stateCapsLock = !stateCapsLock;
        requestLedState(stateCapsLock, stateNumLock, stateScrollLock);
      } else if(pressed==HID_KEY_NUM_LOCK) {
        stateNumLock = !stateNumLock;
        requestLedState(stateCapsLock, stateNumLock, stateScrollLock);
      } else if(pressed==HID_KEY_SCROLL_LOCK) {
        stateScrollLock = !stateScrollLock;
        requestLedState(stateCapsLock, stateNumLock, stateScrollLock);
      } else if (pressed<=HID_KEY_APPLICATION) {
        if(premapKey(pressed, input, pressedKeys, countOfPressedKeys, pressedKeysMaxLength))
          return countOfPressedKeys;
        const KeyboardLayout* keyboardLayout = getKeyboardLayout(this->getLayout());
        if(keyboardLayout == nullptr)
          return countOfPressedKeys;
        size_t written = 0;
        keyboardLayout->mapKey(pressed, *input, ((uint8_t*)pressedKeys)+countOfPressedKeys, pressedKeysMaxLength, written);
        countOfPressedKeys+=written;
      } else {
        if(pressed >= HID_KEY_F1 && pressed <= HID_KEY_F7) {
          // for ANSI terminal
          // current buffer is 6 bytes !!!
          pressedKeys[countOfPressedKeys++]=0x1b;
          pressedKeys[countOfPressedKeys++]='[';
          pressedKeys[countOfPressedKeys++]='3';
          pressedKeys[countOfPressedKeys++]='1'+pressed-HID_KEY_F1;
          pressedKeys[countOfPressedKeys++]='m';
        }
      }
    }
  }
  return countOfPressedKeys;
}

void Keyboard::_requestLedState(uint8_t state)
{
  newLedStateRequested = state;
}

void Keyboard::requestLedState(bool capslock, bool numlock, bool scroll)
{
  uint8_t state = 0;
  if(numlock) state |= 0x1;
  if(capslock) state |= 0x2;
  if(scroll) state |= 0x4;
  newLedStateRequested = state;
}

void Keyboard::applyLedState()
{
  if(hid_device_handle==nullptr) return; // still no keayboard
  if(newLedStateRequested==NO_NEW_LED_STATE) return;
  uint8_t led_state[1] = { 0 };
  led_state[0]=newLedStateRequested;
  newLedStateRequested = NO_NEW_LED_STATE;
  esp_err_t err = hid_class_request_set_report(
    hid_device_handle, 
    HID_REPORT_TYPE_OUTPUT,
    0,
    led_state,
    sizeof(led_state)
  );
  if(ESP_OK != err)
    Serial.printf("applyLedState failed: %d\r\n", err);
}

size_t Keyboard::processKeyboardData(const uint8_t *const data, const int length) {
  if(theServerContext==nullptr)
    return 0;
  if(theServerContext->getMode()==2) {
    char pressedKeys[6];
    uint8_t count = handleInput((hid_keyboard_input_report_boot_t*)data, pressedKeys, sizeof(pressedKeys));
    if(count==0) return 0;
    size_t sent = 0;
    for(int i=0;i<count;i++) {
      char ch = pressedKeys[i];
      String msg;
      msg += ch;
      if(ch=='\r')
        msg += "\n";
      sent+=theServerContext->sendAsyncString(msg.c_str());
    }
    return sent;
  }
  if(theServerContext->getMode()==0) {
    const char *ptr = (const char *) data;
    return theServerContext->writeBytes(ptr, length);
  }
  if(theServerContext->getMode()==1) {
    String msg;
    msg+="keyboard:";
    for(int i=0;i<length;i++) {
      msg+=data[i];
      msg+=",";
    }
    return theServerContext->sendAsyncString(msg.c_str());
  }
  return 0;
}

bool mapKeyFx(uint8_t& key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength)
{
  if (key == HID_KEY_F1) // F1: 0x1B 0x4F 0x50
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x50;
    return true;
  }
  if (key == HID_KEY_F2) // F2: 0x1B 0x4F 0x51
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x51;
    return true;
  }
  if (key == HID_KEY_F3) // F3: 0x1B 0x4F 0x52
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x52;
    return true;
  }
  if (key == HID_KEY_F4) // F4: 0x1B 0x4F 0x53
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x4F;
    pressedKeys[countOfPressedKeys++] = 0x53;
    return true;
  }
  if (key == HID_KEY_F5) // F5: 0x1B 0x5B 0x31 0x35 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x35;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F6) // F6: 0x1B 0x5B 0x31 0x37 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x37;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F7) // F7: 0x1B 0x5B 0x31 0x38 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x38;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F8) // F8: 0x1B 0x5B 0x31 0x39 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x39;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F9) // F9: 0x1B 0x5B 0x32 0x30 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x30;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F10) // F10: 0x1B 0x5B 0x32 0x31 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x31;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F11) // F11: 0x1B 0x5B 0x32 0x33 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x33;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  if (key == HID_KEY_F12) // F12: 0x1B 0x5B 0x32 0x34 0x7E
  {
    pressedKeys[countOfPressedKeys++] = 0x1B;
    pressedKeys[countOfPressedKeys++] = 0x5B;
    pressedKeys[countOfPressedKeys++] = 0x32;
    pressedKeys[countOfPressedKeys++] = 0x34;
    pressedKeys[countOfPressedKeys++] = 0x7E;
    return true;
  }
  return false;
}

bool Keyboard::premapKey(uint8_t& key, hid_keyboard_input_report_boot_t *input, char *pressedKeys, size_t &countOfPressedKeys, const size_t pressedKeysMaxLength)
{
  if(mapKeyFx(key, input, pressedKeys, countOfPressedKeys, pressedKeysMaxLength))
    return true;
  if (key == HID_KEY_ENTER || key == HID_KEY_KEYPAD_ENTER) {
    pressedKeys[countOfPressedKeys++] = '\r';
    return true;
  }
  if (key == HID_KEY_TAB) {
    pressedKeys[countOfPressedKeys++] = '\t';
    return true;
  }
  if (key == HID_KEY_SPACE) {
    pressedKeys[countOfPressedKeys++] = ' ';
    return true;
  }
  if (key == HID_KEY_ESC) {
    pressedKeys[countOfPressedKeys++] = 0x1b;
    return true;
  }
  if (key == HID_KEY_DEL) { // backspace
    pressedKeys[countOfPressedKeys++] = '\b';
    return true;
  }
  if(stateNumLock) {
    if (key == HID_KEY_KEYPAD_0)
    {
      key = HID_KEY_0;
      return false;
    }
    if (key == HID_KEY_KEYPAD_1)
    {
      key = HID_KEY_1;
      return false;
    }
    if (key == HID_KEY_KEYPAD_2)
    {
      key = HID_KEY_2;
      return false;
    }
    if (key == HID_KEY_KEYPAD_3)
    {
      key = HID_KEY_3;
      return false;
    }
    if (key == HID_KEY_KEYPAD_4)
    {
      key = HID_KEY_4;
      return false;
    }
    if (key == HID_KEY_KEYPAD_5)
    {
      key = HID_KEY_5;
      return false;
    }
    if (key == HID_KEY_KEYPAD_6)
    {
      key = HID_KEY_6;
      return false;
    }
    if (key == HID_KEY_KEYPAD_7)
    {
      key = HID_KEY_7;
      return false;
    }
    if (key == HID_KEY_KEYPAD_8)
    {
      key = HID_KEY_8;
      return false;
    }
    if (key == HID_KEY_KEYPAD_9)
    {
      key = HID_KEY_9;
      return false;
    }
    if (key == HID_KEY_KEYPAD_DELETE)
    {
      key = HID_KEY_KEYPAD_DECIMAL;
      return false;
    }
  }
  if (key == HID_KEY_KEYPAD_ENTER)
  {
    key = HID_KEY_ENTER;
    return false;
  }
  if (key == HID_KEY_KEYPAD_DELETE)
  {
    key = HID_KEY_DELETE;
    return false;
  }
  if (key == HID_KEY_KEYPAD_DIV)
  {
    pressedKeys[countOfPressedKeys++] = '/';
    return true;
  }
  if (key == HID_KEY_KEYPAD_MUL)
{
    pressedKeys[countOfPressedKeys++] = '*';
    return true;
  }
  if (key == HID_KEY_KEYPAD_SUB)
  {
    pressedKeys[countOfPressedKeys++] = '-';
    return true;
  }
  if (key == HID_KEY_KEYPAD_ADD)
  {
    pressedKeys[countOfPressedKeys++] = '+';
    return true;
  }
  return false;
}
