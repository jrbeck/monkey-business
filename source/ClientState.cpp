#include "ClientState.h"

ClientState::ClientState(unsigned int id, const struct sockaddr_in* address, int state) :
  mId(id),
  mState(state)
{
  memcpy(&mAddress, address, sizeof(*address));
}

ClientState::~ClientState() {
}

bool ClientState::doesAddressMatch(const struct sockaddr_in* address) const {
  return (mAddress.sin_addr.s_addr == address->sin_addr.s_addr) && (mAddress.sin_port == address->sin_port);
}
