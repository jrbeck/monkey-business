#pragma once

#include <string.h>
#include <arpa/inet.h>

enum CLIENT_STATES {
  CLIENT_STATE_DISCONNECTED,
  CLIENT_STATE_CONNECTED,
  CLIENT_STATE_TRAINING_WAITING_FOR_SHOT_INFO,
};

class ClientState {
public:
  ClientState(unsigned int id, const struct sockaddr_in* address, int state);
  ~ClientState();

  bool doesAddressMatch(const struct sockaddr_in* address) const;

  int mId;
  struct sockaddr_in mAddress;
  int mState;
};
