#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <math.h>

#include "Game.h"
#include "Utilities.h"
#include "Constants.h"

Game *game = new Game();

void gameLoop() {
  int startTicks = SDL_GetTicks64();

  game->gameLoop();

  int frameTicks = SDL_GetTicks64() - startTicks;
  if (frameTicks < frameDelay)
    SDL_Delay(frameDelay - frameTicks);
}

int main(int argc, char* argv[]) {
  game->init();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(gameLoop, 0, 1);
#else
  while (game->running()) {
    int startTicks = SDL_GetTicks64();

    game->gameLoop();

    int frameTicks = SDL_GetTicks64() - startTicks;
    if (frameTicks < frameDelay)
      SDL_Delay(frameDelay - frameTicks);
  }
#endif

  game->cleanUp();
  delete game;
  return 0;
}