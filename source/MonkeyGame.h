#pragma once

#include "engine/EngineModeController.h"
#include "engine/Messaging/MessageBus.h"
#include "EngineModes/Lobby/Controller.h"
#include "EngineModes/BananaToss/Controller.h"
#include "MonkeyServer.h"

enum GAME_STATES {
  GAME_STATE_WAITING_FOR_CONNECTION,
  GAME_STATE_CONNECTED,
  GAME_STATE_SIMULATING
};

class MonkeyGame {
public:
  MonkeyGame(int windowWidth, int windowHeight);
  ~MonkeyGame();

  bool update();
  void drawFrame();

  ImageBuffer* getOutputImageBuffer();

private:
  void handleServerEvents();
  bool handleMessages();
  bool handleControllerQuit();
  void deleteCurrentEngineMode();
  void enterLobbyMode();
  void enterTrainingMode(int clientId);

  int mWindowWidth;
  int mWindowHeight;

  MonkeyServer* mMonkeyServer;
  EngineModeController* mCurrentEngineModeController;
  EngineXYZ::Messaging::MessageBus* mMessageBus;
  int mMailboxId;
  int mGameState;
};
