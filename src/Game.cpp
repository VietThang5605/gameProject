#include <iostream>
#include <vector>

#include "Game.h"
#include "Functions.h"
#include "ScrollingBackground.h"
#include "Entity.h"
#include "Player.h"
#include "PlayerArrow.h"
#include "Bullet.h"

using std::vector;

const Uint8* keystate = SDL_GetKeyboardState(NULL);

enum KeyPressSurfaces {
  KEY_PRESS_LEFT,
  KEY_PRESS_RIGHT,
  KEY_PRESS_A,
  KEY_PRESS_D,
  KEY_PRESS_P,
  KEY_PRESS_C,
  KEY_PRESS_F11,
  KEY_PRESS_TOTAL
};

int old_key[KEY_PRESS_TOTAL];
int new_key[KEY_PRESS_TOTAL];

SDL_Texture* waterSprite = NULL;
SDL_Texture* grassSprite = NULL;
SDL_Texture* ezrealSprite = NULL;
SDL_Texture* skillArrow = NULL;
SDL_Texture* skillQ = NULL;

vector<Entity> waterBackground;
vector<Bullet> Bullets;

Bullet Qbullet;
PlayerArrow player1Arrow;
Player player1, player2;

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
  waterSprite = loadTexture("res/images/water.png");
  grassSprite = loadTexture("res/images/grass.png");
  ezrealSprite = loadTexture("res/images/ezreal-Sheet.png");
  skillArrow = loadTexture("res/images/skillArrow6.png");
  skillQ = loadTexture("res/images/skillQ.png");

  for (int j = 15; j < 30; j++)
    for (int i = 0; i < 21; i++) {
      Entity entity;
      entity.init(waterSprite, 64, 16, 1, 1);
      entity.setX(i * 64);
      entity.setY(j * 16);
      waterBackground.push_back(entity);
    }
  
  player1.init(ezrealSprite, 108, 138, 8, 8);
  player1.setX(SCREEN_WIDTH / 2 - player1.getWidth() / 2);
  player1.setY(100);
  player1Arrow.init(skillArrow, 48, 96, 1, 1);
  player1Arrow.setCenter(player1Arrow.getWidth() / 2, 0);

  Qbullet.init(skillQ, 48, 96, 1, 1);
  Qbullet.setCenter(Qbullet.getWidth() / 2, 0);

  player2.init(ezrealSprite, 108, 138, 8, 8);
  player2.setX(SCREEN_WIDTH / 2 - player1.getWidth() / 2);
  player2.setY(500);
}

void Game::closeMedia() {
  SDL_DestroyTexture(waterSprite);
  waterSprite = NULL;
  SDL_DestroyTexture(grassSprite);
  grassSprite = NULL;
  SDL_DestroyTexture(ezrealSprite);
  ezrealSprite = NULL;
  SDL_DestroyTexture(skillArrow);
  skillArrow = NULL;
  SDL_DestroyTexture(skillQ);
  skillQ = NULL;
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

void Game::render(SDL_Texture* p_tex, int x, int y, int w, int h, double angle, SDL_Point* center, SDL_RendererFlip flip) {
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

void Game::render(Entity &p_entity, int w, int h) {
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

void Game::handleEvents() {  
  if (keystate[SDL_SCANCODE_LEFT]) new_key[KEY_PRESS_LEFT] = 1;
  if (keystate[SDL_SCANCODE_RIGHT]) new_key[KEY_PRESS_RIGHT] = 1;
  if (keystate[SDL_SCANCODE_A]) new_key[KEY_PRESS_A] = 1;
  if (keystate[SDL_SCANCODE_D]) new_key[KEY_PRESS_D] = 1;
  if (keystate[SDL_SCANCODE_C]) new_key[KEY_PRESS_C] = 1;
  if (keystate[SDL_SCANCODE_F11]) new_key[KEY_PRESS_F11] = 1;

  while (SDL_PollEvent(&gEvent)) {
    if (gEvent.type == SDL_QUIT || gEvent.key.keysym.sym == SDLK_RETURN) {
      gameRunning = false;
      break;
    }
  }

  if (new_key[KEY_PRESS_LEFT] && new_key[KEY_PRESS_RIGHT]) {
    new_key[KEY_PRESS_LEFT] = new_key[KEY_PRESS_RIGHT] = 0;
  }
  if (new_key[KEY_PRESS_A] && new_key[KEY_PRESS_D]) {
    new_key[KEY_PRESS_A] = new_key[KEY_PRESS_D] = 0;
  }

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
}

void Game::update() {
  if (new_key[KEY_PRESS_C] && old_key[KEY_PRESS_C] == 0) {
    Bullet tmp = Qbullet;
    tmp.setX(player1.getX() + player1.getWidth() / 2 - tmp.getWidth() / 2);
    tmp.setY(player1.getY() + player1.getHeight());
    tmp.setAngle(player1Arrow.getAngle());
    Bullets.push_back(tmp);
  }

  if (new_key[KEY_PRESS_LEFT]) {
    if (old_key[KEY_PRESS_LEFT] == 0) {
      player1.setCurrentFrame(0);
      player1.setFlip(SDL_FLIP_HORIZONTAL);
    }
    player1.setX(player1.getX() - 15);
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
    player1.setX(player1.getX() + 15);
    player1.setCurrentFrame(player1.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_RIGHT]) {
    player1.setCurrentFrame(0);
  }

  if (new_key[KEY_PRESS_A]) {
    if (old_key[KEY_PRESS_A] == 0) {
      player2.setCurrentFrame(0);
      player2.setFlip(SDL_FLIP_HORIZONTAL);
    }
    player2.setX(player2.getX() - 15);
    player2.setCurrentFrame(player2.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_LEFT]) {
    player2.setCurrentFrame(0);
  }
  if (new_key[KEY_PRESS_D]) {
    if (old_key[KEY_PRESS_D] == 0) {
      player2.setCurrentFrame(0);
      player2.setFlip(SDL_FLIP_NONE);
    }
    player2.setX(player2.getX() + 15);
    player2.setCurrentFrame(player2.getCurrentFrame() + 1);
  }
  else if (old_key[KEY_PRESS_D]) {
    player2.setCurrentFrame(0);
  }

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

  for (int j = 0; j < 8; j++)
    for (int i = 0; i * 96 < SCREEN_WIDTH; i++)
      render(grassSprite, i * 96, j * 32);
  
  for (int j = 0; j < 8; j++)
    for (int i = 0; i * 96 < SCREEN_WIDTH; i++)
      render(grassSprite, i * 96, SCREEN_HEIGHT - 32 - j * 32);
}

void Game::gameLoop() {
  clear();
  handleEvents();
  update();
  renderGameBackground();

  render(player1);
  player1Arrow.setX(player1.getX() + player1.getWidth() / 2 - player1Arrow.getWidth() / 2);
  player1Arrow.setY(player1.getY() + player1.getHeight() + 10);
  render(player1Arrow);
  
  player1Arrow.moveAngle();

  for (Bullet &bullet : Bullets) {
    render(bullet);
    bullet.move();
    if ((bullet.getX() > SCREEN_WIDTH || bullet.getX() < 0) && bullet.getY() > SCREEN_HEIGHT) Bullets.erase(Bullets.begin() + (&bullet - &Bullets[0]));
  }

  render(player2);
  
  display();
}