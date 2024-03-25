#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "ScrollingBackground.h"
#include "Entity.h";

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct Game {
public:
  Game();
  
  void init();
  void initSDL();
  void loadMedia();
  void closeMedia();

  void display();
  void clear();
  void cleanUp();
  bool running();

  SDL_Texture* loadTexture(const char* p_filePath);
  void render(SDL_Texture* p_tex, int x, int y, int w = 0, int h = 0, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
  void render(ScrollingBackground &bgr);
  void render(Entity &p_entity, int w = 0, int h = 0);

  void handleEvents();
  void update();
  void renderGameBackground();
  void gameLoop();
private:
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  SDL_Event gEvent;
  bool gameRunning;
  bool mainMenu;
  bool fullscreen;
};