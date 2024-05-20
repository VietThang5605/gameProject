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
  void resetSummonerSpellID();
  void resetItemID();
  void setEffectTime(int time, int skill_ID);

  int getType();
  int getHealth();
  int getSkillCooldown(int skill_ID);
  int getVulnerable();
  int getCastTimeCooldown();
  int getSkillDelay();
  int getDamagedDelay();
  int getSummonerSpellID();
  int getItemID();
  int getEffectTime(int skill_ID);
  bool isInvulnerable();

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
  int player_effect_time[skill_ID_Total];
  int skillDelay;
  int damagedDelay;
  int SummonerSpellID;
  int ItemID;
  int bonusVelocity;
  bool invulnerable;
};