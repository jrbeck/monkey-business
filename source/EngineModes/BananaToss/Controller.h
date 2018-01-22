#pragma once

#include "../../engine/EngineModeController.h"
#include "../../engine/Messaging/MessageBus.h"
#include "../../MonkeyServer.h"
#include "State.h"
#include "Renderer.h"

#define TICKS_PER_SECOND (1000.0)

#define PHYSICS_TICKS (17) // in milliseconds
#define DELTA_TIME (PHYSICS_TICKS / TICKS_PER_SECOND)

// #define GRAVITY (-9.8)
#define GRAVITY (0.0)

#define TOKEN_QUIT "QUIT"
#define TOKEN_RESET "RESET"
#define TOKEN_TARGET "TARGET"
#define TOKEN_SHOOT "SHOOT"
#define TOKEN_RESULT "RESULT"
#define TOKEN_HIT "HIT"
#define TOKEN_MISS "MISS"
#define TOKEN_SPEED "SPEED"
#define TOKEN_FAST "FAST"
#define TOKEN_SLOW "SLOW"


namespace BananaToss {
  enum ControllerModes {
    MODE_WAITING_FOR_CLIENT,
    MODE_SIMULATING,
    MODE_QUITTING
  };

  enum SimulationModes {
    SIM_MODE_SLOW,
    SIM_MODE_FAST
  };

  struct Input {
    int mProjectileIndex;
    float mForce;
    float mAngle;
  };

  class Controller : public EngineModeController {
  public:
    Controller(int windowWidth, int windowHeight, MonkeyServer* monkeyServer, int clientId, EngineXYZ::Messaging::MessageBus* messageBus, int parentMailboxId);
    ~Controller();

    int update();
    void render();

    ImageBuffer* getOutputImageBuffer();

  private:
    void reset();

    void handleServerEvents();
    void handleClientMessage(const ServerEvent* serverEvent);

    void sendTargetInfo();
    void clientShoot(TokenParser& tokenParser);
    void sendResults();

    void updateSimulation();
    void stepSimulation();

    void fireProjectile(Projectile* projectile, float angle, float force);
    void placeTarget(Target* target);

    bool isProjectileTargetIntersect(const Projectile* projectile, const Target* target) const;

    int XXXmWidth;

    int mMode;
    int mSimulationMode;
    MonkeyServer* mMonkeyServer;
    int mClientId;
    State* mState;
    Renderer* mRenderer;
    EngineXYZ::Messaging::MessageBus* mMessageBus;
    int mMailboxId;
    int mParentMailboxId;
  };
}
