#include "UdpListener.h"

UdpListener::UdpListener(size_t bufferSize) :
  mBufferSize(bufferSize),
  mReceiveBuffer(nullptr),
  mSocketFileDescriptor(-1),
  mTerminate(false)
{
  mReceiveBuffer = new char[bufferSize];
}

UdpListener::~UdpListener() {
  mThread.join();

  if (mReceiveBuffer != nullptr) {
    delete [] mReceiveBuffer;
    mReceiveBuffer = nullptr;
  }

  while (!mMessages.empty()) {
    delete mMessages.front();
    mMessages.pop();
  }
}

void UdpListener::listenToSocket(int socketFileDescriptor) {
  mSocketFileDescriptor = socketFileDescriptor;
  mThread = std::thread(&UdpListener::listen, this);
}

void UdpListener::stopListening() {
  mTerminate = true;
}

UdpMessage* UdpListener::nextMessage() {
  std::lock_guard<std::mutex> guard(mMessagesMutex);

  if (mMessages.empty()) {
    return nullptr;
  }
  UdpMessage* front = mMessages.front();
  mMessages.pop();
  return front;
}

void UdpListener::listen() {
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
  int bytesReceived;

  while(!mTerminate) {
    bytesReceived = recvfrom(mSocketFileDescriptor, mReceiveBuffer, mBufferSize, 0, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (bytesReceived > 0) {
      mReceiveBuffer[bytesReceived] = 0;
      UdpMessage* udpMessage = new UdpMessage(&clientAddress, mReceiveBuffer);
      pushMessage(udpMessage);
    }
    else {
      mTerminate = true;
    }
  }
}

void UdpListener::pushMessage(UdpMessage* udpMessage) {
  std::lock_guard<std::mutex> guard(mMessagesMutex);

  mMessages.push(udpMessage);
}
