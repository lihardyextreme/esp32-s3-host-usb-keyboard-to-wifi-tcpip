#include <Arduino.h>
#include <AsyncTCP.h>
#include <hid_host.h>

#include "defs.h"
#include "Keyboard.h"

void handleClientDisconnect(void *arg, AsyncClient *client)
{
  ServerContext *theServerContext = (ServerContext *)arg;
  Serial.println("[AsyncServer] client disconnected");
  if (theServerContext->getClient() == client)
  {
    Serial.println("[AsyncServer] client nullified");
    theServerContext->setClient(nullptr);
    theServerContext->resetMode();
  }
  Serial.println("[AsyncServer] client deleted");
  delete client;
}

void handleClientError(void *arg, AsyncClient *client, int error)
{
  ServerContext *theServerContext = (ServerContext *)arg;
  Serial.printf("[AsyncServer] client error: (%d) %s\r\n", error, client->errorToString(error));
  if (theServerContext->getClient() == client)
  {
    theServerContext->setClient(nullptr);
    theServerContext->resetMode();
  }
  if (client->connected())
  {
    client->close();
  }
}

void handleOnData(void *arg, AsyncClient *client, void *data, size_t len)
{
  ServerContext *theServerContext = (ServerContext *)arg;
  uint8_t *ptr = (uint8_t *)data;
  for (int i = 0; i < len; i++)
  {
    if (ptr[i] == '0')
      theServerContext->setMode(0);
    if (ptr[i] == '1')
      theServerContext->setMode(1);
    if (ptr[i] == '2')
      theServerContext->setMode(2);
    if (ptr[i] == '3')
      theServerContext->setLayout(0);
    if (ptr[i] == '4')
      theServerContext->setLayout(1);
    if (ptr[i] == '\r' || ptr[i] == '\n')
    {
      Serial.printf("[AsyncServer] data[%d/%d]: %d 0x%x\r\n", i, len, ptr[i], ptr[i]);
    }
    else
    {
      Serial.printf("[AsyncServer] data[%d/%d]: %d '%c' 0x%x\r\n", i, len, ptr[i], ptr[i], ptr[i]);
    }
  }
  for (int i = 0; i < len; i++)
    if (ptr[i] == '.')
    {
      if (client->connected())
      {
        client->close();
      }
      return;
    }
}

void handleClient(void *arg, AsyncClient *client)
{
  ServerContext *theServerContext = (ServerContext *)arg;
  Serial.println("[AsyncServer] client connected.");
  if (theServerContext->getClient() != nullptr)
  {
    client->close();
    Serial.println("[AsyncServer] client busy");
    return;
  }
  theServerContext->setClient(client);
  client->onData(handleOnData, theServerContext);
  client->onError(handleClientError, theServerContext);
  client->onDisconnect(handleClientDisconnect, theServerContext);
  theServerContext->resetMode();
}

ServerContext *setupServerContext()
{
  ServerContext *context = new ServerContext;
  AsyncServer *server = new AsyncServer(TCPIP_PORT);
  server->onClient(&handleClient, context);
  server->begin();
  context->setServer(server);
  return context;
}
