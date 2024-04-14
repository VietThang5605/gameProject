#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <vector>
#include <string.h>

#include "ScrollingBackground.h"
#include "Entity.h"
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
  Mix_Chunk* loadSFX(const char* p_filePath);
  Mix_Music* loadMusic(const char* p_filePath);
  TTF_Font* loadFont(const char* p_filePath, int sz);
  void render(SDL_Texture* p_tex, double x, double y, double w = 0, double h = 0, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
  void render(ScrollingBackground &bgr);
  void render(Entity &p_entity, double w = 0, double h = 0);
  void renderText(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor);
  void renderTextCenter(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor);

  void render_Skill_Hud_And_Cooldown();

  void PlaySFX(int skill_ID);

  void ProcessingSkill(int player_id, int skill_ID);

  void handleEvents();
  void update();
  void render_GameBackground();
  void render_Player();
  void render_MainMenu();
  void render_GamePlay();
  void render_Game();
  void gameLoop();
private:
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  SDL_Event gEvent;
  bool gameRunning;
  bool fullscreen;
  int gameState;
};