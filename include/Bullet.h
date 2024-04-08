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
  void init(SDL_Texture* p_tex, double width, double height, int frames, int lines, int skill_ID);

  void setVelocityX(double x);
  void setVelocityY(double y);

  double getVelocityX();
  double getVelocityY();

  void move();
  int getSkillId();
private:
  double velocityX;
  double velocityY;
  int skill_ID;
};