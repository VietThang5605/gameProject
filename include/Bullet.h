#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <math.h>

#include "Entity.h"
#include "Constants.h"

class Bullet : public Entity {
public:
  Bullet();
  void move();
  void setVelocityX(double x);
  void setVelocityY(double y);
  double getVelocityX();
  double getVelocityY();
private:
  double velocityX;
  double velocityY;
};