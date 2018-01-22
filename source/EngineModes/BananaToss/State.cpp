#include "State.h"

namespace BananaToss {
  State::State() :
    mProjectiles(nullptr),
    mTargets(nullptr),
    mPseudoRandom(nullptr),
    mLastTicks(0),
    mNumHits(0),
    mTotalShots(0)
  {
    mProjectiles = new Projectile[NUM_PROJECTILES];
    mTargets = new Target[NUM_TARGETS];
    mPseudoRandom = new PseudoRandom();

    reset();
  }

  State::~State()
  {
    if (mProjectiles != nullptr) {
      delete [] mProjectiles;
      mProjectiles = nullptr;
    }
    if (mTargets != nullptr) {
      delete [] mTargets;
      mTargets = nullptr;
    }
    if (mPseudoRandom != nullptr) {
      delete mPseudoRandom;
      mPseudoRandom = nullptr;
    }
  }

  void State::reset() {
    mLastTicks = SDL_GetTicks();

    mNumHits = 0;
    mTotalShots = 0;

    for (int i = 0; i < NUM_PROJECTILES; i++) {
      mProjectiles[i].active = false;
    }
    for (int i = 0; i < NUM_TARGETS; i++) {
      mTargets[i].active = false;
    }

    mLastSimulationResult = SIMULATION_RESULT_NONE;
  }
}
