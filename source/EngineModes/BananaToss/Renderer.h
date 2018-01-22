#pragma once

#include "../../engine/Vec2.h"
#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"
#include "State.h"

namespace BananaToss {
  class Renderer {
public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void drawFrame(State* state);

private:
    v2d_t transformToViewport(const v2d_t& a) const;

    void drawProjectiles() const;
    void drawProjectile(const Projectile* projectile) const;

    void drawTargets() const;
    void drawTarget(const Target* target) const;

    int mWindowWidth, mWindowHeight;

    ImageBuffer* mOutput;
    Painter* mPainter;

    State* mState;
  };
}
