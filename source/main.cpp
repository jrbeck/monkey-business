#include <cstdio>

#include "engine/SdlApp.h"
#include "MonkeyGame.h"

#define SCREEN_WIDTH (512)
#define SCREEN_HEIGHT (512)

void showIt(MonkeyGame* monkeyGame) {
  SdlApp sdlApp;
  if (sdlApp.init(SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
    printf("ERROR: could not initialize SdlApp\n");
    return;
  }

  Uint32 startTicks = SDL_GetTicks();
  Uint32 frameCount = 0;

  SDL_Event sdlEvent;
  bool quit = false;
  while (!quit) {
    quit = monkeyGame->update();
    monkeyGame->drawFrame();

    sdlApp.drawFrame(monkeyGame->getOutputImageBuffer());
    ++frameCount;

    while (SDL_PollEvent(&sdlEvent)) {
      if (sdlEvent.type == SDL_QUIT) {
        quit = true;
      }
      if (sdlEvent.type == SDL_KEYDOWN) {
        switch (sdlEvent.key.keysym.sym) {
        case SDLK_ESCAPE:
          quit = true;
          break;
        case SDLK_q:
          break;
        case SDLK_a:
          break;
        default:
          break;
        }
      }
      // if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
      //   Uint32 endTicks = SDL_GetTicks();
      //   printf("average FPS: %.3f\n", ((double)(1000 * frameCount) / (double)(endTicks - startTicks)));
      // }
    }
  }

  Uint32 endTicks = SDL_GetTicks();
  printf("average FPS: %.3f\n", ((double)(1000 * frameCount) / (double)(endTicks - startTicks)));

  sdlApp.quit();
}

int main() {
  MonkeyGame* monkeyGame = new MonkeyGame(SCREEN_WIDTH, SCREEN_HEIGHT);
  showIt(monkeyGame);
  delete monkeyGame;

  return 0;
}
