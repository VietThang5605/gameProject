#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <math.h>

#include "Entity.h"

class Bullet : public Entity {
public:
  Bullet();
  void move();
private:
  int velocityX;
  int velocityY;
};