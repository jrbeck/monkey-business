#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

enum TCP_SOCKET_ERROR {
  TCP_SOCKET_OK = 0,
  TCP_SOCKET_OPEN_ERROR,
  TCP_SOCKET_BIND_ERROR,
  TCP_SOCKET_ACCEPT_ERROR,
  TCP_SOCKET_WRITE_ERROR,
  TCP_SOCKET_READ_ERROR
};

#define LISTEN_BACKLOG (5)

class TcpServer {
public:
  TcpServer(int port);
  ~TcpServer();

  int bind();
  void release();

  int acceptConnection();

  int writeLine(char* string);
  int readLine(char* buffer, size_t bufferSize);

private:
  int mPort;
  int mSocketDescriptor;
  int mClientSocketDescriptor;
};
