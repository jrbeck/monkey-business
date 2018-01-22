#include "Renderer.h"

namespace BananaToss {
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

  v2d_t Renderer::transformToViewport(const v2d_t& a) const {
    v2d_t b;
    b.x = a.x;
    b.y = mWindowHeight - a.y;
    return b;
  }

  void Renderer::drawFrame(State* state) {
    mState = state;

    mPainter->clear(0, 0, 0);
    drawProjectiles();
    drawTargets();
  }

  void Renderer::drawProjectiles() const {
    Projectile* projectile;

    for (int i = 0; i < NUM_PROJECTILES; i++) {
      projectile = &mState->mProjectiles[i];
      if (projectile->active) {
        drawProjectile(projectile);
      }
    }
  }

  void Renderer::drawProjectile(const Projectile* projectile) const {
    v2d_t originationPoint = {{ 16, 16 }};
    v2d_t viewPortCoords = transformToViewport(projectile->pos);
    mPainter->drawLine(transformToViewport(originationPoint), viewPortCoords, 255, 0, 0);
    mPainter->setRgb(viewPortCoords, 255, 255, 255);
  }

  void Renderer::drawTargets() const {
    Target* target;

    for (int i = 0; i < NUM_TARGETS; i++) {
      target = &mState->mTargets[i];
      if (target->active) {
        drawTarget(target);
      }
    }
  }

  void Renderer::drawTarget(const Target* target) const {
    v2d_t corners[4];
    v2d_t halfDimensions;
    halfDimensions.x = 0.5 * target->dimensions.x;
    halfDimensions.y = 0.5 * target->dimensions.y;

    corners[0].x = target->pos.x - halfDimensions.x;
    corners[0].y = target->pos.y - halfDimensions.y;

    corners[1].x = target->pos.x + halfDimensions.x;
    corners[1].y = target->pos.y - halfDimensions.y;

    corners[2].x = target->pos.x + halfDimensions.x;
    corners[2].y = target->pos.y + halfDimensions.y;

    corners[3].x = target->pos.x - halfDimensions.x;
    corners[3].y = target->pos.y + halfDimensions.y;

    mPainter->drawLine(transformToViewport(corners[0]), transformToViewport(corners[1]), 0, 255, 0);
    mPainter->drawLine(transformToViewport(corners[1]), transformToViewport(corners[2]), 0, 255, 0);
    mPainter->drawLine(transformToViewport(corners[2]), transformToViewport(corners[3]), 0, 255, 0);
    mPainter->drawLine(transformToViewport(corners[3]), transformToViewport(corners[0]), 0, 255, 0);
  }
}
