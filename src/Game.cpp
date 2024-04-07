#include <iostream>
#include <vector>

#include "Game.h"

const Uint8* keystate = SDL_GetKeyboardState(NULL);

enum KeyPressSurfaces {
  KEY_PRESS_LEFT, KEY_PRESS_RIGHT, KEY_PRESS_UP, KEY_PRESS_DOWN,
  KEY_PRESS_J, KEY_PRESS_K, KEY_PRESS_L, KEY_PRESS_U, KEY_PRESS_I,
  KEY_PRESS_A, KEY_PRESS_D, KEY_PRESS_W, KEY_PRESS_S,
  KEY_PRESS_C, KEY_PRESS_V, KEY_PRESS_B, KEY_PRESS_F, KEY_PRESS_G,
  KEY_PRESS_P, KEY_PRESS_F11,
  KEY_PRESS_TOTAL
};

int old_key[KEY_PRESS_TOTAL];
int new_key[KEY_PRESS_TOTAL];

SDL_Texture* waterTexture = NULL;
SDL_Texture* grassTexture = NULL;
SDL_Texture* ezrealTexture = NULL;

SDL_Texture* healthBarTexture = NULL;

SDL_Texture* skillQ_Hud = NULL;
SDL_Texture* skillW_Hud = NULL;
SDL_Texture* skillE_Hud = NULL;
SDL_Texture* skillR_Hud = NULL;

SDL_Texture* skillArrow = NULL;
SDL_Texture* skillQ = NULL;
SDL_Texture* skillW = NULL;
SDL_Texture* skillW_ground = NULL;

vector<Entity> waterBackground;
vector<Bullet> Bullets;

Entity player1_skillQ_Hud, player1_skillW_Hud, player1_skillE_Hud, player1_skillR_Hud;
Entity player2_skillQ_Hud, player2_skillW_Hud, player2_skillE_Hud, player2_skillR_Hud;

Bullet bullet_Q, bullet_W;
PlayerArrow player1_Arrow, player2_Arrow;
Player player1, player2;

TTF_Font* font32;

SDL_Color white = {255, 255, 255};

Game::Game() {
  gWindow = NULL;
  gRenderer = NULL;
  gameRunning = true;
  mainMenu = true;
  fullscreen = false;
};

void Game::init() {
  initSDL();
  loadMedia();
}

void Game::initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    logError("Failed to initialize SDL.", SDL_GetError());

  gWindow = SDL_CreateWindow("Game v1.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
  if (gWindow == NULL)
    logError("Failed to create window.", SDL_GetError());
  
  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == NULL)
    logError("Failted to create renderer.", SDL_GetError());
  
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
    logError("Failed to initialize SDL_image.", IMG_GetError());
  
  if (TTF_Init() != 0)
    logError("Failed to initialize SDL_ttf.", TTF_GetError());
  
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    logError("Failed to initialize SDL_mixer.", Mix_GetError());
}

void Game::loadMedia() {
  waterTexture = loadTexture("res/images/water.png");
  grassTexture = loadTexture("res/images/grass.png");
  ezrealTexture = loadTexture("res/images/ezreal-Sheet.png");

  skillQ_Hud = loadTexture("res/images/ezreal_q.png");
  skillW_Hud = loadTexture("res/images/ezreal_w.png");
  skillE_Hud = loadTexture("res/images/ezreal_e.png");
  skillR_Hud = loadTexture("res/images/ezreal_r.png");

  skillArrow = loadTexture("res/images/skillArrow.png");
  skillQ = loadTexture("res/images/skillQ.png");
  skillW = loadTexture("res/images/skillW.png");
  skillW_ground = loadTexture("res/images/skillW_ground.png");

  font32 = TTF_OpenFont("res/fonts/cocogoose.ttf", 32);

  ///entity's initialization
  bullet_Q.init(skillQ, 4 * 9, 16 * 9, 1, 1);
  bullet_W.init(skillW, 8 * 14, 8 * 14, 1, 1);

  for (int j = 20; j < 45; j++)
    for (int i = 0; i < 32; i++) {
      Entity entity;
      entity.init(waterTexture, 64, 16, 1, 1);
      entity.setX(i * 64);
      entity.setY(j * 16);
      waterBackground.push_back(entity);
    }
  
  ///player1's initialization
  player1.init(ezrealTexture, 54 * 3, 69 * 3, 8, 8);
  player1.setX(SCREEN_WIDTH / 2 - player1.getWidth() / 2);
  player1.setY(150);

  player1_Arrow.init(skillArrow, 8 * 10, 16 * 10, 1, 1);
  player1_Arrow.setCenter(player1_Arrow.getWidth() / 2, 0);

  player1_skillR_Hud.init(skillR_Hud, 96, 96, 1, 1);
  player1_skillR_Hud.setX(SCREEN_WIDTH - 30 - player1_skillR_Hud.getWidth());
  player1_skillR_Hud.setY(120 - player1_skillR_Hud.getHeight());

  player1_skillE_Hud.init(skillE_Hud, 96, 96, 1, 1);
  player1_skillE_Hud.setX(player1_skillR_Hud.getX() - player1_skillE_Hud.getWidth() - 30);
  player1_skillE_Hud.setY(120 - player1_skillE_Hud.getHeight());

  player1_skillW_Hud.init(skillW_Hud, 96, 96, 1, 1);
  player1_skillW_Hud.setX(player1_skillE_Hud.getX() - player1_skillW_Hud.getWidth() - 30);
  player1_skillW_Hud.setY(120 - player1_skillW_Hud.getHeight());

  player1_skillQ_Hud.init(skillQ_Hud, 96, 96, 1, 1);
  player1_skillQ_Hud.setX(player1_skillW_Hud.getX() - player1_skillQ_Hud.getWidth() - 30);
  player1_skillQ_Hud.setY(120 - player1_skillQ_Hud.getHeight());
  
  ///player2's initialization
  player2.init(ezrealTexture, 54 * 3, 69 * 3, 8, 8);
  player2.setX(SCREEN_WIDTH / 2 - player1.getWidth() / 2);
  player2.setY(720);

  player2_Arrow.init(skillArrow, 8 * 10, 16 * 10, 1, 1);
  player2_Arrow.setCenter(player2_Arrow.getWidth() / 2, player2_Arrow.getHeight());
  player2_Arrow.setFlip(SDL_FLIP_VERTICAL);

  player2_skillQ_Hud.init(skillQ_Hud, 96, 96, 1, 1);
  player2_skillQ_Hud.setX(30);
  player2_skillQ_Hud.setY(SCREEN_HEIGHT - 120);

  player2_skillW_Hud.init(skillW_Hud, 96, 96, 1, 1);
  player2_skillW_Hud.setX(player2_skillQ_Hud.getX() + player2_skillQ_Hud.getWidth() + 30);
  player2_skillW_Hud.setY(SCREEN_HEIGHT - 120);

  player2_skillE_Hud.init(skillE_Hud, 96, 96, 1, 1);
  player2_skillE_Hud.setX(player2_skillW_Hud.getX() + player2_skillW_Hud.getWidth() + 30);
  player2_skillE_Hud.setY(SCREEN_HEIGHT - 120);

  player2_skillR_Hud.init(skillR_Hud, 96, 96, 1, 1);
  player2_skillR_Hud.setX(player2_skillE_Hud.getX() + player2_skillE_Hud.getWidth() + 30);
  player2_skillR_Hud.setY(SCREEN_HEIGHT - 120);
}

void Game::closeMedia() {
  SDL_DestroyTexture(waterTexture); waterTexture = NULL;
  SDL_DestroyTexture(grassTexture); grassTexture = NULL;
  SDL_DestroyTexture(ezrealTexture); ezrealTexture = NULL;
  
  SDL_DestroyTexture(healthBarTexture); healthBarTexture = NULL;

  SDL_DestroyTexture(skillQ_Hud); skillQ_Hud = NULL;
  SDL_DestroyTexture(skillW_Hud); skillW_Hud = NULL;
  SDL_DestroyTexture(skillE_Hud); skillE_Hud = NULL;
  SDL_DestroyTexture(skillR_Hud); skillR_Hud = NULL;

  SDL_DestroyTexture(skillArrow); skillArrow = NULL;
  SDL_DestroyTexture(skillQ); skillQ = NULL;
  SDL_DestroyTexture(skillW); skillW = NULL;
  SDL_DestroyTexture(skillW_ground); skillW_ground = NULL;

  TTF_CloseFont(font32);
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

void Game::render(ScrollingBackground &bgr) {
  render(bgr.getTexture(), bgr.getScrollingOffset(), 0);
  render(bgr.getTexture(), bgr.getScrollingOffset() - bgr.getWidth(), 0);
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
}

void Game::renderSkillCooldown(Player &player, int skill_ID) {
  
}

void Game::handleEvents() { 
  ///player1
  if (keystate[SDL_SCANCODE_LEFT]) new_key[KEY_PRESS_LEFT] = 1;
  if (keystate[SDL_SCANCODE_RIGHT]) new_key[KEY_PRESS_RIGHT] = 1;
  if (keystate[SDL_SCANCODE_UP]) new_key[KEY_PRESS_UP] = 1;
  if (keystate[SDL_SCANCODE_DOWN]) new_key[KEY_PRESS_DOWN] = 1;
  if (keystate[SDL_SCANCODE_J]) new_key[KEY_PRESS_J] = 1;
  if (keystate[SDL_SCANCODE_K]) new_key[KEY_PRESS_K] = 1;
  if (keystate[SDL_SCANCODE_L]) new_key[KEY_PRESS_L] = 1;
  if (keystate[SDL_SCANCODE_U]) new_key[KEY_PRESS_U] = 1;
  if (keystate[SDL_SCANCODE_I]) new_key[KEY_PRESS_I] = 1;

  ///player2
  if (keystate[SDL_SCANCODE_A]) new_key[KEY_PRESS_A] = 1;
  if (keystate[SDL_SCANCODE_D]) new_key[KEY_PRESS_D] = 1;
  if (keystate[SDL_SCANCODE_W]) new_key[KEY_PRESS_W] = 1;
  if (keystate[SDL_SCANCODE_S]) new_key[KEY_PRESS_S] = 1;
  if (keystate[SDL_SCANCODE_C]) new_key[KEY_PRESS_C] = 1;
  if (keystate[SDL_SCANCODE_V]) new_key[KEY_PRESS_V] = 1;
  if (keystate[SDL_SCANCODE_B]) new_key[KEY_PRESS_B] = 1;
  if (keystate[SDL_SCANCODE_F]) new_key[KEY_PRESS_F] = 1;
  if (keystate[SDL_SCANCODE_G]) new_key[KEY_PRESS_G] = 1;

  ///utility
  if (keystate[SDL_SCANCODE_P]) new_key[KEY_PRESS_P] = 1;
  if (keystate[SDL_SCANCODE_F11]) new_key[KEY_PRESS_F11] = 1;

  if (new_key[KEY_PRESS_LEFT] && new_key[KEY_PRESS_RIGHT]) {
    new_key[KEY_PRESS_LEFT] = new_key[KEY_PRESS_RIGHT] = 0;
  }
  if (new_key[KEY_PRESS_A] && new_key[KEY_PRESS_D]) {
    new_key[KEY_PRESS_A] = new_key[KEY_PRESS_D] = 0;
  }

  while (SDL_PollEvent(&gEvent)) {
    if (gEvent.type == SDL_QUIT || gEvent.key.keysym.sym == SDLK_RETURN) {
      gameRunning = false;
      break;
    }
  }
}

void Game::update() {
  ///Function keys
  if (new_key[KEY_PRESS_F11] && old_key[KEY_PRESS_F11] == 0) {
    if (fullscreen == false) {
      SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
      fullscreen = true;
    }
    else {
      SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_SHOWN);
      fullscreen = false;
    }
  }
  
  if (new_key[KEY_PRESS_P] && old_key[KEY_PRESS_P] == 0) {
    
  }

  ///player1
  player1.updateSkill_Cooldown();
  if (new_key[KEY_PRESS_J] && old_key[KEY_PRESS_J] == 0 && player1.getSkill_Cooldown(skillQ_ID) == 0) {
    Bullet tmp = bullet_Q;
    tmp.setX(player1.getX() + player1.getWidth() / 2 - tmp.getWidth() / 2);
    tmp.setY(player1.getY() + player1.getHeight());
    tmp.setAngle(player1_Arrow.getAngle());
    tmp.setCenter(tmp.getWidth() / 2, 0);
    tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY()); ///not reset after each frame when in vector Bullets
    Bullets.push_back(tmp);
    player1.setSkill_Cooldown(FPS * 3, skillQ_ID);
  }
  else if (player1.getSkill_Cooldown(skillQ_ID) > 0) {
    std::string text = TimeToString(player1.getSkill_Cooldown(skillQ_ID) / (FPS * 1.0));
    renderTextCenter(player1_skillQ_Hud.getX() + player1_skillQ_Hud.getWidth() / 2,
                    player1_skillQ_Hud.getY() + player1_skillQ_Hud.getHeight() / 2,
                    text,
                    font32,
                    white);
  }

  if (new_key[KEY_PRESS_K] && old_key[KEY_PRESS_K] == 0 && player1.getSkill_Cooldown(skillW_ID) == 0) {
    Bullet tmp = bullet_W;
    tmp.setX(player1.getX() + player1.getWidth() / 2 - tmp.getWidth() / 2);
    tmp.setY(player1.getY() + player1.getHeight());
    tmp.setAngle(player1_Arrow.getAngle());
    tmp.setCenter(tmp.getWidth() / 2, 0);
    tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY()); ///not reset after each frame when in vector Bullets
    Bullets.push_back(tmp);
    // player1.setSkill_Cooldown(FPS * 3, skillW_ID);
  }
  else if (player1.getSkill_Cooldown(skillW_ID) > 0) {
    std::string text = TimeToString(player1.getSkill_Cooldown(skillW_ID) / (FPS * 1.0));
    renderTextCenter(player1_skillW_Hud.getX() + player1_skillW_Hud.getWidth() / 2,
                    player1_skillW_Hud.getY() + player1_skillW_Hud.getHeight() / 2,
                    text,
                    font32,
                    white);
  }

  if (new_key[KEY_PRESS_LEFT]) {
    if (old_key[KEY_PRESS_LEFT] == 0) {
      player1.setCurrentFrame(0);
      player1.setFlip(SDL_FLIP_HORIZONTAL);
    }
    player1.setX(player1.getX() - 20);
    player1.setCurrentFrame(player1.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_LEFT]) {
    player1.setCurrentFrame(0);
  }

  if (new_key[KEY_PRESS_RIGHT]) {
    if (old_key[KEY_PRESS_RIGHT] == 0) {
      player1.setCurrentFrame(0);
      player1.setFlip(SDL_FLIP_NONE);
    }
    player1.setX(player1.getX() + 20);
    player1.setCurrentFrame(player1.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_RIGHT]) {
    player1.setCurrentFrame(0);
  }

  ///player2
  player2.updateSkill_Cooldown();
  if (new_key[KEY_PRESS_C] && old_key[KEY_PRESS_C] == 0 && player2.getSkill_Cooldown(skillQ_ID) == 0) {
    Bullet tmp = bullet_Q;
    tmp.setX(player2.getX() + player2.getWidth() / 2 - tmp.getWidth() / 2);
    tmp.setY(player2.getY() - tmp.getHeight());
    tmp.setAngle(player2_Arrow.getAngle());
    tmp.setCenter(tmp.getWidth() / 2, tmp.getHeight());
    tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY() + tmp.getHeight());
    tmp.setFlip(SDL_FLIP_VERTICAL);
    tmp.setVelocityX(-tmp.getVelocityX());
    tmp.setVelocityY(-tmp.getVelocityY());
    Bullets.push_back(tmp);
    player2.setSkill_Cooldown(FPS * 3, skillQ_ID);
  }
  else if (player2.getSkill_Cooldown(skillQ_ID) > 0) {
    std::string text = TimeToString(player2.getSkill_Cooldown(skillQ_ID) / (FPS * 1.0));
    renderTextCenter(player2_skillQ_Hud.getX() + player2_skillQ_Hud.getWidth() / 2,
                    player2_skillQ_Hud.getY() + player2_skillQ_Hud.getHeight() / 2,
                    text,
                    font32,
                    white);
  }

  if (new_key[KEY_PRESS_A]) {
    if (old_key[KEY_PRESS_A] == 0) {
      player2.setCurrentFrame(0);
      player2.setFlip(SDL_FLIP_HORIZONTAL);
    }
    player2.setX(player2.getX() - 20);
    player2.setCurrentFrame(player2.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_A]) {
    player2.setCurrentFrame(0);
  }

  if (new_key[KEY_PRESS_D]) {
    if (old_key[KEY_PRESS_D] == 0) {
      player2.setCurrentFrame(0);
      player2.setFlip(SDL_FLIP_NONE);
    }
    player2.setX(player2.getX() + 20);
    player2.setCurrentFrame(player2.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_D]) {
    player2.setCurrentFrame(0);
  }

  ///reset
  for (int i = 0; i < KEY_PRESS_TOTAL; i++) {
    old_key[i] = new_key[i];
    new_key[i] = 0;
  }
}

void Game::renderGameBackground() {
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

void Game::renderPlayer() {
  render(player1);
  player1_Arrow.setX(player1.getX() + player1.getWidth() / 2 - player1_Arrow.getWidth() / 2);
  player1_Arrow.setY(player1.getY() + player1.getHeight() + 10);
  player1_Arrow.setRotPoint(player1_Arrow.getX() + player1_Arrow.getWidth() / 2, player1_Arrow.getY());
  render(player1_Arrow);
  player1_Arrow.moveAngle();

  render(player2);
  player2_Arrow.setX(player2.getX() + player2.getWidth() / 2 - player2_Arrow.getWidth() / 2);
  player2_Arrow.setY(player2.getY() - player2_Arrow.getHeight() - 10);
  player2_Arrow.setRotPoint(player2_Arrow.getX() + player2_Arrow.getWidth() / 2, player2_Arrow.getY() + player2_Arrow.getHeight());
  render(player2_Arrow);
  player2_Arrow.moveAngle();
}

void Game::gameLoop() {
  clear();
  handleEvents();
  renderGameBackground();

  render(player1_skillQ_Hud);
  render(player1_skillW_Hud);
  render(player1_skillE_Hud);
  render(player1_skillR_Hud);

  render(player2_skillQ_Hud);
  render(player2_skillW_Hud);
  render(player2_skillE_Hud);
  render(player2_skillR_Hud);

  // render(skillW_ground, player1.getX() + player1.getWidth() / 2 - 128 / 2, player1.getY() + player1.getHeight() - 64 / 2, 128, 64, 0, NULL);

  update();
  renderPlayer();

  // static int count = 0;
  for (Bullet &bullet : Bullets) {
    // Rectangle rec1(player2);
    // Rectangle rec2(bullet);
    // if (isColliding(rec1, rec2)) {
    //   count++;
    //   std::cout << count << "\n\n";
    // }
    render(bullet);
    bullet.move();
    // bullet.setRotPoint(bullet.getX() + bullet.getWidth() / 2, bullet.getY());
    if ((bullet.getX() > SCREEN_WIDTH || bullet.getX() < 0) && (bullet.getY() > SCREEN_HEIGHT || bullet.getY() < -200)) Bullets.erase(Bullets.begin() + (&bullet - &Bullets[0]));
  }

  display();
}