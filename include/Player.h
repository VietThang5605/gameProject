#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"

class Player : public Entity {
public:
  Player();
private:
  bool dead;
  int health;
  int cntShot;
  int teleportCooldown;
};