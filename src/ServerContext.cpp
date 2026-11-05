#include <Arduino.h>

#include "ServerContext.h"

ServerContext::ServerContext()
{
  layout = KEYBOARD_LAYOUT;
  mode = KEYBOARD_MODE;
}

void ServerContext::setServer(AsyncServer *server)
{
  this->server = server;
}

void ServerContext::resetMode()
{
  this->mode = KEYBOARD_MODE;
}

void ServerContext::setMode(int mode)
{
  Serial.printf("-->> setMode %d \r\n", mode);
  this->mode = mode;
}

int ServerContext::getMode()
{
  return this->mode;
}

int ServerContext::getLayout()
{
  return layout;
}

void ServerContext::setLayout(int layout)
{
  Serial.printf("-->> setLayout %d \r\n", mode);
  this->layout = layout;
}

void ServerContext::setClient(AsyncClient *client)
{
  this->client = client;
}

AsyncClient *ServerContext::getClient()
{
  return this->client;
}

size_t ServerContext::sendAsyncString(const char *val)
{
  if (this->client == nullptr)
  {
    Serial.println("[AsyncServer] sendAsyncString - ignore");
    return 0;
  }
  Serial.printf("[AsyncServer] send: '%c' %d 0x%x\r\n", *val, *val, *val);
  return client->write(val);
}

size_t ServerContext::writeBytes(const char *val, size_t size)
{
  if (client == nullptr)
  {
    Serial.println("[AsyncServer] sendAsyncString - ignore");
    return 0;
  }
  Serial.printf("[AsyncServer] send %d bytes\r\n", size);
  return client->write(val, size);
}
