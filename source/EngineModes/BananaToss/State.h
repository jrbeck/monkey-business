#pragma once

#include <SDL2/SDL.h>

#include "../../engine/Vec2.h"
#include "../../engine/PseudoRandom.h"

#define NUM_PROJECTILES (1)
#define NUM_TARGETS (1)

namespace BananaToss {
  enum SimulationResults {
    SIMULATION_RESULT_NONE,
    SIMULATION_RESULT_HIT,
    SIMULATION_RESULT_MISS
  };

  struct Projectile {
    bool active;
    v2d_t pos;
    v2d_t vel;
  };

  struct Target {
    bool active;
    v2d_t pos;
    v2d_t dimensions;
  };

  class State {
public:
    State();
    ~State();

    void reset();

    Projectile* mProjectiles;
    Target* mTargets;
    PseudoRandom* mPseudoRandom;

    Uint32 mLastTicks;

    int mNumHits;
    int mTotalShots;
    v2d_t mLastShotInfo;

    int mLastSimulationResult;
  };
}
