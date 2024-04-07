#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string.h>

#include "ScrollingBackground.h"
#include "Entity.h";
#include "Constants.h"
#include "Functions.h"
#include "Player.h"
#include "PlayerArrow.h"
#include "Bullet.h"
#include "Polygon.h"

using std::string;
using std::vector;

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
  void render(SDL_Texture* p_tex, double x, double y, double w = 0, double h = 0, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
  void render(ScrollingBackground &bgr);
  void render(Entity &p_entity, double w = 0, double h = 0);
  void renderTextCenter(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor);

  void renderSkillCooldown(Player &player, int skill_ID);

  void handleEvents();
  void update();
  void renderGameBackground();
  void renderPlayer();
  void gameLoop();
private:
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  SDL_Event gEvent;
  bool gameRunning;
  bool mainMenu;
  bool fullscreen;
};