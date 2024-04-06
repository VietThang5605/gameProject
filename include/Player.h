#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"
#include "Constants.h"

class Player : public Entity {
public:
  Player();
  void setType(bool value);
  bool getType();
  void setHealth(int health);
  int getHealth();
  void updateSkill_Cooldown();
  void setSkill_Cooldown(int time, int skill_ID);
  int getSkill_Cooldown(int skill_ID);
private:
  bool type;
  int health;
  int cntShot;
  int skill_Cooldown[4];
};