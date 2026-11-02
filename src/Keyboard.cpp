#include <Arduino.h>
#include <hid_usage_keyboard.h>

#include "defs.h"
#include "layouts/layouts.h"
#include "Keyboard.h"

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
  bool magicCombo = input->modifier.left_ctr && input->modifier.left_alt && input->modifier.left_gui;
  if(magicCombo) {
    //Serial.printf("[AsyncServer] MAGIC %d %d %d %d\r\n",input->key[0],input->key[1],input->key[2],input->key[3]);
    if(input->key[1]==0) {
      if(input->key[0]==HID_KEY_E) {
        Serial.printf("[AsyncServer] MAGIC english\r\n");
        setLayout(LAYOUT_EN);
        return 0;
      }
      if(input->key[0]==HID_KEY_D) {
        Serial.printf("[AsyncServer] MAGIC deutsch\r\n");
        setLayout(LAYOUT_DE);
        return 0;
      }
      if(input->key[0]==HID_KEY_1) {
        Serial.printf("[AsyncServer] MAGIC 1\r\n");
        return 0;
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
        uint8_t newkey = 0;
        if(stateNumLock) {
          uint8_t newkey=0;
          if(mapKeyToLayoutNumLock(pressed, input, pressedKeys, countOfPressedKeys, pressedKeysMaxLength, newkey)) {
            Serial.printf("[AsyncServer] NumLock mapped\r\n");
            return countOfPressedKeys;
          }
          if(newkey!=0)
            pressed = newkey;
        }
        boolean mapped = false;
        if(this->getLayout()==LAYOUT_EN)
          mapped = mapSpecialKeyEN(pressed);
        if(this->getLayout()==LAYOUT_DE)
          mapped = mapSpecialKeyDE(pressed);
        if(mapped) {
          Serial.printf("[AsyncServer] already mapped %d\r\n", pressed);
          pressedKeys[countOfPressedKeys++]=pressed;
          return countOfPressedKeys;
        }
        if(newkey==0 && mapKeyToLayoutXX(pressed, input, pressedKeys, countOfPressedKeys, pressedKeysMaxLength, newkey)) {
          Serial.printf("[AsyncServer] xx processed\r\n");
          return countOfPressedKeys;
        } else if(newkey!=0) {
          Serial.printf("[AsyncServer] xx mapped\r\n");
          pressed = newkey;
        }
        const TLayout& layout = *getLayoutMap(this->getLayout());
        Serial.printf("[AsyncServer] mapped: %d\r\n", layout[pressed][0]);
        bool isExtended = layout[pressed][0] == 0xFF;
        if(isExtended) {
          mapKeyToLayoutDE(pressed, input, pressedKeys, countOfPressedKeys, pressedKeysMaxLength);
        } else {
          bool isUmlaut = layout[pressed][0] == 0xC3;
          if(!isUmlaut) {
            pressedKey = layout[pressed][uppercase];
            if(countOfPressedKeys<pressedKeysMaxLength) {
              pressedKeys[countOfPressedKeys]=pressedKey;
              countOfPressedKeys++;
            } else {
              //ignore rest - buffer too small
            }
          } else {
              pressedKeys[countOfPressedKeys++]=layout[pressed][0];
              pressedKeys[countOfPressedKeys++]=layout[pressed][1+uppercase];
          }
        }
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
        // ignore
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
  Serial.printf("requestLedState %d %d %d\r\n",capslock, numlock, scroll);
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
  Serial.printf("applyLedState %d\r\n",newLedStateRequested);
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
  else
    Serial.printf("applyLedState ok\r\n");
}

size_t Keyboard::processKeyboardData(const uint8_t *const data, const int length) {
  if(theServerContext==nullptr)
    return 0;
  if(theServerContext->getMode()==2) {
    char pressedKeys[6];
    uint8_t count = handleInput((hid_keyboard_input_report_boot_t*)data, pressedKeys, sizeof(pressedKeys));
    Serial.printf("[AsyncServer] handleInput: %d\r\n", count);
    if(count==0) return 0;
    size_t sent = 0;
    for(int i=0;i<count;i++) {
      char ch = pressedKeys[i];
      String msg;
      msg += ch;
      if(ch=='\r')
        msg += "\n";
      Serial.printf("[AsyncServer] send char: %c %d 0x%x\r\n", ch, ch, ch);
      sent+=theServerContext->sendAsyncString(msg.c_str());
    }
    return sent;
  }
  if(theServerContext->getMode()==0) {
    const char *ptr = (const char *) data;
    size_t size = length;
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
