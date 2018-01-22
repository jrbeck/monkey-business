#include "Renderer.h"

namespace Lobby {
  Renderer::Renderer(int windowWidth, int windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mOutput(nullptr),
    mPainter(nullptr)
  {
    mOutput = new ImageBuffer();
    mOutput->resize(mWindowWidth, mWindowHeight);

    mPainter = new Painter(mOutput);
  }

  Renderer::~Renderer() {
    if (mOutput != nullptr) {
      delete mOutput;
      mOutput = nullptr;
    }
    if (mPainter != nullptr) {
      delete mPainter;
      mPainter = nullptr;
    }
  }

  ImageBuffer* Renderer::getImageBuffer() {
    return mOutput;
  }

  void Renderer::drawFrame(State* state) {
    mState = state;

    mPainter->clear(0, 0, 0);
    mPainter->drawString(0, 0, "Lobby");
  }
}
