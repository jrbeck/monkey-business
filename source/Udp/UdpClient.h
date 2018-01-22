#pragma once

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstring>

#include <SDL2/SDL.h>

#include "UdpMessage.h"
#include "UdpListener.h"

#define UDP_CLIENT_SUCCESS (0)
#define UDP_CLIENT_ERROR_SOCKET_CREATE (1)
#define UDP_CLIENT_ERROR_HOST_ADDRESS (2)
#define UDP_CLIENT_ERROR_SEND (3)

class UdpClient {
public:
  UdpClient(size_t bufferSize);
  ~UdpClient();

  int openSocket();
  void closeSocket();

  int connectToHost(const char* host, int port);
  int resolveAddress(struct sockaddr_in* address, const char* host, int port) const;

  void startListening();
  void stopListening();

  int sendMessage(const char* message);
  int receiveMessage();
  UdpMessage* nextMessage();

private:
  size_t mBufferSize;
  unsigned char* mReceiveBuffer;

  int mSocketFileDescriptor;
  struct sockaddr_in mServerAddress;
  socklen_t mServerAddressLength;

  UdpListener* mUdpListener;
};
