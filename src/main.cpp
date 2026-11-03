#include <Arduino.h>
#include <AsyncTCP.h>

#include "ServerContext.h"
#include "Keyboard.h"

Keyboard* keyboard = nullptr;
ServerContext* theServerContext = nullptr;
void setupWiFi(Keyboard*);
ServerContext * setupServerContext();
Keyboard* setupUsbHid();

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  keyboard = setupUsbHid();
  keyboard->requestLedState(false,false,false);
  setupWiFi(keyboard);
  theServerContext = setupServerContext();
  keyboard->setServerContext(theServerContext);
}

int loops=0;
bool started=false;

void blinkKeyboardLeds()
{
  if(keyboard== nullptr) return;
    keyboard->requestLedState(true,true,true);
    delay(10);
    keyboard->applyLedState();
    delay(300);
    keyboard->requestLedState(false,false,false);
    delay(10);
    keyboard->applyLedState();
}

void loop() {
  if(keyboard==nullptr) return;
  for(int i=0;i<100;i++) {
    delay(10);
    keyboard->applyLedState();
  }
  if(!started) {
    blinkKeyboardLeds();
    started=true;
  }
}
