#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h";

class PlayerArrow : public Entity {
public:
  PlayerArrow();

  void setAngleDelta(int delta);
  void moveAngle();
private:
  int angleDelta;
};