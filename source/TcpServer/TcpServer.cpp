#include "TcpServer.h"

TcpServer::TcpServer(int port) :
  mPort(port),
  mSocketDescriptor(0),
  mClientSocketDescriptor(0)
{}

TcpServer::~TcpServer()
{}

int TcpServer::bind() {
  mSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (mSocketDescriptor < 0) {
    return TCP_SOCKET_OPEN_ERROR;
  }

  struct sockaddr_in serverAddress;
  bzero((char*)&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(mPort);
  if (::bind(mSocketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    return TCP_SOCKET_BIND_ERROR;
  }

  listen(mSocketDescriptor, LISTEN_BACKLOG);

  return TCP_SOCKET_OK;
}

void TcpServer::release() {
   close(mClientSocketDescriptor);
   close(mSocketDescriptor);
}

int TcpServer::acceptConnection() {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLength = sizeof(clientAddr);
  mClientSocketDescriptor = accept(mSocketDescriptor, (struct sockaddr*)&clientAddr, &clientAddrLength);
  if (mClientSocketDescriptor < 0) {
    return TCP_SOCKET_ACCEPT_ERROR;
  }

  return TCP_SOCKET_OK;
}

int TcpServer::writeLine(char* string) {
  size_t stringLength = strlen(string);
  int bytesWritten = write(mClientSocketDescriptor, string, stringLength);
  if (bytesWritten < 0) {
    return TCP_SOCKET_WRITE_ERROR;
  }

  return TCP_SOCKET_OK;
}

int TcpServer::readLine(char* buffer, size_t bufferSize) {
  bzero(buffer, bufferSize);
  int bytesRead = read(mClientSocketDescriptor, buffer, bufferSize - 1);
  if (bytesRead < 0) {
    return TCP_SOCKET_READ_ERROR;
  }

  printf("Here is the message: %s\n", buffer);
  return TCP_SOCKET_OK;
}

int main(int argc, char* argv[]) {
  char buffer[256];

  TcpServer* tcpServer = new TcpServer(6789);

  tcpServer->bind();
  printf("pre\n");
  tcpServer->acceptConnection();
  printf("post\n");
  tcpServer->readLine(buffer, 256);
  tcpServer->writeLine("got it");
  tcpServer->release();

  delete tcpServer;
  return 0;
}
