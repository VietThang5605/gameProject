#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>

#include "Entity.h"
#include "Constants.h"
#include "Utilities.h"
#include "Player.h"
#include "PlayerArrow.h"
#include "Bullet.h"
#include "Polygon.h"
#include "Button.h"

using std::string;
using std::vector;

struct Game {
public:
  Game();

  void resetGameDelayTime(int time);
  void resetGame();
  
  void init();
  void initSDL();
  void loadMedia();
  void closeMedia();

  void display();
  void clear();
  void cleanUp();
  bool running();

  SDL_Texture* loadTexture(const char* p_filePath);
  SDL_Texture* loadTextureFromText(const char* p_text, TTF_Font* font, SDL_Color textColor);
  SDL_Texture* loadTextureFromText(string& p_text, TTF_Font* font, SDL_Color textColor);
  Mix_Chunk* loadSFX(const char* p_filePath);
  Mix_Music* loadMusic(const char* p_filePath);
  TTF_Font* loadFont(const char* p_filePath, int Size);

  void render(SDL_Texture* p_tex, double x, double y, double w = 0, double h = 0, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
  void renderCenter(SDL_Texture* p_tex, double x, double y, double w = 0, double h = 0, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
  void render(Entity &p_entity, double w = 0, double h = 0);
  void renderText(double p_x, double p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
  void renderText(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor);
  void renderTextCenter(double p_x, double p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
  void renderTextCenter(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor);

  void render_Skill_Hud_And_Cooldown();

  void PlaySkillSFX(int skill_ID);
  void PlaySFX(int sfx_ID);
  void PlayMusic(int music_ID);

  bool isPlayerCollidingBullet(Player p_player, Bullet p_bullet);
  void setArrowToPlayer(bool isMaximizing, Player tmp_player, PlayerArrow &tmp_arrow);
  Bullet makeBullet(int skill_ID, Player &tmp_player, PlayerArrow &tmp_arrow);
  double CalculateArrowAndBulletAccuracy(bool isMaximizing, vector<Player> tmp_player, Entity tmp_arrow);
  double HeuristicEvaluation(vector<Player> tmp_player, vector<PlayerArrow> tmp_arrow, vector<vector<Bullet>> tmp_bullets);
  double Minimax(int depth, bool isMaximizing, double Alpha, double Beta, vector<Player> &tmp_player, vector<PlayerArrow> &tmp_arrow, vector<vector<Bullet>> &tmp_bullets);
  
  void ProcessingAIMove(int player_id);
  void ProcessingSkill(int player_id, int skill_ID);

  void handleEvents();
  void update();

  void render_GameBackground();
  void render_Player();
  void render_Bullet();

  void render_GameEndButtons();
  void render_Credit();
  void render_MainMenu();
  void render_SelectAIMode();
  void render_SelectMode();
  void render_GamePlay();
  void render_GamePause();
  void render_YouWin();
  void render_YouLose();
  void render_Player1Win();
  void render_Player2Win();
  void render_GameDraw();
  void render_Help();
  void render_Game();

  void gameLoop();
private:
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  SDL_Event gEvent;
  bool gameRunning;
  bool fullscreen;
  int gameState;
  int gameDelayTime;
  int gameEndDelayTime;
  int gameTimer;
  int gameAIMode;
  int gameMode;
};