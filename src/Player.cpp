#include "Player.h"

Player::Player() {

}

void Player::setStartXY(double x, double y) {
  startX = x;
  startY = y;
}

void Player::reset(int Player_Type) {
  x = startX;
  y = startY;
  type = Player_Type;
  vulnerable = 0;
  health = 20;
  cntShot = 0;
  velocity = 18;
  velocityTeleport = 500;
  castTimeCooldown = 0;
  player_skill_Cooldown[skillQ_ID] = 1 * FPS;
  player_skill_Cooldown[skillW_ID] = 3 * FPS;
  player_skill_Cooldown[skillE_ID] = 3 * FPS;
  player_skill_Cooldown[skillR_ID] = 15 * FPS;
}

void Player::setType(int Player_Type) {
  type = Player_Type;
}

void Player::setHealth(int delta) {
  health += delta;
}

void Player::setSkillCooldown(int time, int skill_ID) {
  player_skill_Cooldown[skill_ID] = time;
}

void Player::setVulnerable(int value) {
  vulnerable = value;
}

void Player::setCastTimeCooldown(int value) {
  castTimeCooldown = value;
}

int Player::getType() {
  return type;
}

int Player::getHealth() {
  return health;
}

int Player::getSkillCooldown(int skill_ID) {
  return player_skill_Cooldown[skill_ID];
}

int Player::getVulnerable() {
  return vulnerable;
}

int Player::getCastTimeCooldown() {
  return castTimeCooldown;
}

bool Player::isDead() {
  return getHealth() <= 0;
}

void Player::updateCooldown() {
  if (castTimeCooldown > 0) castTimeCooldown--;
  
  for (int id = 0; id < skill_ID_Total; id++) {
    if (player_skill_Cooldown[id] > 0)
      player_skill_Cooldown[id]--;
  }
}

void Player::updatePlayerEffects() {
  if (vulnerable > 0) vulnerable--;
}

void Player::moveLeft() {
  if (player_skill_Cooldown[skillE_ID] == skill_Cooldown[skillE_ID])
    setX(getX() - velocityTeleport);
  else
    setX(getX() - velocity);
  if (getX() < 50) setX(50);
}

void Player::moveRight() {
  if (player_skill_Cooldown[skillE_ID] == skill_Cooldown[skillE_ID])
    setX(getX() + velocityTeleport);
  else
    setX(getX() + velocity);
  if (getX() > SCREEN_WIDTH - 200) setX(SCREEN_WIDTH - 200);
}