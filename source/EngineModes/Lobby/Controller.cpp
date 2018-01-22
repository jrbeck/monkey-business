#include "Controller.h"

namespace Lobby {
  Controller::Controller(int windowWidth, int windowHeight, MonkeyServer* monkeyServer, EngineXYZ::Messaging::MessageBus* messageBus, int parentMailboxId) :
    mMonkeyServer(monkeyServer),
    mState(nullptr),
    mRenderer(nullptr),
    mMessageBus(messageBus),
    mParentMailboxId(parentMailboxId)
  {
    mState = new State();
    mRenderer = new Renderer(windowWidth, windowHeight);

    mMailboxId = mMessageBus->newMailbox();
  }

  Controller::~Controller() {
    if (mState != nullptr) {
      delete mState;
      mState = nullptr;
    }
    if (mRenderer != nullptr) {
      delete mRenderer;
      mRenderer = nullptr;
    }
  }

  int Controller::update() {
    handleServerEvents();

    return 0;
  }

  void Controller::handleServerEvents() {
    ServerEvent* serverEvent;
    while ((serverEvent = mMonkeyServer->getNextEvent()) != nullptr) {
      switch (serverEvent->mType) {
      case SERVER_EVENT_CLIENT_CONNECTED:
        // mGameState = GAME_STATE_CONNECTED;
        break;
      case SERVER_EVENT_CLIENT_DISCONNECTED:
        // mGameState = GAME_STATE_WAITING_FOR_CONNECTION;
        break;
      case SERVER_EVENT_CLIENT_MESSAGE:
        handleClientMessage(serverEvent);
        break;
      default:
        break;
      }
    }
  }

  void Controller::handleClientMessage(const ServerEvent* serverEvent) {
    TokenParser tokenParser = TokenParser(serverEvent->mMessage->mBody, TOKEN_SEPARATOR);

    if (tokenParser.compare(0, TOKEN_ENTER_TRAINING_MODE)) {
      mMonkeyServer->sendMessage(serverEvent->mClientId, "TRAIN,OK");

      EngineXYZ::Messaging::Message message;
      message.mSenderId = mMailboxId;
      message.mType = MESSAGE_TYPE_ENTER_TRAINING;
      message.mIntValue = serverEvent->mClientId;
      mMessageBus->sendMessage(mParentMailboxId, message);
    }
    else {
      mMonkeyServer->sendMessage(serverEvent->mClientId, "UNKNOWN COMMAND");
    }
  }

  void Controller::render() {
    mRenderer->drawFrame(mState);
  }

  ImageBuffer* Controller::getOutputImageBuffer() {
    return mRenderer->getImageBuffer();
  }
}
