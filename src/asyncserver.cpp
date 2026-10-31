#include <Arduino.h>
#include <AsyncTCP.h>
#include "defs.h"

#include "keyboard.h"

Keyboard keyboard;

AsyncServer* theAsyncServer = nullptr;
const int AsyncServerPort = 9009;

AsyncClient *theAsyncClient = nullptr;
int mode = KEYBOARD_MODE;

void resetMode() {
  mode = KEYBOARD_MODE;
}

void handleClientDisconnect(void *arg, AsyncClient *client) {
  Serial.println("[AsyncServer] client disconnected");
  if (theAsyncClient == client) {
    Serial.println("[AsyncServer] client nullified");
    theAsyncClient = nullptr;
    resetMode();
  }
  Serial.println("[AsyncServer] client deleted");
  delete client;
}

void handleClientError(void *arg, AsyncClient *client, int error) {
  Serial.printf("[AsyncServer] client error: (%d) %s\r\n", error, client->errorToString(error));
  if (theAsyncClient == client) {
    theAsyncClient = nullptr;
    resetMode();
  }
  if (client->connected()) {
    client->close();
  }
}

void handleOnData(void *arg, AsyncClient *client, void *data, size_t len) {
  uint8_t *ptr = (uint8_t *) data;
  for(int i=0; i<len;i++) {
    if(ptr[i]=='0') mode=0;
    if(ptr[i]=='1') mode=1;
    if(ptr[i]=='2') mode=2;
    if(ptr[i]=='\r' || ptr[i]=='\n') {
      Serial.printf("[AsyncServer] data[%d/%d]: %d 0x%x\r\n", i, len, ptr[i], ptr[i]);
    } else {
      Serial.printf("[AsyncServer] data[%d/%d]: %d '%c' 0x%x\r\n", i, len, ptr[i], ptr[i], ptr[i]);
    }
  }
  for(int i=0; i<len;i++)
    if(ptr[i]=='.') {
      if (client->connected()) {
        client->close();
      }
      return;
    }
}

void handleClient(void *arg, AsyncClient *client) {
  Serial.println("[AsyncServer] client connected.");
  if(theAsyncClient!=nullptr) {
    client->close();
    Serial.println("[AsyncServer] client busy");
    return;
  }
  theAsyncClient = client;
  resetMode();
  theAsyncClient->onData(handleOnData, nullptr);
  theAsyncClient->onError(handleClientError, nullptr);
  theAsyncClient->onDisconnect(handleClientDisconnect, nullptr);
}

size_t sendAsyncString(const char* data) {
  if(theAsyncClient==nullptr) {
    Serial.println("[AsyncServer] ignore");
    return 0;
  }
  Serial.print("[AsyncServer] send:");
  Serial.println(data);
  return theAsyncClient->write(data);
}

size_t sendAsyncChar(const char ch) {
  const char *ptr = &ch;
  size_t length = 1;
  return theAsyncClient->write(ptr, length);
}

size_t sendAsyncData(const uint8_t *const data, const int length) {
  if(theAsyncClient==nullptr) {
    Serial.println("[AsyncServer] ignore");
    return 0;
  }
  if(mode==0) {
    const char *ptr = (const char *) data;
    size_t size = length;
    return theAsyncClient->write(ptr, length);
  }
  if(mode==1) {
    String msg;
    msg+="keyboard:";
    for(int i=0;i<length;i++) {
      msg+=data[i];
      msg+=",";
    }
    return sendAsyncString(msg.c_str());
  }
  if(mode==2) {
    char pressedKeys[6];
    uint8_t count = keyboard.handleInput((hid_keyboard_input_report_boot_t*)data, pressedKeys, sizeof(pressedKeys));
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
      sent+=sendAsyncString(msg.c_str());
    }
    return sent;
  }
  return 0;
}

void setupAsyncServer() {
  theAsyncServer = new AsyncServer(AsyncServerPort);
  theAsyncServer->onClient(&handleClient, nullptr);
  theAsyncServer->begin();
}
