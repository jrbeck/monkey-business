#include "MonkeyState.h"

MonkeyState::MonkeyState() :
  mPseudoRandom(nullptr),
  mLastTicks(0)
{
  mPseudoRandom = new PseudoRandom();
}

MonkeyState::~MonkeyState()
{
  if (mPseudoRandom != nullptr) {
    delete mPseudoRandom;
    mPseudoRandom = nullptr;
  }
}

void MonkeyState::reset() {
  mLastTicks = SDL_GetTicks();
}
