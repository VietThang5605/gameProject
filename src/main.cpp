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
#include "Functions.h"
#include "Constants.h"

Game *game = new Game();
int maxFrameTicks = 0;

void gameLoop() {
  int startTicks = SDL_GetTicks64();

  game->gameLoop();

  int frameTicks = SDL_GetTicks64() - startTicks;
  if (frameTicks > maxFrameTicks)
      maxFrameTicks = frameTicks;
  if (frameTicks < frameDelay)
    SDL_Delay(frameDelay - frameTicks);
}

int main(int argc, char* argv[]) {
  game->init();
  std::cout << "Hello world\n";

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(gameLoop, 0, 1);
#else
  while (game->running()) {
    int startTicks = SDL_GetTicks64();

    game->gameLoop();

    int frameTicks = SDL_GetTicks64() - startTicks;
    if (frameTicks > maxFrameTicks)
      maxFrameTicks = frameTicks;
    if (frameTicks < frameDelay)
      SDL_Delay(frameDelay - frameTicks);
  }
#endif

  std::cout << maxFrameTicks << '\n';
  game->closeMedia();
  game->cleanUp();
  return 0;
}