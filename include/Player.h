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
  void setSkillDelay(int value);
  void setDamagedDelay(int value);

  int getType();
  int getHealth();
  int getSkillCooldown(int skill_ID);
  int getVulnerable();
  int getCastTimeCooldown();
  int getSkillDelay();
  int getDamagedDelay();

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
  int velocity;
  int velocityTeleport;
  int castTimeCooldown;
  int player_skill_cooldown[skill_ID_Total];
  int skillDelay;
  int damagedDelay;
};