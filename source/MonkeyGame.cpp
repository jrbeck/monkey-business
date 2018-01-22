#include "MonkeyGame.h"

MonkeyGame::MonkeyGame(int windowWidth, int windowHeight) :
  mWindowWidth(windowWidth),
  mWindowHeight(windowHeight),
  mMonkeyServer(nullptr),
  mCurrentEngineModeController(nullptr),
  mMessageBus(nullptr),
  mMailboxId(-1),
  mGameState(GAME_STATE_WAITING_FOR_CONNECTION)
{
  mMonkeyServer = new MonkeyServer();

  mMessageBus = new EngineXYZ::Messaging::MessageBus();
  mMailboxId = mMessageBus->newMailbox();

  // // testing the message bus * * * * * * * * * * * * * * * * * * * * * * * *
  // printf("Messaging Stuff:\n");
  //
  // EngineXYZ::Messaging::Message message;
  // EngineXYZ::Messaging::Message* receivedMessage;
  //
  // mMailboxId = mMessageBus->newMailbox();
  // receivedMessage = mMessageBus->readNextMessage(mailboxId);
  //
  // if (receivedMessage == nullptr) {
  //   printf("No message ... passed\n");
  // }
  // else {
  //   printf("No message ... failed\n");
  // }
  //
  // message.mSenderId = mailboxId;
  // message.mType = 1;
  // message.mIntValue = 2;
  // mMessageBus->sendMessage(mailboxId, message);
  //
  // receivedMessage = mMessageBus->readNextMessage(mailboxId);
  //
  // printf("id: %d\n", mailboxId);
  // if (receivedMessage == nullptr) {
  //   printf("One message ... failed\n");
  // }
  // else {
  //   receivedMessage->print();
  // }
  //
  // receivedMessage = mMessageBus->readNextMessage(mailboxId);
  // if (receivedMessage == nullptr) {
  //   printf("After reading message ... passed\n");
  // }
  // else {
  //   printf("After reading message ... failed\n");
  // }

  enterLobbyMode();
}

MonkeyGame::~MonkeyGame() {
  if (mMonkeyServer != nullptr) {
    delete mMonkeyServer;
    mMonkeyServer = nullptr;
  }
  if (mCurrentEngineModeController != nullptr) {
    delete mCurrentEngineModeController;
    mCurrentEngineModeController = nullptr;
  }
  if (mMessageBus != nullptr) {
    delete mMessageBus;
    mMessageBus = nullptr;
  }
}

bool MonkeyGame::update() {
  bool quit = false;

  mMonkeyServer->update();

  // if (mGameState == GAME_STATE_WAITING_FOR_CONNECTION ||
  //   mGameState == GAME_STATE_CONNECTED) {
  //   // handleServerEvents();
  // }
  // else {
  //
  // }

  mCurrentEngineModeController->update();

  quit |= handleMessages();

  return quit;
}

bool MonkeyGame::handleMessages() {
  bool quit = false;
  EngineXYZ::Messaging::Message* message;

  while ((message = mMessageBus->readNextMessage(mMailboxId)) != nullptr) {
    switch (message->mType) {
    case MESSAGE_TYPE_CONTROLLER_QUIT:
      quit |= handleControllerQuit();
      break;
    case MESSAGE_TYPE_ENTER_TRAINING:
      deleteCurrentEngineMode();
      enterTrainingMode(message->mIntValue);
      break;
    case MESSAGE_TYPE_ENTER_LOBBY:
      deleteCurrentEngineMode();
      enterLobbyMode();
      break;
    default:
      printf("warning: MonkeyGame::handleMessages(): unknown message type received: %d\n", message->mType);
    }
  }

  return quit;
}

bool MonkeyGame::handleControllerQuit() {
  printf("warning: MonkeyGame::handleControllerQuit(): CONTROLLER QUIT! OH NOEZ!!\n");
  return false;
}

void MonkeyGame::deleteCurrentEngineMode() {
  if (mCurrentEngineModeController != nullptr) {
    delete mCurrentEngineModeController;
    mCurrentEngineModeController = nullptr;
  }
}

void MonkeyGame::enterLobbyMode() {
  mCurrentEngineModeController = new Lobby::Controller(mWindowWidth, mWindowHeight, mMonkeyServer, mMessageBus, mMailboxId);
}

void MonkeyGame::enterTrainingMode(int clientId) {
  mCurrentEngineModeController = new BananaToss::Controller(mWindowWidth, mWindowHeight, mMonkeyServer, clientId, mMessageBus, mMailboxId);
}

void MonkeyGame::drawFrame() {
  if (mCurrentEngineModeController != nullptr) {
    mCurrentEngineModeController->render();
  }
}

ImageBuffer* MonkeyGame::getOutputImageBuffer() {
  return mCurrentEngineModeController->getOutputImageBuffer();
}
