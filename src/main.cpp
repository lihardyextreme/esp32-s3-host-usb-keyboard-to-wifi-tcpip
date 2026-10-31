#include <Arduino.h>

void setupWiFi();
void setupAsyncServer();
void setupUsbHid();

#define PIN 48
void blink() {
  neopixelWrite(PIN,RGB_BRIGHTNESS,0,0); // Red
  delay(100);
  neopixelWrite(PIN,0,0,0); // Off / black
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  for(int i = 5; i>=0; i--) {
    blink();
    delay(1000);
    Serial.println(i);
  }

  setupUsbHid();
  setupWiFi();
  setupAsyncServer();
}

int loops=0;
void loop() {
  blink();
  delay(1000);
  //Serial.println("loop: ");
  //Serial.println(loops++);
}
