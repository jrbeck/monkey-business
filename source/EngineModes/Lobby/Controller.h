#pragma once

#include "../../engine/EngineModeController.h"
#include "../../engine/Messaging/MessageBus.h"
#include "../../MonkeyServer.h"
#include "State.h"
#include "Renderer.h"

#define TOKEN_ENTER_TRAINING_MODE "TRAIN"

namespace Lobby {
  class Controller : public EngineModeController {
  public:
    Controller(int windowWidth, int windowHeight, MonkeyServer* monkeyServer, EngineXYZ::Messaging::MessageBus* messageBus, int parentMailboxId);
    ~Controller();

    int update();
    void render();

    ImageBuffer* getOutputImageBuffer();

  private:
    void handleServerEvents();
    void handleClientMessage(const ServerEvent* serverEvent);

    MonkeyServer* mMonkeyServer;
    State* mState;
    Renderer* mRenderer;
    EngineXYZ::Messaging::MessageBus* mMessageBus;
    int mMailboxId;
    int mParentMailboxId;
  };
}
