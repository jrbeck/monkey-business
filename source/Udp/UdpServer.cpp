#include "UdpServer.h"

UdpServer::UdpServer(size_t bufferSize) :
  mBufferSize(bufferSize),
  mSocketFileDescriptor(-1),
  mUdpListener(nullptr)
{}

UdpServer::~UdpServer() {
  stopListening();
}

int UdpServer::bindToPort(int port) {
  mSocketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
  if (mSocketFileDescriptor < 0) {
    return UDP_SERVER_ERROR_SOCKET_CREATE;
  }

  struct sockaddr_in serverAddress;
  memset((char*)&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);

  if (bind(mSocketFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    return UDP_SERVER_ERROR_BIND;
  }
  return UDP_SERVER_SUCCESS;
}

void UdpServer::startListening() {
  if (mSocketFileDescriptor >= 0 && mUdpListener == nullptr) {
    mUdpListener = new UdpListener(mBufferSize);
    mUdpListener->listenToSocket(mSocketFileDescriptor);
  }
}

void UdpServer::stopListening() {
  if (mSocketFileDescriptor >= 0 && mUdpListener != nullptr) {
    close(mSocketFileDescriptor);
    mSocketFileDescriptor = -1;
    delete mUdpListener;
    mUdpListener = nullptr;
  }
}

UdpMessage* UdpServer::nextMessage() {
  return mUdpListener->nextMessage();
}

// void UdpServer::sendMessage(const struct sockaddr* address, const char* message) const {
//   socklen_t addressLength = sizeof(*address);
//   sendto(mSocketFileDescriptor, message, strlen(message), 0, address, addressLength);
// }

void UdpServer::sendMessage(const struct sockaddr_in* address, const char* message) const {
  socklen_t addressLength = sizeof(*address);
  sendto(mSocketFileDescriptor, message, strlen(message), 0, (struct sockaddr*)address, addressLength);
}

void UdpServer::sendMessage(const UdpMessage* udpMessage) const {
  sendMessage(&udpMessage->mAddress, udpMessage->mBody);
}

// int main() {
//   UdpServer* udpServer = new UdpServer(2048);
//   udpServer->bindToPort(6789);
//   udpServer->startListening();
//
//   for (int i = 0; i < 10; ++i) {
//     printf("iteration %d ...\n", i);
//     UdpMessage* udpMessage = udpServer->nextMessage();
//     while (udpMessage != nullptr) {
//       printf("MESSAGE: %s\n", udpMessage->mBody);
//       udpServer->sendMessage(udpMessage);
//       delete udpMessage;
//       udpMessage = udpServer->nextMessage();
//     }
//
//     SDL_Delay(1000);
//   }
//
//   udpServer->stopListening();
//   delete udpServer;
// }
