#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "defs.h"

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

const char* ssid     = TO_LITERAL(WIFI_SSID);
const char* password = TO_LITERAL(WIFI_PASSWORD);

void setupWiFi() {
  delay(10);

  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.print("connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("connected!");
  Serial.print("ip: ");
  Serial.println(WiFi.localIP());

  // mDNS
  if (!MDNS.begin(HOSTNAME)) {
    Serial.println("mDNS failed to start!");
  } else {
    MDNS.addService("netkeyboard", "tcp", TCPIP_PORT);
    MDNS.addServiceTxt("netkeyboard", "tcp", "build-date", __DATE__);
    MDNS.addServiceTxt("netkeyboard", "tcp", "build-time", __TIME__);
  }
}
