#include "TcpClient.h"

void error(const char* msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  int portno = atoi(argv[2]);
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    error("ERROR opening socket");
  }

  struct hostent* server;
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  struct sockaddr_in serv_addr;
  bzero((char*)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    error("ERROR connecting");
  }

  printf("Please enter the message: ");
  char buffer[256];
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  int n = write(sockfd, buffer, strlen(buffer));
  if (n < 0) {
    error("ERROR writing to socket");
  }

  bzero(buffer, 256);
  n = read(sockfd, buffer, 255);
  if (n < 0) {
    error("ERROR reading from socket");
  }

  printf("%s\n", buffer);
  close(sockfd);
  return 0;
}
