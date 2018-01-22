#pragma once

#include <mutex>
#include <thread>
#include <queue>

#include "UdpMessage.h"

class UdpListener {
public:
  UdpListener(size_t bufferSize);
  ~UdpListener();

  void listenToSocket(int descriptor);
  void stopListening();

  UdpMessage* nextMessage();

private:
  void listen();
  void pushMessage(UdpMessage* udpMessage);

  size_t mBufferSize;
  char* mReceiveBuffer;
  int mSocketFileDescriptor;

  bool mTerminate;
  std::thread mThread;

  std::queue<UdpMessage*> mMessages;
  std::mutex mMessagesMutex;
};
