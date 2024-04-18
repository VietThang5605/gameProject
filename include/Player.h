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

  void reset(int Player_Type);

  void setStartXY(double x, double y);
  void setType(int Player_Type);
  void setHealth(int health);
  void setSkillCooldown(int time, int skill_ID);
  void setVulnerable(int value);
  void setCastTimeCooldown(int value);

  int getType();
  int getHealth();
  int getSkillCooldown(int skill_ID);
  int getVulnerable();
  int getCastTimeCooldown();

  bool isDead();
  void updateCooldown();
  void updatePlayerEffects();
  void moveLeft();
  void moveRight();
private:
  double startX, startY;
  int type;
  int vulnerable;
  int health;
  int cntShot;
  int velocity;
  int velocityTeleport;
  int castTimeCooldown;
  int player_skill_Cooldown[4];
};