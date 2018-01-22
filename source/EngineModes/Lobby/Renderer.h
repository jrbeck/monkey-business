#pragma once

#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"
#include "State.h"

namespace Lobby {
  class Renderer {
public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void drawFrame(State* state);

private:
    int mWindowWidth, mWindowHeight;

    ImageBuffer* mOutput;
    Painter* mPainter;

    State* mState;
  };
}
