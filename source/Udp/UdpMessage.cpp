#include "UdpMessage.h"

UdpMessage::UdpMessage(const struct sockaddr_in* address, const char* body) :
  mBody(nullptr)
{
  memcpy(&mAddress, address, sizeof(*address));

  size_t bodyLength = strlen(body);
  if (bodyLength > 0) {
    mBody = new char[bodyLength + 1];
    memcpy(mBody, body, bodyLength + 1);
  }
}

UdpMessage::~UdpMessage() {
  if (mBody != nullptr) {
    delete [] mBody;
  }
}

// char* UdpServer::internetAddress(const struct sockaddr* address) {
//   char* addressString = new char[1024];
//   sprintf(addressString, "%s:%d", inet_ntoa(mAddress.sin_addr), ntohs(mAddress.sin_port));
//   return addressString;
// }

void UdpMessage::print() const {
  printf("UdpMessage\n");
  printf("Address: %s:%d\n", inet_ntoa(mAddress.sin_addr), ntohs(mAddress.sin_port));
  printf("Body: %s\n", mBody);
}
