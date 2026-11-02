#include <Arduino.h>
#include <AsyncTCP.h>

#include "ServerContext.h"
#include "Keyboard.h"

Keyboard* keyboard = nullptr;
ServerContext* theServerContext = nullptr;
void setupWiFi(Keyboard*);
ServerContext * setupServerContext();
Keyboard* setupUsbHid();

#define PIN 48
void blink() {
  neopixelWrite(PIN,RGB_BRIGHTNESS,0,0); // Red
  delay(100);
  neopixelWrite(PIN,0,0,0); // Off / black
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  delay(100);

  keyboard = setupUsbHid();
  keyboard->requestLedState(false,false,false);
  setupWiFi(keyboard);
  theServerContext = setupServerContext();
  keyboard->setServerContext(theServerContext);
}

int loops=0;
bool playLeds=true;

void doPlayLeds()
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
  if(keyboard== nullptr) return;
  blink();
  for(int i=0;i<100;i++) {
    delay(10);
    keyboard->applyLedState();
  }
  if(loops==0 && playLeds) {
    doPlayLeds();
    playLeds=false;
  }
}
