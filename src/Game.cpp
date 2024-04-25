#include "Game.h"

enum GameState {
  MainMenu,
  GamePlay,
  Help,
  Exit,
  GameState_Total
};

enum KeyPress {
  KEY_PRESS_LEFT, KEY_PRESS_RIGHT, KEY_PRESS_UP, KEY_PRESS_DOWN,
  KEY_PRESS_J, KEY_PRESS_K, KEY_PRESS_L, KEY_PRESS_I, KEY_PRESS_O,
  KEY_PRESS_KP_1, KEY_PRESS_KP_2, KEY_PRESS_KP_3, KEY_PRESS_KP_4, KEY_PRESS_KP_6,

  KEY_PRESS_A, KEY_PRESS_D, KEY_PRESS_W, KEY_PRESS_S,
  KEY_PRESS_C, KEY_PRESS_V, KEY_PRESS_B, KEY_PRESS_F, KEY_PRESS_G,
  KEY_PRESS_P, KEY_PRESS_F11,
  KEY_PRESS_ESC,
  KEY_PRESS_TOTAL
};

const Uint8* keystate = SDL_GetKeyboardState(NULL);

int old_key[KEY_PRESS_TOTAL];
int new_key[KEY_PRESS_TOTAL];

int MouseX, MouseY;
bool mouseClicked;

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

TTF_Font* font32;
TTF_Font* font32_outline;
TTF_Font* font80;

Mix_Chunk* SFX[sfx_ID_Total] = { NULL };

SDL_Texture* waterTexture = NULL;
SDL_Texture* grassTexture = NULL;

SDL_Texture* ezrealTexture = NULL;

SDL_Texture* skill_Hud[skill_ID_Total] = { NULL };
SDL_Texture* skillArrow = NULL;
SDL_Texture* skillQ = NULL;
SDL_Texture* skillW = NULL;
SDL_Texture* skillW_ground = NULL;
SDL_Texture* skillR = NULL;

SDL_Texture* Background[GameState_Total] = { NULL };

SDL_Texture* Button_Texture[Button_Total][2] = { NULL };

vector<Entity> waterBackground;

Entity player_skill_Hud[3][skill_ID_Total];
Entity skillW_effect;

Button button[Button_Total];

PlayerArrow player_Arrow[3];
Player player[3];

Bullet bullet[skill_ID_Total];
vector<Bullet> player_bullets[3];

Game::Game() {
  gWindow = NULL;
  gRenderer = NULL;
  gameRunning = true;
  gameState = MainMenu;
  fullscreen = false;
};

void Game::resetGame() {
  for (int id = 1; id <= 2; id++) {
    player_Arrow[id].reset();
    player_bullets[id].erase(player_bullets[id].begin(), player_bullets[id].end());
  }
}

void Game::init() {
  initSDL();
  loadMedia();
}

void Game::initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING & ~(SDL_INIT_TIMER | SDL_INIT_HAPTIC))!= 0)
    logError("Failed to initialize SDL.", SDL_GetError());

  // gWindow = SDL_CreateWindow("Ezreal mirror shooting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  // gWindow = SDL_CreateWindow("Ezreal mirror shooting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE);
  gWindow = SDL_CreateWindow("Ezreal mirror shooting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (gWindow == NULL)
    logError("Failed to create window.", SDL_GetError());
  
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == NULL)
    logError("Failted to create renderer.", SDL_GetError());
  
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  // SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");
  SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
    logError("Failed to initialize SDL_image.", IMG_GetError());
  
  if (TTF_Init() != 0)
    logError("Failed to initialize SDL_ttf.", TTF_GetError());
  
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    logError("Failed to initialize SDL_mixer.", Mix_GetError());
  
  Mix_Volume(-1, 50);
}

void Game::loadMedia() {
  ///Fonts
  font32 = loadFont("res/fonts/cocogoose.ttf", 32);
  font32_outline = loadFont("res/fonts/cocogoose.ttf", 32);
  TTF_SetFontOutline(font32_outline, 1);

  font80 = loadFont("res/fonts/DTM-Sans.ttf", 80);

  ///Textures
  waterTexture = loadTexture("res/images/Water.png");
  grassTexture = loadTexture("res/images/Grass.png");
  ezrealTexture = loadTexture("res/images/ezreal-Sheet.png");

  skill_Hud[skillQ_ID] = loadTexture("res/images/ezreal_q.png");
  skill_Hud[skillW_ID] = loadTexture("res/images/ezreal_w.png");
  skill_Hud[skillE_ID] = loadTexture("res/images/ezreal_e.png");
  skill_Hud[skillR_ID] = loadTexture("res/images/ezreal_r.png");

  skillArrow = loadTexture("res/images/skillArrow.png");
  skillQ = loadTexture("res/images/skillQ.png");
  skillW = loadTexture("res/images/skillW.png");
  skillW_ground = loadTexture("res/images/skillW_ground.png");
  skillR = loadTexture("res/images/skillR.png");

  Background[MainMenu] = loadTexture("res/images/MainMenu_background.png");
  Background[Help] = loadTexture("res/images/Help_background.png");

  Button_Texture[VSAI_Button][0] = loadTextureFromText("VS AI", font80, black);
  Button_Texture[VSAI_Button][1] = loadTextureFromText("VS AI", font80, white);
  button[VSAI_Button].init(Button_Texture[VSAI_Button][0], Button_Texture[VSAI_Button][1]);
  button[VSAI_Button].setX(SCREEN_WIDTH / 2 - button[VSAI_Button].getWidth() / 2);
  button[VSAI_Button].setY(SCREEN_HEIGHT / 2 - button[VSAI_Button].getHeight() - 100);

  Button_Texture[VSPlayer_Button][0] = loadTextureFromText("VS Player", font80, black);
  Button_Texture[VSPlayer_Button][1] = loadTextureFromText("VS Player", font80, white);
  button[VSPlayer_Button].init(Button_Texture[VSPlayer_Button][0], Button_Texture[VSPlayer_Button][1]);
  button[VSPlayer_Button].setX(SCREEN_WIDTH / 2 - button[VSPlayer_Button].getWidth() / 2);
  button[VSPlayer_Button].setY(button[VSAI_Button].getY() + button[VSAI_Button].getHeight() + 10);

  Button_Texture[Help_Button][0] = loadTextureFromText("Help", font80, black);
  Button_Texture[Help_Button][1] = loadTextureFromText("Help", font80, white);
  button[Help_Button].init(Button_Texture[Help_Button][0], Button_Texture[Help_Button][1]);
  button[Help_Button].setX(SCREEN_WIDTH / 2 - button[Help_Button].getWidth() / 2);
  button[Help_Button].setY(button[VSPlayer_Button].getY() + button[VSPlayer_Button].getHeight() + 10);

  Button_Texture[Exit_Button][0] = loadTextureFromText("Exit", font80, black);
  Button_Texture[Exit_Button][1] = loadTextureFromText("Exit", font80, white);
  button[Exit_Button].init(Button_Texture[Exit_Button][0], Button_Texture[Exit_Button][1]);
  button[Exit_Button].setX(SCREEN_WIDTH / 2 - button[Exit_Button].getWidth() / 2);
  button[Exit_Button].setY(button[Help_Button].getY() + button[Help_Button].getHeight() + 10);

  ///Sounds
  SFX[Q1_sfx_ID] = loadSFX("res/audio/sfx/Q1.wav");
  SFX[Q2_sfx_ID] = loadSFX("res/audio/sfx/Q2.wav");
  SFX[Q_hit_sfx_ID] = loadSFX("res/audio/sfx/Q_hit.wav");
  SFX[W_sfx_ID] = loadSFX("res/audio/sfx/W.wav");
  SFX[W_hit_sfx_ID] = loadSFX("res/audio/sfx/W_hit.wav");
  SFX[W_hit_crashed_sfx_ID] = loadSFX("res/audio/sfx/W_hit_crashed.wav");
  SFX[E_sfx_ID] = loadSFX("res/audio/sfx/E.wav");
  SFX[R1_sfx_ID] = loadSFX("res/audio/sfx/R1_sfx.wav");
  SFX[R2_sfx_ID] = loadSFX("res/audio/sfx/R2_sfx.wav");

  ///entity's initialization
  skillW_effect.init(skillW_ground, 128, 64, 1, 1);
  bullet[skillQ_ID].init(skillQ, 4 * 9, 16 * 9, 1, 1, skillQ_ID);
  bullet[skillW_ID].init(skillW, 8 * 14, 8 * 14, 1, 1, skillW_ID);
  bullet[skillR_ID].init(skillR, 224, 224, 1, 1, skillR_ID);

  for (int j = 20; j < 45; j++)
    for (int i = 0; i < 32; i++) {
      Entity entity;
      entity.init(waterTexture, 64, 16, 1, 1);
      entity.setX(i * 64);
      entity.setY(j * 16);
      waterBackground.push_back(entity);
    }
  
  ///player1's initialization
  player[1].init(ezrealTexture, 54 * 3, 69 * 3, 8, 8);
  player[1].setStartXY(SCREEN_WIDTH / 2 - player[1].getWidth() / 2, 150);
  player[1].reset(Human);

  player_Arrow[1].init(skillArrow, 8 * 10, 16 * 10, 1, 1);
  player_Arrow[1].setCenter(player_Arrow[1].getWidth() / 2, 0);
  player_Arrow[1].setAngle(0);
  player_Arrow[1].setX(player[1].getX() + player[1].getWidth() / 2 - player_Arrow[1].getWidth() / 2);
  player_Arrow[1].setY(player[1].getY() + player[1].getHeight() + 10);
  player_Arrow[1].setRotPoint(player_Arrow[1].getX() + player_Arrow[1].getWidth() / 2, player_Arrow[1].getY());

  player_skill_Hud[1][skillR_ID].init(skill_Hud[skillR_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillR_ID].setX(SCREEN_WIDTH - 30 - player_skill_Hud[1][skillR_ID].getWidth());
  player_skill_Hud[1][skillR_ID].setY(120 - player_skill_Hud[1][skillR_ID].getHeight());

  player_skill_Hud[1][skillE_ID].init(skill_Hud[skillE_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillE_ID].setX(player_skill_Hud[1][skillR_ID].getX() - player_skill_Hud[1][skillE_ID].getWidth() - 30);
  player_skill_Hud[1][skillE_ID].setY(120 - player_skill_Hud[1][skillE_ID].getHeight());

  player_skill_Hud[1][skillW_ID].init(skill_Hud[skillW_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillW_ID].setX(player_skill_Hud[1][skillE_ID].getX() - player_skill_Hud[1][skillW_ID].getWidth() - 30);
  player_skill_Hud[1][skillW_ID].setY(120 - player_skill_Hud[1][skillW_ID].getHeight());

  player_skill_Hud[1][skillQ_ID].init(skill_Hud[skillQ_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillQ_ID].setX(player_skill_Hud[1][skillW_ID].getX() - player_skill_Hud[1][skillQ_ID].getWidth() - 30);
  player_skill_Hud[1][skillQ_ID].setY(120 - player_skill_Hud[1][skillQ_ID].getHeight());
  
  ///player2's initialization
  player[2].init(ezrealTexture, 54 * 3, 69 * 3, 8, 8);
  player[2].setStartXY(SCREEN_WIDTH / 2 - player[2].getWidth() / 2, 720);
  player[2].reset(Human);

  player_Arrow[2].init(skillArrow, 8 * 10, 16 * 10, 1, 1);
  player_Arrow[2].setCenter(player_Arrow[2].getWidth() / 2, player_Arrow[2].getHeight());
  player_Arrow[2].setFlip(SDL_FLIP_VERTICAL);
  player_Arrow[2].setAngle(0);
  player_Arrow[2].setX(player[2].getX() + player[2].getWidth() / 2 - player_Arrow[2].getWidth() / 2);
  player_Arrow[2].setY(player[2].getY() - player_Arrow[2].getHeight() - 10);
  player_Arrow[2].setRotPoint(player_Arrow[2].getX() + player_Arrow[2].getWidth() / 2, player_Arrow[2].getY() + player_Arrow[2].getHeight());

  player_skill_Hud[2][skillQ_ID].init(skill_Hud[skillQ_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillQ_ID].setX(30);
  player_skill_Hud[2][skillQ_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillW_ID].init(skill_Hud[skillW_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillW_ID].setX(player_skill_Hud[2][skillQ_ID].getX() + player_skill_Hud[2][skillQ_ID].getWidth() + 30);
  player_skill_Hud[2][skillW_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillE_ID].init(skill_Hud[skillE_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillE_ID].setX(player_skill_Hud[2][skillW_ID].getX() + player_skill_Hud[2][skillW_ID].getWidth() + 30);
  player_skill_Hud[2][skillE_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillR_ID].init(skill_Hud[skillR_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillR_ID].setX(player_skill_Hud[2][skillE_ID].getX() + player_skill_Hud[2][skillE_ID].getWidth() + 30);
  player_skill_Hud[2][skillR_ID].setY(SCREEN_HEIGHT - 120);
}

void Game::closeMedia() {
  SDL_DestroyTexture(waterTexture); waterTexture = NULL;
  SDL_DestroyTexture(grassTexture); grassTexture = NULL;
  SDL_DestroyTexture(ezrealTexture); ezrealTexture = NULL;

  SDL_DestroyTexture(skillArrow); skillArrow = NULL;
  SDL_DestroyTexture(skillQ); skillQ = NULL;
  SDL_DestroyTexture(skillW); skillW = NULL;
  SDL_DestroyTexture(skillW_ground); skillW_ground = NULL;
  SDL_DestroyTexture(skillR); skillR = NULL;

  for (int id = 0; id < GameState_Total; id++) {
    SDL_DestroyTexture(Background[id]);
    Background[id] = NULL;
  }

  for (int id = 0; id < Button_Total; id++) {
    for (int state = 0; state <= 1; state++) {
      SDL_DestroyTexture(Button_Texture[id][state]);
      Button_Texture[id][state] = NULL;
    }
  }

  TTF_CloseFont(font32);
  TTF_CloseFont(font32_outline);
  TTF_CloseFont(font80);

  for (int id = 0; id < skill_ID_Total; id++) {
    SDL_DestroyTexture(skill_Hud[id]);
    skill_Hud[id] = NULL;
  }

  for (int id = 0; id < sfx_ID_Total; id++) {
    Mix_FreeChunk(SFX[id]);
    SFX[id] = NULL;
  }
}

void Game::display() {
  SDL_RenderPresent(gRenderer);
}

void Game::clear() {
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
  SDL_RenderClear(gRenderer);
}

void Game::cleanUp() {
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
  Mix_CloseAudio();
}

bool Game::running() {
  return gameRunning;
}

SDL_Texture* Game::loadTexture(const char* p_filePath) {
  SDL_Texture* texture = NULL;
  texture = IMG_LoadTexture(gRenderer, p_filePath);

  if (texture == NULL)
    logError("Failed to load texture.", IMG_GetError());

  return texture;
}

SDL_Texture* Game::loadTextureFromText(const char* p_text, TTF_Font* font, SDL_Color textColor) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = NULL;
  message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);
  
  if (message == NULL)
    logError("Failed to create texture from surface.", SDL_GetError());

  SDL_FreeSurface(surfaceMessage);
  return message;
}

Mix_Chunk* Game::loadSFX(const char* p_filePath) {
  Mix_Chunk* sound = NULL;
  sound = Mix_LoadWAV(p_filePath);

  if (sound == NULL)
    logError("Failed to load sound effct with path:", p_filePath);

  return sound;
}

Mix_Music* Game::loadMusic(const char* p_filePath) {
  Mix_Music* music = NULL;
  music = Mix_LoadMUS(p_filePath);

  if (music == NULL)
    logError("Failed to load music.", Mix_GetError());

  return music;
}

TTF_Font* Game::loadFont(const char* p_filePath, int sz) {
  TTF_Font* font = NULL;
  font = TTF_OpenFont(p_filePath, sz);

  if (font == NULL)
    logError("Failed to load font.", TTF_GetError());

  return font;
}

void Game::render(SDL_Texture* p_tex, double x, double y, double w, double h, double angle, SDL_Point* center, SDL_RendererFlip flip) {
  SDL_Rect src;
  src.x = src.y = 0;
  SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  if (w != 0 && h != 0) {
    dst.w = w;
    dst.h = h;
  }
  else {
    dst.w = src.w;
    dst.h = src.h;
  }
  SDL_RenderCopyEx(gRenderer, p_tex, &src, &dst, angle, center, flip);
}

void Game::render(Entity &p_entity, double w, double h) {
  SDL_Rect dst;
  dst.x = p_entity.getX();
  dst.y = p_entity.getY();
  if (w != 0 && h != 0) {
    dst.w = w;
    dst.h = h;
  }
  else {
    dst.w = p_entity.getWidth();
    dst.h = p_entity.getHeight();
  }
  SDL_RenderCopyEx(gRenderer, p_entity.getTexture(), p_entity.getCurrentClip(), &dst, p_entity.getAngle(), p_entity.getCenter(), p_entity.getFlip());
}

void Game::renderText(double p_x, double p_y, const char* p_text, TTF_Font* font, SDL_Color textColor) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x;
  dst.y = p_y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::renderText(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor) {
  char* text = &p_text[0];
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x;
  dst.y = p_y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::renderTextCenter(double p_x, double p_y, const char* p_text, TTF_Font* font, SDL_Color textColor) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x - src.w / 2;
  dst.y = p_y - src.h / 2;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::renderTextCenter(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor) {
  char* text = &p_text[0];
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x - src.w / 2;
  dst.y = p_y - src.h / 2;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::render_Skill_Hud_And_Cooldown() {
  {
    string health;
    if (player[1].getHealth() > 0)
      health = "Health: " + IntToString(player[1].getHealth());
    else
      health = "Health: !!";
    renderText(0, 0, health, font32, white);
    renderText(0, 0, health, font32_outline, black);
    if (player[2].getHealth() > 0)
      health = "Health: " + IntToString(player[2].getHealth());
    else
      health = "Health: !!";
    renderText(SCREEN_WIDTH - 190, SCREEN_HEIGHT - 40, health, font32, white);
    renderText(SCREEN_WIDTH - 190, SCREEN_HEIGHT - 40, health, font32_outline, black);
  }
  for (int id = 1; id <= 2; id++)
    for (int skill_id = 0; skill_id < skill_ID_Total; skill_id++) {
      render(player_skill_Hud[id][skill_id]);

      if (player[id].getSkillCooldown(skill_id) > 0) {
        std::string text = TimeToString(player[id].getSkillCooldown(skill_id) / (FPS * 1.0));
        renderTextCenter(player_skill_Hud[id][skill_id].getX() + player_skill_Hud[id][skill_id].getWidth() / 2,
                        player_skill_Hud[id][skill_id].getY() + player_skill_Hud[id][skill_id].getHeight() / 2,
                        text,
                        font32,
                        white);
        renderTextCenter(player_skill_Hud[id][skill_id].getX() + player_skill_Hud[id][skill_id].getWidth() / 2,
                        player_skill_Hud[id][skill_id].getY() + player_skill_Hud[id][skill_id].getHeight() / 2,
                        text,
                        font32_outline,
                        black);
      }
    }
}

void Game::PlaySFX(int skill_ID) {
  switch (skill_ID) {
    case (skillQ_ID): {
      Mix_PlayChannel(-1, SFX[Q1_sfx_ID], 0);
      Mix_PlayChannel(-1, SFX[Q2_sfx_ID], 0);
      break;
    }
    case (skillW_ID): {
      Mix_PlayChannel(-1, SFX[W_sfx_ID], 0);
      break;
    }
    case (skillE_ID): {
      Mix_PlayChannel(-1, SFX[E_sfx_ID], 0);
      break;
    }
    case (skillR_ID): {
      Mix_PlayChannel(-1, SFX[R1_sfx_ID], 0);
      break;
    }
  }
}

void Game::setArrowToPlayer(bool isMaximizing, Player tmp_player, PlayerArrow &tmp_arrow) {
  if (isMaximizing == Human) {
    tmp_arrow.setX(tmp_player.getX() + tmp_player.getWidth() / 2 - tmp_arrow.getWidth() / 2);
    tmp_arrow.setY(tmp_player.getY() - tmp_arrow.getHeight() - 10);
    tmp_arrow.setRotPoint(tmp_arrow.getX() + tmp_arrow.getWidth() / 2, tmp_arrow.getY() + tmp_arrow.getHeight());
  }
  else {
    tmp_arrow.setX(tmp_player.getX() + tmp_player.getWidth() / 2 - tmp_arrow.getWidth() / 2);
    tmp_arrow.setY(tmp_player.getY() + tmp_player.getHeight() + 10);
    tmp_arrow.setRotPoint(tmp_arrow.getX() + tmp_arrow.getWidth() / 2, tmp_arrow.getY());
  }
}

double Game::CalculateArrowAccuracy(bool isMaximizing, vector<Player> tmp_player, vector<PlayerArrow> tmp_arrow) {
  int player_id = isMaximizing;
  int enemy_id = isMaximizing ^ 1;
  
  Point arrow;
  if (player_id == Human)
    arrow = rotatePointAroundAngle(tmp_arrow[player_id].getX() + tmp_arrow[player_id].getWidth() / 2, tmp_arrow[player_id].getY(), tmp_arrow[player_id].getRotPoint(), tmp_arrow[player_id].getAngle());
  else
    arrow = rotatePointAroundAngle(tmp_arrow[player_id].getX() + tmp_arrow[player_id].getWidth() / 2, tmp_arrow[player_id].getY() + tmp_arrow[player_id].getHeight(), tmp_arrow[player_id].getRotPoint(), tmp_arrow[player_id].getAngle());

  Point rot_point = Point(tmp_arrow[player_id].getRotPoint()->x, tmp_arrow[player_id].getRotPoint()->y);
  Vector vec;
  if (player_id == Human)
    vec = Vector(sin(PI / 2 + tmp_arrow[player_id].getAngle() * PI / 180), -cos(PI / 2 + tmp_arrow[player_id].getAngle() * PI / 180));
  else
    vec = Vector(sin(-PI / 2 + tmp_arrow[player_id].getAngle() * PI / 180), -cos(-PI / 2 + tmp_arrow[player_id].getAngle() * PI / 180));
  
  int Enemy_Y_Current;
  if (player_id == Human) 
    Enemy_Y_Current = tmp_player[enemy_id].getY() + tmp_player[enemy_id].getWidth();
  else
    Enemy_Y_Current = tmp_player[enemy_id].getY();
    
  Point arrow_y_tmp((vec.x * arrow.x - vec.y * Enemy_Y_Current + vec.y * arrow.y) / vec.x, Enemy_Y_Current);

  // if (arrow_y_tmp.x < ScreenLeftBoundary)
  //   arrow_y_tmp.x = ScreenLeftBoundary;
  // if (arrow_y_tmp.x > ScreenRightBoundary)
  //   arrow_y_tmp.x = ScreenRightBoundary;

  // SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
  // SDL_RenderDrawLine(gRenderer, (int)arrow_y_tmp.x, (int)arrow_y_tmp.y, (int) rot_point.x, (int)rot_point.y);
  // SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);

  return abs(arrow_y_tmp.x - (tmp_player[enemy_id].getX() + tmp_player[enemy_id].getWidth() / 2));
}

void Game::makeBullet(bool isMaximizing, Player tmp_player, PlayerArrow tmp_arrow, vector<Bullet> &tmp_bullets) {

}

double Game::HeuristicEvaluation(vector<Player> tmp_player, vector<PlayerArrow> tmp_arrow, vector<vector<Bullet>> tmp_bullets) {
  vector<double> ArrowAccuracy(2);
  double Score = 0;
  for (int id = 0; id <= 1; id++) {
    ArrowAccuracy[id] = CalculateArrowAccuracy(id, tmp_player, tmp_arrow);
    if (id == 0) {
      if (tmp_player[0].getSkillCooldown(skillQ_ID) == 0)
        Score += ArrowAccuracy[id];
      // if (ArrowAccuracy[id] > 200) {
      //   Score -= ArrowAccuracy[id];
      //   Score -= abs(tmp_player[0].getX() - Enemy.getX());
      // }
    }
    if (id == 1) {
      if (tmp_player[0].getSkillCooldown(skillQ_ID) > 0)
        Score -= ArrowAccuracy[id];
    }
  }

  if (tmp_bullets[0].empty() == 0) {
    if (ArrowAccuracy[0] < 100)
      Score -= 100;
  }

  return Score;
}

double Game::Minimax(int depth, bool isMaximizing, double Alpha, double Beta, vector<Player> &tmp_player, vector<PlayerArrow> &tmp_arrow, vector<vector<Bullet>> &tmp_bullets) {
  if (depth == 0) return HeuristicEvaluation(tmp_player, tmp_arrow, tmp_bullets);

  int player_id = isMaximizing;

  double bestScore = HeuristicEvaluation(tmp_player, tmp_arrow, tmp_bullets);
  // double bestScore;
  // if (isMaximizing)
  //   bestScore = -DOUBLE_INF;
  // else
  //   bestScore = DOUBLE_INF;

  for (int move_type = 0; move_type < AI_Move_Type_Total; move_type++) { ///***cast time
    switch (move_type) {
      case (Do_Nothing): {
        if (isMaximizing) {
          bestScore = max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = max(Alpha, bestScore);
        }
        else { 
          bestScore = min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = min(Beta, bestScore);
        }

        // cout << "minimax do nothing: " << Minimax(depth - 1, isMaximizing ^ 1, tmp_player, tmp_arrow, tmp_bullets) << '\n';

        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Move_Left): {
        int x = tmp_player[player_id].getX(), y = tmp_player[player_id].getY();
        tmp_player[player_id].moveLeft();
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);

        if (isMaximizing) { 
          bestScore = max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = max(Alpha, bestScore);
        }
        else { 
          bestScore = min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = min(Beta, bestScore);
        }

        // cout << "minimax move left: " << Minimax(depth - 1, isMaximizing ^ 1, tmp_player, tmp_arrow, tmp_bullets) << '\n';
        tmp_player[player_id].setX(x); tmp_player[player_id].setY(y);
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);

        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Move_Right): {
        int x = tmp_player[player_id].getX(), y = tmp_player[player_id].getY();
        tmp_player[player_id].moveRight();
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);

        if (isMaximizing) {
          bestScore = max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = max(Alpha, bestScore);
        }
        else { 
          bestScore = min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = min(Beta, bestScore);
        }

        // cout << "minimax do move right: " << Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets) << '\n';
        tmp_player[player_id].setX(x); tmp_player[player_id].setY(y);
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);

        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_Q): {
        if (tmp_player[player_id].getSkillCooldown(skillQ_ID) > 0)
          break;
        
        Bullet tmp;
        tmp.setX(tmp_player[0].getX() + tmp_player[0].getWidth() / 2 - tmp.getWidth() / 2);
        tmp.setY(tmp_player[0].getY() + tmp_player[0].getHeight());
        tmp.setAngle(tmp_arrow[0].getAngle());
        tmp.setCenter(tmp.getWidth() / 2, 0);
        tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY()); ///reset after each frame in void move();
        
        tmp_bullets[0].push_back(tmp);
        if (isMaximizing) {
          bestScore = max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = max(Alpha, bestScore);
        }
        else { 
          bestScore = min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = min(Beta, bestScore);
        }

        tmp_bullets[0].pop_back();

        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_W): {
        if (tmp_player[player_id].getSkillCooldown(skillW_ID) > 0)
          break;
        
        Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets);

        if (Beta <= Alpha)
          return bestScore;
        break;
      }
    }
  }

  // cout << "minimax bestScore: " << bestScore << '\n';

  return bestScore;
}

void Game::ProcessingAIMove(int player_id) {
  if (player[player_id].isDead() || player[3  - player_id].isDead())
    return;

  vector<Player> tmp_player(2);
  tmp_player[0] = player[player_id];
  tmp_player[1] = player[3 - player_id];

  vector<PlayerArrow> tmp_arrow(2);
  tmp_arrow[0] = player_Arrow[player_id];
  tmp_arrow[1] = player_Arrow[3 - player_id];

  vector<vector<Bullet>> tmp_bullets(3);
  tmp_bullets[0] = player_bullets[player_id];
  tmp_bullets[1] = player_bullets[3 - player_id];

  double bestScore = DOUBLE_INF;
  int bestMove = -1;
  int depth = 4;

  for (int move_type = 0; move_type < AI_Move_Type_Total; move_type++) {
    switch (move_type) {
      case (Do_Nothing): {
        double score = Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets);
        if (score < bestScore) {
          bestScore = score;
          bestMove = move_type;
        }
        // cout << "do nothing: " << score << '\n';
        
        break;
      }
      case (Move_Left): {
        int x = tmp_player[0].getX(), y = tmp_player[0].getY();
        tmp_player[0].moveLeft();
        setArrowToPlayer(0, tmp_player[0], tmp_arrow[0]);

        double score = Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets);
        if (score < bestScore) {
          bestScore = score;
          bestMove = move_type;
        }
        // cout << "move left: " << score << '\n';

        tmp_player[0].setX(x); tmp_player[0].setY(y);
        setArrowToPlayer(0, tmp_player[0], tmp_arrow[0]);
        break;
      }
      case (Move_Right): {
        int x = tmp_player[0].getX(), y = tmp_player[0].getY();
        tmp_player[0].moveRight();
        setArrowToPlayer(0, tmp_player[0], tmp_arrow[0]);

        double score = Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets);
        if (score < bestScore) {
          bestScore = score;
          bestMove = move_type;
        }
        // cout << "move right: " << score << '\n';

        tmp_player[0].setX(x); tmp_player[0].setY(y);
        setArrowToPlayer(0, tmp_player[0], tmp_arrow[0]);
        break;
      }
      case (Use_Q): {
        if (tmp_player[0].getSkillCooldown(skillQ_ID) > 0)
          break;

        Bullet tmp;
        tmp.setX(tmp_player[0].getX() + tmp_player[0].getWidth() / 2 - tmp.getWidth() / 2);
        tmp.setY(tmp_player[0].getY() + tmp_player[0].getHeight());
        tmp.setAngle(tmp_arrow[0].getAngle());
        tmp.setCenter(tmp.getWidth() / 2, 0);
        tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY()); ///reset after each frame in void move();
        
        tmp_bullets[0].push_back(tmp);
        double score = Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets);
        if (score < bestScore) {
          bestScore = score;
          bestMove = move_type;
        }

        tmp_bullets[0].pop_back();
        break;
      }
      case (Use_W): {
        if (tmp_player[0].getSkillCooldown(skillW_ID) > 0)
          break;
        
        Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets);

        break;
      }
    }
  }

  // cout << bestScore << '\n';
  // cout << '\n';

  switch (bestMove) {
    case (Do_Nothing): {
      player[player_id].setCurrentFrame(0);
      break;
    }
    case (Move_Left): {
      player[player_id].moveLeft();
      break;
    }
    case (Move_Right): {
      player[player_id].moveRight();
      break;
    }
    case (Use_Q): {
      ProcessingSkill(player_id, skillQ_ID);
      break;
    }
  }
}

void Game::ProcessingSkill(int player_id, int skill_ID) {
  if (player[player_id].getSkillCooldown(skill_ID) > 0)
    return;

  player[player_id].setCastTimeCooldown(skill_castTime[skill_ID]);
  player[player_id].setSkillCooldown(skill_Cooldown[skill_ID], skill_ID);
  PlaySFX(skill_ID);

  switch (skill_ID) {
    case (skillE_ID): {

      break;
    }
    default: {
      Bullet tmp = bullet[skill_ID];
      if (player_id == 1) {
        tmp.setX(player[1].getX() + player[1].getWidth() / 2 - tmp.getWidth() / 2);
        tmp.setY(player[1].getY() + player[1].getHeight());
        tmp.setAngle(player_Arrow[1].getAngle());
        tmp.setCenter(tmp.getWidth() / 2, 0);
        tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY()); ///reset after each frame in void move();
      }
      else if (player_id == 2) {
        tmp.setX(player[player_id].getX() + player[player_id].getWidth() / 2 - tmp.getWidth() / 2);
        tmp.setY(player[player_id].getY() - tmp.getHeight());
        tmp.setAngle(player_Arrow[player_id].getAngle());
        tmp.setCenter(tmp.getWidth() / 2, tmp.getHeight());
        tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY() + tmp.getHeight()); ///reset after each frame in void move();
        tmp.setFlip(SDL_FLIP_VERTICAL);

        tmp.setVelocityX(-tmp.getVelocityX());
        tmp.setVelocityY(-tmp.getVelocityY());
      }
      player_bullets[player_id].push_back(tmp);
      if (skill_ID == skillR_ID)
        player[player_id].setSkillDelay(FPS);
    }
  }
}

void Game::handleEvents() {
  ///player1
  if (player[1].getType() == Human) {
    if (keystate[SDL_SCANCODE_LEFT]) new_key[KEY_PRESS_LEFT] = 1;
    if (keystate[SDL_SCANCODE_RIGHT]) new_key[KEY_PRESS_RIGHT] = 1;
    if (keystate[SDL_SCANCODE_UP]) new_key[KEY_PRESS_UP] = 1;
    if (keystate[SDL_SCANCODE_DOWN]) new_key[KEY_PRESS_DOWN] = 1;
    if (keystate[SDL_SCANCODE_J]) new_key[KEY_PRESS_J] = 1;
    if (keystate[SDL_SCANCODE_K]) new_key[KEY_PRESS_K] = 1;
    if (keystate[SDL_SCANCODE_L]) new_key[KEY_PRESS_L] = 1;
    if (keystate[SDL_SCANCODE_I]) new_key[KEY_PRESS_I] = 1;
    if (keystate[SDL_SCANCODE_O]) new_key[KEY_PRESS_O] = 1;
  }

  ///player2
  if (player[2].getType() == Human) {
    if (keystate[SDL_SCANCODE_A]) new_key[KEY_PRESS_A] = 1;
    if (keystate[SDL_SCANCODE_D]) new_key[KEY_PRESS_D] = 1;
    if (keystate[SDL_SCANCODE_W]) new_key[KEY_PRESS_W] = 1;
    if (keystate[SDL_SCANCODE_S]) new_key[KEY_PRESS_S] = 1;
    if (keystate[SDL_SCANCODE_C]) new_key[KEY_PRESS_C] = 1;
    if (keystate[SDL_SCANCODE_V]) new_key[KEY_PRESS_V] = 1;
    if (keystate[SDL_SCANCODE_B]) new_key[KEY_PRESS_B] = 1;
    if (keystate[SDL_SCANCODE_F]) new_key[KEY_PRESS_F] = 1;
    if (keystate[SDL_SCANCODE_G]) new_key[KEY_PRESS_G] = 1;
  }

  if (new_key[KEY_PRESS_LEFT] && new_key[KEY_PRESS_RIGHT]) {
    new_key[KEY_PRESS_LEFT] = new_key[KEY_PRESS_RIGHT] = 0;
  }
  if (new_key[KEY_PRESS_A] && new_key[KEY_PRESS_D]) {
    new_key[KEY_PRESS_A] = new_key[KEY_PRESS_D] = 0;
  }

  ///utility
  if (keystate[SDL_SCANCODE_P]) new_key[KEY_PRESS_P] = 1;
  if (keystate[SDL_SCANCODE_F11]) new_key[KEY_PRESS_F11] = 1;
  if (keystate[SDL_SCANCODE_ESCAPE]) new_key[KEY_PRESS_ESC] = 1;

  int width, height;
  SDL_GetWindowSize(gWindow, &width, &height);
  SDL_GetMouseState(&MouseX, &MouseY);
  // cout << width << ' ' << height << ' ' << MouseX << ' ' << MouseY << '\n';
  MouseX = (int)((double)SCREEN_WIDTH / width * MouseX);
  MouseY = (int)((double)SCREEN_HEIGHT / height * MouseY);
  // cout << width << ' ' << height << ' ' << MouseX << ' ' << MouseY << '\n';
  // cout << '\n';
  mouseClicked = false;

  while (SDL_PollEvent(&gEvent)) {
    if (gEvent.type == SDL_QUIT) {
      gameRunning = false;
      break;
    }
    if (gEvent.type == SDL_MOUSEBUTTONUP && gEvent.button.button == SDL_BUTTON_LEFT) {
      mouseClicked = true;
    }
  }
}

void Game::update() {
  ///Function keys
  // if (new_key[KEY_PRESS_F11] && old_key[KEY_PRESS_F11] == 0) {
  //   if (fullscreen == false) {
  //     SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
  //     fullscreen = true;
  //   }
  //   else {
  //     SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_SHOWN);
  //     fullscreen = false;
  //   }
  // }
  
  // if (new_key[KEY_PRESS_ESC] && old_key[KEY_PRESS_ESC] == 0) {
  //   if (fullscreen) {
  //     SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_SHOWN);
  //     fullscreen = false;
  //   }
  // }

  if (new_key[KEY_PRESS_P] && old_key[KEY_PRESS_P] == 0) {
    
  }

  if (gameState == GamePlay) {
    ///player1
    if (player[1].isDead() == 0) {
      player[1].updatePlayerEffects();
      player[1].updateCooldown();
      if (player[1].getType() == Bot) ProcessingAIMove(1);
      else {
        if (player[1].getCastTimeCooldown() == 0) {
          if (new_key[KEY_PRESS_J] && old_key[KEY_PRESS_J] == 0)
            ProcessingSkill(1, skillQ_ID);

          if (new_key[KEY_PRESS_K] && old_key[KEY_PRESS_K] == 0)
            ProcessingSkill(1, skillW_ID);
          
          if ((new_key[KEY_PRESS_LEFT] || new_key[KEY_PRESS_RIGHT])
              && new_key[KEY_PRESS_L] && old_key[KEY_PRESS_L] == 0 && player[1].getSkillCooldown(skillE_ID) == 0) 
          {
            ProcessingSkill(1, skillE_ID);
          }

          if (new_key[KEY_PRESS_O] && old_key[KEY_PRESS_O] == 0)
            ProcessingSkill(1, skillR_ID);

          if (new_key[KEY_PRESS_I] && old_key[KEY_PRESS_I] == 0)
            player_Arrow[1].increaseAngleDelta();

          if (new_key[KEY_PRESS_LEFT]) {
            player[1].moveLeft();
          }
          else if (old_key[KEY_PRESS_LEFT]) {
            player[1].setCurrentFrame(0);
          }

          if (new_key[KEY_PRESS_RIGHT]) {
            player[1].moveRight();
          }
          else if (old_key[KEY_PRESS_RIGHT]) {
            player[1].setCurrentFrame(0);
          }
        }
      }
    }

    if (player[1].getCastTimeCooldown() > 0)
      player[1].setCurrentFrame(0);

    ///player2
    if (player[2].isDead() == 0) {
      player[2].updatePlayerEffects();
      player[2].updateCooldown();
      if (player[2].getCastTimeCooldown() == 0) {
        if (new_key[KEY_PRESS_C] && old_key[KEY_PRESS_C] == 0)
          ProcessingSkill(2, skillQ_ID);
        
        if (new_key[KEY_PRESS_V] && old_key[KEY_PRESS_V] == 0)
          ProcessingSkill(2, skillW_ID);

        if ((new_key[KEY_PRESS_A] || new_key[KEY_PRESS_D])
            && new_key[KEY_PRESS_B] && old_key[KEY_PRESS_B] == 0 && player[2].getSkillCooldown(skillE_ID) == 0) 
        {
          ProcessingSkill(2, skillE_ID);
        }

        if (new_key[KEY_PRESS_G] && old_key[KEY_PRESS_G] == 0)
          ProcessingSkill(2, skillR_ID);

        if (new_key[KEY_PRESS_F] && old_key[KEY_PRESS_F] == 0)
          player_Arrow[2].increaseAngleDelta();

        if (new_key[KEY_PRESS_A]) {
          player[2].moveLeft();
        }
        else if (old_key[KEY_PRESS_A]) {
          player[2].setCurrentFrame(0);
        }

        if (new_key[KEY_PRESS_D]) {
          player[2].moveRight();
        }
        else if (old_key[KEY_PRESS_D]) {
          player[2].setCurrentFrame(0);
        }
      }
    }

    if (player[2].getCastTimeCooldown() > 0)
      player[2].setCurrentFrame(0);
  }

  ///reset
  for (int i = 0; i < KEY_PRESS_TOTAL; i++) {
    old_key[i] = new_key[i];
    new_key[i] = 0;
  }
}

void Game::render_GameBackground() {
  for (Entity& e : waterBackground) {
    render(e);
    int x = e.getX() - 1;
    if (x + e.getWidth() <= 0) x = SCREEN_WIDTH;
    e.setX(x);
  }

  for (int j = 0; j < 13; j++)
    for (int i = 0; i * 96 < SCREEN_WIDTH; i++)
      render(grassTexture, i * 96, j * 32);
  
  for (int j = 0; j < 13; j++)
    for (int i = 0; i * 96 < SCREEN_WIDTH; i++)
      render(grassTexture, i * 96, SCREEN_HEIGHT - 32 - j * 32);
}

void Game::render_Player() {
  render_Skill_Hud_And_Cooldown();

  if (player[1].getHealth() > 0) {
    if (player[1].getVulnerable() > 0) {
      Entity effect = skillW_effect;
      effect.setX(player[1].getX() + player[1].getWidth() / 2 - effect.getWidth() / 2);
      effect.setY(player[1].getY() + player[1].getHeight() - effect.getHeight() / 2);
      render(effect);
    }

    render(player[1]);

    player_Arrow[1].setX(player[1].getX() + player[1].getWidth() / 2 - player_Arrow[1].getWidth() / 2);
    player_Arrow[1].setY(player[1].getY() + player[1].getHeight() + 10);
    player_Arrow[1].setRotPoint(player_Arrow[1].getX() + player_Arrow[1].getWidth() / 2, player_Arrow[1].getY());
    render(player_Arrow[1]);
    player_Arrow[1].moveAngle();
  }

  if (player[2].getHealth() > 0) {
    if (player[2].getVulnerable() > 0) {
      Entity effect = skillW_effect;
      effect.setX(player[2].getX() + player[2].getWidth() / 2 - effect.getWidth() / 2);
      effect.setY(player[2].getY() + player[2].getHeight() - effect.getHeight() / 2);
      render(effect);
    }
    
    render(player[2]);

    player_Arrow[2].setX(player[2].getX() + player[2].getWidth() / 2 - player_Arrow[2].getWidth() / 2);
    player_Arrow[2].setY(player[2].getY() - player_Arrow[2].getHeight() - 10);
    player_Arrow[2].setRotPoint(player_Arrow[2].getX() + player_Arrow[2].getWidth() / 2, player_Arrow[2].getY() + player_Arrow[2].getHeight());
    render(player_Arrow[2]);
    player_Arrow[2].moveAngle();
  }
}

void Game::render_Bullet() {
  for (int id = 1; id <= 2; id++) {
    for (Bullet &bullet : player_bullets[id]) {
      Player &Enemy = player[3 - id];
      Rectangle Agent(Enemy);
      Rectangle Shot(bullet);

      if (bullet.getSkillId() == skillR_ID) {
        if (player[id].getSkillDelay() == 0)
          render(bullet);
      }
      else
        render(bullet);
      
      if (Enemy.isDead() == 0 && isColliding(Agent, Shot)) {
        // cout << "player " << 3 - id << " is being shot\n";
        switch (bullet.getSkillId()) {
          case (skillQ_ID): {
            Mix_Volume(-1, 100);
            Mix_PlayChannel(-1, SFX[Q_hit_sfx_ID], 0);
            Mix_Volume(-1, 50);

            Enemy.setHealth(-2);
            if (Enemy.getVulnerable() > 0) {
              Enemy.setHealth(-2);
              Enemy.setVulnerable(0);
              Mix_Volume(-1, 100);
              Mix_PlayChannel(-1, SFX[W_hit_crashed_sfx_ID], 0);
              Mix_Volume(-1, 50);
            }
            break;
          }
          case (skillW_ID): {
            Enemy.setVulnerable(vulnerableTime);
            Mix_PlayChannel(-1, SFX[W_hit_sfx_ID], 0);
            break;
          }
          case (skillR_ID): {
            if (Enemy.getDamagedDelay() == 0) {
              Mix_Volume(-1, 100);
              Mix_PlayChannel(-1, SFX[R2_sfx_ID], 0);
              Mix_Volume(-1, 50);

              Enemy.setDamagedDelay(5 * FPS);
              Enemy.setHealth(-5);
              if (Enemy.getVulnerable() > 0) {
                Enemy.setHealth(-2);
                Enemy.setVulnerable(0);
                Mix_Volume(-1, 100);
                Mix_PlayChannel(-1, SFX[W_hit_crashed_sfx_ID], 0);
                Mix_Volume(-1, 50);
              }
            }
            break;
          }
        }
        if (bullet.getSkillId() != skillR_ID)
          player_bullets[id].erase(player_bullets[id].begin() + (&bullet - &player_bullets[id][0]));
        else
          bullet.move();
      }
      else if ((bullet.getX() > SCREEN_WIDTH || bullet.getX() < 0) && (bullet.getY() > SCREEN_HEIGHT || bullet.getY() < -200)) {
        player_bullets[id].erase(player_bullets[id].begin() + (&bullet - &player_bullets[id][0]));
      }
      else {
        if (bullet.getSkillId() == skillR_ID) {
          if (player[id].getSkillDelay() == 0)
            bullet.move();
        }
        else
          bullet.move();
      }
    }
  }
}

void Game::render_MainMenu() {
  render(Background[MainMenu], 0, 0);

  if (button[VSAI_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    player[1].reset(Bot);
    player[2].reset(Human);
    resetGame();
    gameState = GamePlay;
  }
  render(button[VSAI_Button]);

  if (button[VSPlayer_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    player[1].reset(Human);
    player[2].reset(Human);
    resetGame();
    gameState = GamePlay;
  }
  render(button[VSPlayer_Button]);

  if (button[Help_Button].isClicked(MouseX, MouseY, mouseClicked))
    gameState = Help;
  render(button[Help_Button]);

  if (button[Exit_Button].isClicked(MouseX, MouseY, mouseClicked))
    gameRunning = false;
  render(button[Exit_Button]);
}

void Game::render_GamePlay() {
  if (player[1].isDead() || player[2].isDead()) {
    gameState = MainMenu;
  }
  else {
    render_GameBackground();
    render_Player();
    render_Bullet();
  }
}

void Game::render_Help() {
  render(Background[Help], 0, 0);

  // if (button[VSAI_Button].isClicked(MouseX, MouseY, mouseClicked)) {
  //   player[1].reset(Bot);
  //   player[2].reset(Human);
  //   gameState = GamePlay;
  // }
  // render(button[VSAI_Button]);

  // if (button[VSPlayer_Button].isClicked(MouseX, MouseY, mouseClicked)) {
  //   player[1].reset(Human);
  //   player[2].reset(Human);
  //   gameState = GamePlay;
  // }
  // render(button[VSPlayer_Button]);

  if (button[Help_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    if (gameState == Help) gameState = MainMenu;
    else
      gameState = Help;
  }
  render(button[Help_Button]);

  if (button[Exit_Button].isClicked(MouseX, MouseY, mouseClicked))
    gameRunning = false;
  render(button[Exit_Button]);
}

void Game::render_Game() {
  if (gameState == MainMenu) {
    render_MainMenu();
  }
  else if (gameState == GamePlay) {
    render_GamePlay();
  }
  else if (gameState == Help) {
    render_Help();
  }
}

void Game::gameLoop() {
  clear();
  handleEvents();
  update();
  render_Game();
  display();
}