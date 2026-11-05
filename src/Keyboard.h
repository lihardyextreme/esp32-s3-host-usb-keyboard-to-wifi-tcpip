#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <Arduino.h>
#include <AsyncTCP.h>
#include <hid_usage_keyboard.h>
#include <hid_host.h>

#include "ServerContext.h"

class Keyboard
{
private:
  ServerContext *theServerContext;
  hid_host_device_handle_t hid_device_handle;
  uint8_t keystate[256];

  volatile uint8_t newLedStateRequested;
  volatile bool stateNumLock;
  volatile bool stateCapsLock;
  volatile bool stateScrollLock;

public:
  Keyboard();

  int getLayout();
  void setLayout(int layout);

  void setServerContext(ServerContext *context);
  void setDeviceHandle(hid_host_device_handle_t hid_device_handle);
  void handleInput(hid_keyboard_input_report_boot_t *input, String &buffer);
  void requestLedState(bool capslock, bool numlock, bool scroll);
  void _requestLedState(uint8_t state);
  void applyLedState();
  size_t processKeyboardData(hid_keyboard_input_report_boot_t* data);
};

#endif
