#include <iostream>
#include <SDL.h>

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello SDL";

void logErrorAndExit(const char* msg, const char* error) {
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
  SDL_Quit();
}

SDL_Window* InitSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    logErrorAndExit("SDL_Init", SDL_GetError());

  SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//  SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

  if (window == nullptr)
    logErrorAndExit("CreateWindow", SDL_GetError());

  return window;
}

SDL_Renderer* CreateRenderer(SDL_Window* window) {
  ///normal environment
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  ///virtual environment
//  SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

  if (renderer == nullptr)
    logErrorAndExit("CreateRenderer", SDL_GetError());

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "Linear");
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  return renderer;
}

void waitUntilKeyPressed() {
  SDL_Event e;
  while (1) {
    if (SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
      return;
    SDL_Delay(100);
  }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char* argv[]) {
  SDL_Window* window = InitSDL();
  SDL_Renderer* renderer = CreateRenderer(window);



  waitUntilKeyPressed();
  quitSDL(window, renderer);
  return 0;
}
