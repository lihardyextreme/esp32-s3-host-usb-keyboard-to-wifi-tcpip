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
  if (theServerContext == nullptr)
    return KEYBOARD_LAYOUT;
  return theServerContext->getLayout();
}

void Keyboard::setLayout(int layout)
{
  if (theServerContext == nullptr)
    return;
  theServerContext->setLayout(layout);
}

void Keyboard::setServerContext(ServerContext *context)
{
  theServerContext = context;
}

void Keyboard::setDeviceHandle(hid_host_device_handle_t hid_device_handle)
{
  this->hid_device_handle = hid_device_handle;
  applyLedState();
}

void Keyboard::handleInput(hid_keyboard_input_report_boot_t *input, String &buffer)
{
  const KeyboardLayout *keyboardLayout = getKeyboardLayout(this->getLayout());
  if (keyboardLayout == nullptr)
    return;
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
  if (magicCombo)
  {
    if (input->key[1] == 0)
    {
      if (input->key[0] == HID_KEY_ESC)
      {
        if (theServerContext == nullptr)
          return;
        AsyncClient *client = theServerContext->getClient();
        if (client == nullptr)
          return;
        if (client->connected())
          client->close();
        return;
      }
      if (input->key[0] == HID_KEY_PAUSE)
      {
        ESP.restart();
        return;
      }
      if (input->key[0] == HID_KEY_E)
      {
        setLayout(LAYOUT_EN);
        return;
      }
      if (input->key[0] == HID_KEY_D)
      {
        setLayout(LAYOUT_DE);
        return;
      }
    }
  }
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
      if (pressed == HID_KEY_CAPS_LOCK)
      {
        stateCapsLock = !stateCapsLock;
        requestLedState(stateCapsLock, stateNumLock, stateScrollLock);
      }
      else if (pressed == HID_KEY_NUM_LOCK)
      {
        stateNumLock = !stateNumLock;
        requestLedState(stateCapsLock, stateNumLock, stateScrollLock);
      }
      else if (pressed == HID_KEY_SCROLL_LOCK)
      {
        stateScrollLock = !stateScrollLock;
        requestLedState(stateCapsLock, stateNumLock, stateScrollLock);
      }
      else if (pressed <= HID_KEY_APPLICATION)
      {
        if (keyboardLayout->premapKey(stateNumLock, pressed, input, buffer))
          continue;
        if (keyboardLayout->mapKey(pressed, *input, buffer))
          continue;
      }
    }
  }
}

void Keyboard::_requestLedState(uint8_t state)
{
  newLedStateRequested = state;
}

void Keyboard::requestLedState(bool capslock, bool numlock, bool scroll)
{
  uint8_t state = 0;
  if (numlock)
    state |= 0x1;
  if (capslock)
    state |= 0x2;
  if (scroll)
    state |= 0x4;
  newLedStateRequested = state;
}

void Keyboard::applyLedState()
{
  if (hid_device_handle == nullptr)
    return; // still no keayboard
  if (newLedStateRequested == NO_NEW_LED_STATE)
    return;
  uint8_t led_state[1] = {0};
  led_state[0] = newLedStateRequested;
  newLedStateRequested = NO_NEW_LED_STATE;
  esp_err_t err = hid_class_request_set_report(
      hid_device_handle,
      HID_REPORT_TYPE_OUTPUT,
      0,
      led_state,
      sizeof(led_state));
  if (ESP_OK != err)
    Serial.printf("applyLedState failed: %d\r\n", err);
}

size_t Keyboard::processKeyboardData(hid_keyboard_input_report_boot_t* data)
{
  if (theServerContext == nullptr)
    return 0;
  if (theServerContext->getMode() == 2)
  {
    String buffer;
    buffer.reserve(64);
    handleInput(data, buffer);
    unsigned int len = buffer.length();
    if (len == 0)
      return 0;
    size_t sent = 0;
    for (int i = 0; i < len; i++)
    {
      char ch = (char)buffer[i];
      String msg;
      msg += ch;
      if (ch == '\r')
        msg += "\n";
      sent += theServerContext->sendAsyncString(msg.c_str());
    }
    return sent;
  }
  if (theServerContext->getMode() == 0)
  {
    const char *ptr = (const char *)data;
    return theServerContext->writeBytes(ptr, sizeof(hid_keyboard_input_report_boot_t));
  }
  if (theServerContext->getMode() == 1)
  {
    String msg;
    msg += "keyboard:";
    unsigned int len = sizeof(hid_keyboard_input_report_boot_t);
    char* ptr = (char*)data;
    for (int i = 0; i < len; i++)
    {
      msg += ptr[i];
      msg += ",";
    }
    return theServerContext->sendAsyncString(msg.c_str());
  }
  return 0;
}
