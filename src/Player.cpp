#include "Player.h"

Player::Player() {
  type = false;
  health = 10;
  cntShot = 0;
  for (int i = 0; i < 4; i++) skill_Cooldown[i] = 0;
}

void Player::setType(bool value) {
  type = value;
}

bool Player::getType() {
  return type;
}

void Player::setHealth(int health) {
  this->health = health;
}

int Player::getHealth() {
  return health;
}

void Player::updateSkill_Cooldown() {
  for (int i = 0; i < skill_ID_Total; i++) {
    if (skill_Cooldown[i] > 0)
      skill_Cooldown[i]--;
  }
}

void Player::setSkill_Cooldown(int time, int skill_ID) {
  skill_Cooldown[skill_ID] = time;
}

int Player::getSkill_Cooldown(int skill_ID) {
  return skill_Cooldown[skill_ID];
}

