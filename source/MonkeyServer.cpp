#include "MonkeyServer.h"

MonkeyServer::MonkeyServer() :
  mUdpServer(nullptr),
  mNextClientId(0),
  mTokenizer(nullptr)
{
  mUdpServer = new UdpServer(UDP_MESSAGE_LENGTH);
  mUdpServer->bindToPort(BIND_TO_PORT);
  mUdpServer->startListening();

  mTokenizer = new Tokenizer(TOKEN_SEPARATOR, UDP_MESSAGE_LENGTH);
}

MonkeyServer::~MonkeyServer() {
  if (mUdpServer != nullptr) {
    delete mUdpServer;
    mUdpServer = nullptr;
  }
  if (mTokenizer != nullptr) {
    delete mTokenizer;
    mTokenizer = nullptr;
  }

  size_t numClients = mClientStates.size();
  for (size_t i = 0; i < numClients; ++i) {
    delete mClientStates[i];
  }
  mClientStates.clear();

  clearEvents();
}

void MonkeyServer::clearEvents() {
  while (!mServerEvents.empty()) {
    // TODO: this is gross ... promote this thing and give it a d'tor
    if (mServerEvents.front()->mMessage != nullptr) {
      delete mServerEvents.front()->mMessage;
    }
    delete mServerEvents.front();
    mServerEvents.pop();
  }
}

ServerEvent* MonkeyServer::addEvent(ClientState* client, int eventType, UdpMessage* message) {
  ServerEvent* serverEvent = new ServerEvent;
  serverEvent->mClientId = client->mId;
  serverEvent->mType = eventType;
  serverEvent->mMessage = message;
  mServerEvents.push(serverEvent);
  return serverEvent;
}

void MonkeyServer::update() {
  UdpMessage* udpMessage;
  while ((udpMessage = mUdpServer->nextMessage()) != nullptr) {
    printf("UDP: %s\n", udpMessage->mBody);

    ClientState* client = getClientStateFromAddress(&udpMessage->mAddress);
    if (client == nullptr) {
      if (isConnectionMessage(udpMessage->mBody)) {
        client = addClient(&udpMessage->mAddress, CLIENT_STATE_CONNECTED);
        addEvent(client, SERVER_EVENT_CLIENT_CONNECTED, udpMessage);
        mTokenizer->clear();
        mTokenizer->addString(TOKEN_CONNECT);
        mTokenizer->addString(TOKEN_OK);
        mUdpServer->sendMessage(&client->mAddress, mTokenizer->getMessage());
      }
    }
    else if (isDisconnectionMessage(udpMessage->mBody)) {
      addEvent(client, SERVER_EVENT_CLIENT_DISCONNECTED, udpMessage);
      mTokenizer->clear();
      mTokenizer->addString(TOKEN_DISCONNECT);
      mTokenizer->addString(TOKEN_OK);
      mUdpServer->sendMessage(&client->mAddress, mTokenizer->getMessage());
    }
    else {
      addEvent(client, SERVER_EVENT_CLIENT_MESSAGE, udpMessage);
    }
  }
}

bool MonkeyServer::isConnectionMessage(const char* message) const {
  TokenParser tokenParser = TokenParser(message, TOKEN_SEPARATOR);
  return tokenParser.compare(0, TOKEN_CONNECT);
}

bool MonkeyServer::isDisconnectionMessage(const char* message) const {
  TokenParser tokenParser = TokenParser(message, TOKEN_SEPARATOR);
  return tokenParser.compare(0, TOKEN_DISCONNECT);
}

ServerEvent* MonkeyServer::getNextEvent() {
  if (mServerEvents.empty()) {
    return nullptr;
  }
  ServerEvent* serverEvent = mServerEvents.front();
  mServerEvents.pop();
  return serverEvent;
}

int MonkeyServer::sendMessage(int clientId, const char* message) const {
  ClientState* clientState = getClientStateFromId(clientId);
  if (clientState == nullptr) {
    return SEND_MESSAGE_CANT_FIND_CLIENT;
  }
  if (clientState->mState == CLIENT_STATE_DISCONNECTED) {
    return SEND_MESSAGE_CLIENT_DISCONNECTED;
  }

  mUdpServer->sendMessage(&clientState->mAddress, message);
  return SEND_MESSAGE_OK;
}

Tokenizer* MonkeyServer::getTokenizer() {
  mTokenizer->clear();
  return mTokenizer;
}

ClientState* MonkeyServer::getClientStateFromId(int id) const {
  size_t numClients = mClientStates.size();
  for (size_t i = 0; i < numClients; ++i) {
    if (mClientStates[i]->mId == id) {
      return mClientStates[i];
    }
  }
  return nullptr;
}

ClientState* MonkeyServer::getClientStateFromAddress(const struct sockaddr_in* address) const {
  size_t numClients = mClientStates.size();
  for (size_t i = 0; i < numClients; ++i) {
    if (mClientStates[i]->doesAddressMatch(address)) {
      return mClientStates[i];
    }
  }
  return nullptr;
}

ClientState* MonkeyServer::addClient(const struct sockaddr_in* address, int state) {
  ClientState* clientState;
  clientState = new ClientState(mNextClientId++, address, state);
  clientState->mState = CLIENT_STATE_CONNECTED;
  mClientStates.push_back(clientState);
  return clientState;
}
