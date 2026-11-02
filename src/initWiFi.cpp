#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "defs.h"
#include "Keyboard.h"

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

const char* ssid     = TO_LITERAL(WIFI_SSID);
const char* password = TO_LITERAL(WIFI_PASSWORD);

void setupWiFi(Keyboard* keyboard)
{
  delay(10);

  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.print("connecting to ");
  Serial.println(ssid);

  uint8_t state = 0x01;
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
    keyboard->_requestLedState(state);
    state<<=1;
    if(state==0x08) state=0x01;
    delay(10);
    keyboard->applyLedState();
  }
  keyboard->requestLedState(false,false,false);
  delay(10);
  keyboard->applyLedState();

  Serial.println("");
  Serial.println("connected!");
  Serial.print("ip: ");
  Serial.println(WiFi.localIP());

  // mDNS
  if (!MDNS.begin(HOSTNAME)) {
    Serial.println("mDNS failed to start!");
  } else {
    Serial.printf("mDNS: %s.local %d\r\n", HOSTNAME, TCPIP_PORT);
    MDNS.addService("keyboard", "tcp", TCPIP_PORT);
    MDNS.addServiceTxt("keyboard", "tcp", "build-date", __DATE__);
    MDNS.addServiceTxt("keyboard", "tcp", "build-time", __TIME__);
  }
}
