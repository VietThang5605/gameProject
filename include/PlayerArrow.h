#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"
#include "Constants.h"

class PlayerArrow : public Entity {
public:
  PlayerArrow();
  
  void reset();
  void increaseAngleDelta();
  void moveAngle();
private:
  double delta;
  int dir;
  int increaseCooldown;
};