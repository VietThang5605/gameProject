#include <iostream>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Game.h"
#include "Functions.h"
#include "Constants.h"

int main(int argc, char* argv[]) {
  Game *game = new Game();
  game->init();

  while (game->running()) {
    int startTicks = SDL_GetTicks64();

    game->gameLoop();

    int frameTicks = SDL_GetTicks64() - startTicks;
    if (frameTicks < frameDelay)
      SDL_Delay(frameDelay - frameTicks);
  }

  game->closeMedia();
  game->cleanUp();

  return 0;
}