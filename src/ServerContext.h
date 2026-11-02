#ifndef __SERVER_CONTEXT_H__
#define __SERVER_CONTEXT_H__

#include <AsyncTCP.h>

class ServerContext
{
private:

  int mode;
  int layout;
  AsyncClient *client = nullptr;
  AsyncServer* server = nullptr;

public:

  ServerContext();

  void setServer(AsyncServer* server);

  void resetMode();
  void setMode(int mode);
  int getMode();

  int getLayout();
  void setLayout(int layout);

  void setClient(AsyncClient* client);
  AsyncClient* getClient();
  
  size_t sendAsyncString(const char* data);
  size_t writeBytes(const char *data, size_t size);
};

#endif
