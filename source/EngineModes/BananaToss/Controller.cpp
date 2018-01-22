#include "Controller.h"

namespace BananaToss {
  Controller::Controller(int windowWidth, int windowHeight, MonkeyServer* monkeyServer, int clientId, EngineXYZ::Messaging::MessageBus* messageBus, int parentMailboxId) :
    mMode(MODE_WAITING_FOR_CLIENT),
    mSimulationMode(SIM_MODE_SLOW),
    mMonkeyServer(monkeyServer),
    mClientId(clientId),
    mState(nullptr),
    mRenderer(nullptr),
    mMessageBus(messageBus),
    mParentMailboxId(parentMailboxId)
  {
    XXXmWidth = windowWidth;

    mState = new State();
    mRenderer = new Renderer(windowWidth, windowHeight);

    mMailboxId = mMessageBus->newMailbox();

    reset();
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

  void Controller::reset() {
    mState->reset();
    placeTarget(&mState->mTargets[0]);
    mMode = MODE_WAITING_FOR_CLIENT;
  }

  void Controller::placeTarget(Target* target) {
    target->active = true;
    target->pos.x = mState->mPseudoRandom->nextDouble(250, 490);
    target->pos.y = mState->mPseudoRandom->nextDouble(50, 300);
    target->dimensions.x = 50.0;
    target->dimensions.y = 50.0;
  }

  int Controller::update() {
    handleServerEvents();

    switch (mMode) {
    case MODE_WAITING_FOR_CLIENT:
      break;
    case MODE_SIMULATING:
      updateSimulation();
      if (mState->mLastSimulationResult != SIMULATION_RESULT_NONE) {
        sendResults();
        mMode = MODE_WAITING_FOR_CLIENT;
      }
      break;
    case MODE_QUITTING:
      break;
    }

    return 0;
  }

  void Controller::handleServerEvents() {
    ServerEvent* serverEvent;
    while ((serverEvent = mMonkeyServer->getNextEvent()) != nullptr) {
      switch (serverEvent->mType) {
      case SERVER_EVENT_CLIENT_CONNECTED:
        // nothing to do here?
        break;
      case SERVER_EVENT_CLIENT_DISCONNECTED:
        // we should be quitting on this
        break;
      case SERVER_EVENT_CLIENT_MESSAGE:
        handleClientMessage(serverEvent);
        break;
      }
    }
  }

  void Controller::handleClientMessage(const ServerEvent* serverEvent) {
    if (serverEvent->mClientId != mClientId) {
      return;
    }

    TokenParser tokenParser = TokenParser(serverEvent->mMessage->mBody, TOKEN_SEPARATOR);

    if (tokenParser.compare(0, TOKEN_QUIT)) {
      mMonkeyServer->sendMessage(mClientId, "QUIT,OK");

      EngineXYZ::Messaging::Message message;
      message.mSenderId = mMailboxId;
      message.mType = MESSAGE_TYPE_ENTER_LOBBY;
      mMessageBus->sendMessage(mParentMailboxId, message);

      mMode = MODE_QUITTING;
    }
    else if (tokenParser.compare(0, TOKEN_RESET)) {
      reset();
      mMonkeyServer->sendMessage(mClientId, "RESET,OK");
    }
    else if (tokenParser.compare(0, TOKEN_TARGET)) {
      sendTargetInfo();
    }
    else if (tokenParser.compare(0, TOKEN_SHOOT)) {
      if (mMode == MODE_WAITING_FOR_CLIENT) {
        clientShoot(tokenParser);
        mMonkeyServer->sendMessage(mClientId, "SHOOT,OK");
        mMode = MODE_SIMULATING;
        mState->mLastTicks = SDL_GetTicks();
      }
      else {
        mMonkeyServer->sendMessage(mClientId, "SHOOT,FAIL");
      }
    }
    else if (tokenParser.compare(0, TOKEN_SPEED)) {
      if (tokenParser.compare(1, TOKEN_FAST)) {
        mSimulationMode = SIM_MODE_FAST;
        mMonkeyServer->sendMessage(mClientId, "SPEED,FAST");
      }
      else if (tokenParser.compare(1, TOKEN_SLOW)) {
        mSimulationMode = SIM_MODE_SLOW;
        mMonkeyServer->sendMessage(mClientId, "SPEED,SLOW");
      }
      else {
        mMonkeyServer->sendMessage(mClientId, "SPEED,FAIL");
      }
    }
    else {
      mMonkeyServer->sendMessage(mClientId, "UNKNOWN COMMAND");
    }
  }

  void Controller::sendTargetInfo() {
    if (!mState->mTargets[0].active) {
      // TODO: this seems like a bad way to do this ...
      return;
    }

    Tokenizer* tokenizer = mMonkeyServer->getTokenizer();
    tokenizer->addString(TOKEN_TARGET);
    tokenizer->addFloat(mState->mTargets[0].pos.x);
    tokenizer->addFloat(mState->mTargets[0].pos.y);
    mMonkeyServer->sendMessage(mClientId, tokenizer->getMessage());
  }

  void Controller::clientShoot(TokenParser& tokenParser) {
    float angle, force;
    tokenParser.getFloat(1, &angle);
    tokenParser.getFloat(2, &force);
    // TODO: we really should be building in Input struct from all this
    fireProjectile(&mState->mProjectiles[0], angle, force);
  }

  void Controller::sendResults() {
    switch (mState->mLastSimulationResult) {
    case SIMULATION_RESULT_HIT:
      // TODO: use yer Tokenizer wimp...
      mMonkeyServer->sendMessage(mClientId, "RESULT,HIT");
      break;
    case SIMULATION_RESULT_MISS:
    default:
      mMonkeyServer->sendMessage(mClientId, "RESULT,MISS");
    }
  }

  void Controller::updateSimulation() {
    Uint32 currentTicks = SDL_GetTicks();
    Uint32 deltaTicks = currentTicks - mState->mLastTicks;

    if (mSimulationMode == SIM_MODE_FAST) {
      deltaTicks = 500 * PHYSICS_TICKS;
    }
    mState->mLastTicks -= deltaTicks;

    while (deltaTicks >= PHYSICS_TICKS) {
      stepSimulation();
      if (!mState->mTargets[0].active) {
        mState->mLastSimulationResult = SIMULATION_RESULT_HIT;
      }
      else if (!mState->mProjectiles[0].active) {
        mState->mLastSimulationResult = SIMULATION_RESULT_MISS;
      }
      mState->mLastTicks += PHYSICS_TICKS;
      deltaTicks = currentTicks - mState->mLastTicks;
    }
  }

  void Controller::stepSimulation() {
    Projectile* projectile;
    Target* target;

    for (int i = 0; i < NUM_PROJECTILES; ++i) {
      projectile = &mState->mProjectiles[i];
      if (!projectile->active) {
        continue;
      }

      // if (!projectile->active) {
      //   float angle = mState->mPseudoRandom->nextDouble(0.0, 3.142 / 2);
      //   float force = mState->mPseudoRandom->nextDouble(75.0, 110.0);
      //   fireProjectile(projectile, angle, force);
      //   continue;
      // }

      projectile->vel.y += GRAVITY * DELTA_TIME;
      projectile->pos.x += DELTA_TIME * projectile->vel.x;
      projectile->pos.y += DELTA_TIME * projectile->vel.y;

      if (projectile->pos.y < 0.0 || projectile->pos.x < 0.0 || projectile->pos.x > XXXmWidth) {
        projectile->active = false;
        continue;
      }

      // test for collisions
      for (int j = 0; j < NUM_TARGETS; ++j) {
        target = &mState->mTargets[j];

        if (isProjectileTargetIntersect(projectile, target)) {
          mState->mNumHits++;
          // printf("{ input: [%.5f, %.5f], output: [%.5f, %.5f] }, \n", target->pos.x, target->pos.y, mState->mLastShotInfo.x, mState->mLastShotInfo.y);
          projectile->active = false;
          target->active = false;
          break;
        }
      }
    }
  }

  bool Controller::isProjectileTargetIntersect(const Projectile* projectile, const Target* target) const {
    v2d_t lowerLeft;
    lowerLeft.x = target->pos.x - (0.5 * target->dimensions.x);
    lowerLeft.y = target->pos.y - (0.5 * target->dimensions.y);
    if (projectile->pos.x < lowerLeft.x || projectile->pos.y < lowerLeft.y) {
      return false;
    }

    v2d_t upperRight;
    upperRight.x = target->pos.x + (0.5 * target->dimensions.x);
    upperRight.y = target->pos.y + (0.5 * target->dimensions.y);
    if (projectile->pos.x > upperRight.x || projectile->pos.y > upperRight.y) {
      return false;
    }

    return true;
  }

  void Controller::render() {
    mRenderer->drawFrame(mState);
  }

  void Controller::fireProjectile(Projectile* projectile, float angle, float force) {
    mState->mLastShotInfo.x = angle;
    mState->mLastShotInfo.y = force;

    ++mState->mTotalShots;

    projectile->active = true;
    projectile->pos.x = 16;
    projectile->pos.y = 16;
    projectile->vel.x = force * cos(angle);
    projectile->vel.y = force * sin(angle);
  }

  ImageBuffer* Controller::getOutputImageBuffer() {
    return mRenderer->getImageBuffer();
  }
}
