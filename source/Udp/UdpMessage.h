#pragma once

#include <cstdio>
#include <string.h>
#include <arpa/inet.h>

class UdpMessage {
public:
  UdpMessage(const struct sockaddr_in* address, const char* body);
  ~UdpMessage();

  void print() const;

  struct sockaddr_in mAddress;
  char* mBody;
};
