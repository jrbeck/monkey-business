#pragma once

#include <SDL2/SDL.h>

#include "engine/Vec2.h"
#include "engine/PseudoRandom.h"

// enum GAME_STATES {
//   GAME_STATE_WAITING_FOR_CONNECTION,
//   GAME_STATE_WAITING_FOR_INPUT,
//   GAME_STATE_SIMULATING
// };

class MonkeyState {
public:
  MonkeyState();
  ~MonkeyState();

  void reset();

  int mGameState;
  PseudoRandom* mPseudoRandom;
  Uint32 mLastTicks;
};
