#pragma once

#include <vector>
#include <queue>

#include "Udp/UdpServer.h"
#include "ClientState.h"
#include "Tokenizer.h"
#include "TokenParser.h"

#define BIND_TO_PORT (6789)
#define UDP_MESSAGE_LENGTH (2048)

#define TOKEN_SEPARATOR (',')

#define TOKEN_OK ("OK")
#define TOKEN_CONNECT ("CONNECT")
#define TOKEN_DISCONNECT ("DISCONNECT")

enum SERVER_EVENT_TYPES {
  SERVER_EVENT_CLIENT_CONNECTED,
  SERVER_EVENT_CLIENT_DISCONNECTED,
  SERVER_EVENT_CLIENT_MESSAGE
};

enum SERVER_MESSAGE_SEND_STATUSES {
  SEND_MESSAGE_OK = 0,
  SEND_MESSAGE_CANT_FIND_CLIENT,
  SEND_MESSAGE_CLIENT_DISCONNECTED
};

struct ServerEvent {
  int mClientId;
  int mType;
  UdpMessage* mMessage;
};

class MonkeyServer {
public:
  MonkeyServer();
  ~MonkeyServer();

  void update();
  ServerEvent* getNextEvent();
  int sendMessage(int clientIndex, const char* message) const;

  Tokenizer* getTokenizer();

private:
  bool isConnectionMessage(const char* message) const;
  bool isDisconnectionMessage(const char* message) const;

  void clearEvents();
  ServerEvent* addEvent(ClientState* client, int eventType, UdpMessage* message);

  ClientState* getClientStateFromId(int id) const;
  ClientState* getClientStateFromAddress(const struct sockaddr_in* address) const;
  ClientState* addClient(const struct sockaddr_in* address, int state);

  UdpServer* mUdpServer;
  unsigned int mNextClientId;
  std::vector<ClientState*> mClientStates;
  std::queue<ServerEvent*> mServerEvents;
  Tokenizer* mTokenizer;
};
