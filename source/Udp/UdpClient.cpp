#include "UdpClient.h"

UdpClient::UdpClient(size_t bufferSize) :
  mBufferSize(bufferSize),
  mReceiveBuffer(nullptr),
  mSocketFileDescriptor(-1),
  mUdpListener(nullptr)
{
  mReceiveBuffer = new unsigned char[bufferSize];
  mServerAddressLength = sizeof(mServerAddress);
}

UdpClient::~UdpClient() {
  stopListening();
  closeSocket();

  if (mReceiveBuffer != nullptr) {
    delete [] mReceiveBuffer;
    mReceiveBuffer = nullptr;
  }
}

int UdpClient::openSocket() {
  mSocketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
  if (mSocketFileDescriptor < 0) {
    return UDP_CLIENT_ERROR_SOCKET_CREATE;
  }
  return UDP_CLIENT_SUCCESS;
}

void UdpClient::closeSocket() {
  if (mSocketFileDescriptor >= 0) {
    close(mSocketFileDescriptor);
    mSocketFileDescriptor = -1;
  }
}

int UdpClient::connectToHost(const char* host, int port) {
  return resolveAddress(&mServerAddress, host, port);
}

int UdpClient::resolveAddress(struct sockaddr_in* address, const char* host, int port) const {
  memset((char*)address, 0, sizeof(*address));
  address->sin_family = AF_INET;
  address->sin_port = htons(port);

  // TODO: convert to getaddrinfo method
  struct hostent* hp = gethostbyname(host);
  if (!hp) {
    return UDP_CLIENT_ERROR_HOST_ADDRESS;
  }

  memcpy((void*)&address->sin_addr, hp->h_addr_list[0], hp->h_length);

  return UDP_CLIENT_SUCCESS;
}

void UdpClient::startListening() {
  if (mSocketFileDescriptor >= 0 && mUdpListener == nullptr) {
    mUdpListener = new UdpListener(mBufferSize);
    mUdpListener->listenToSocket(mSocketFileDescriptor);
  }
}

void UdpClient::stopListening() {
  if (mSocketFileDescriptor >= 0 && mUdpListener != nullptr) {
    closeSocket();
    mSocketFileDescriptor = -1;
    delete mUdpListener;
    mUdpListener = nullptr;
  }
}

UdpMessage* UdpClient::nextMessage() {
  return mUdpListener->nextMessage();
}

int UdpClient::sendMessage(const char* message) {
  int bytesSent = sendto(mSocketFileDescriptor, message, strlen(message), 0, (struct sockaddr*)&mServerAddress, sizeof(mServerAddress));
  if (bytesSent < 0) {
    return UDP_CLIENT_ERROR_SEND;
  }
  return UDP_CLIENT_SUCCESS;
}

int UdpClient::waitForMessage() {
  printf("waiting for a response\n");

  int bytesReceived = recvfrom(mSocketFileDescriptor, mReceiveBuffer, mBufferSize, 0, (struct sockaddr*)&mServerAddress, &mServerAddressLength);
  printf("received %d bytes\n", bytesReceived);
  if (bytesReceived > 0) {
    mReceiveBuffer[bytesReceived] = 0;
    printf("received message: \"%s\"\n", mReceiveBuffer);
  }
  return UDP_CLIENT_SUCCESS;
}

int main() {
  UdpClient* udpClient = new UdpClient(2048);
  udpClient->openSocket();
  udpClient->connectToHost("127.0.0.1", 6789);
  udpClient->startListening();

  for (int i = 0; i < 10; ++i) {
    printf("sending\n");
    udpClient->sendMessage("heyo");
    // udpClient->waitForMessage();

    UdpMessage* udpMessage = udpClient->nextMessage();
    while (udpMessage != nullptr) {
      printf("MESSAGE: %s\n", udpMessage->mBody);
      // udpClient->sendMessage(udpMessage);
      delete udpMessage;
      udpMessage = udpClient->nextMessage();
    }

    SDL_Delay(200);
  }

  delete udpClient;

  return 0;
}
