#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "UdpListener.h"
#include "UdpMessage.h"

#define UDP_SERVER_SUCCESS (0)
#define UDP_SERVER_ERROR_SOCKET_CREATE (1)
#define UDP_SERVER_ERROR_BIND (2)

class UdpServer {
public:
  UdpServer(size_t bufferSize);
  ~UdpServer();

  int bindToPort(int port);

  void startListening();
  void stopListening();

  UdpMessage* nextMessage();

  void sendMessage(const struct sockaddr_in* address, const char* message) const;
  void sendMessage(const UdpMessage* udpMessage) const;

private:
  size_t mBufferSize;
  int mSocketFileDescriptor;
  UdpListener* mUdpListener;
};
