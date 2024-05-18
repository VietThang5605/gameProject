#include "Player.h"

Player::Player() {

}

void Player::setStartXY(double x, double y) {
  startX = x;
  startY = y;
}

void Player::reset(int Player_Type) {
  setCurrentFrame(0);
  x = startX;
  y = startY;
  type = Player_Type;
  vulnerable = 0;
  health = PlayerStartHealth;
  velocity = PlayerVelocity;
  velocityTeleport = PlayerTeleportVelocity;
  castTimeCooldown = 0;
  for (int id = 0; id < skill_ID_Total; id++)
    player_skill_cooldown[id] = skill_cooldown_start[id];
}

void Player::setType(int Player_Type) {
  type = Player_Type;
}

void Player::setHealth(int delta) {
  health += delta;
}

void Player::setSkillCooldown(int time, int skill_ID) {
  player_skill_cooldown[skill_ID] = time;
}

void Player::setVulnerable(int value) {
  vulnerable = value;
}

void Player::setCastTimeCooldown(int value) {
  castTimeCooldown = value;
}

void Player::setSkillDelay(int value) {
  skillDelay = value;
}

void Player::setDamagedDelay(int value) {
  damagedDelay = value;
}

int Player::getType() {
  return type;
}

int Player::getHealth() {
  return health;
}

int Player::getSkillCooldown(int skill_ID) {
  return player_skill_cooldown[skill_ID];
}

int Player::getVulnerable() {
  return vulnerable;
}

int Player::getCastTimeCooldown() {
  return castTimeCooldown;
}

int Player::getSkillDelay() {
  return skillDelay;
}

int Player::getDamagedDelay() {
  return damagedDelay;
}

bool Player::isDead() {
  return getHealth() <= 0;
}

void Player::updateCooldown() {
  if (castTimeCooldown > 0) castTimeCooldown--;
  if (skillDelay > 0) skillDelay--;
  if (damagedDelay > 0) damagedDelay--;
  
  for (int id = 0; id < skill_ID_Total; id++) {
    if (player_skill_cooldown[id] > 0)
      player_skill_cooldown[id]--;
  }
}

void Player::updatePlayerEffects() {
  if (vulnerable > 0) vulnerable--;
}

void Player::moveLeft() {
  if (getFlip() == SDL_FLIP_NONE) //last direction is right
    setCurrentFrame(0);
  setFlip(SDL_FLIP_HORIZONTAL);
  setCurrentFrame(getCurrentFrame() + 1);

  if (player_skill_cooldown[skillE_ID] == skill_cooldown[skillE_ID])
    setX(getX() - velocityTeleport);
  else
    setX(getX() - velocity);
  if (getX() < PlayerScreenLeftBoundary) setX(PlayerScreenLeftBoundary);
}

void Player::moveRight() {
  if (getFlip() == SDL_FLIP_HORIZONTAL) //last direction is left
    setCurrentFrame(0);
  setFlip(SDL_FLIP_NONE);
  setCurrentFrame(getCurrentFrame() + 1);

  if (player_skill_cooldown[skillE_ID] == skill_cooldown[skillE_ID])
    setX(getX() + velocityTeleport);
  else
    setX(getX() + velocity);
  if (getX() > PlayerScreenRightBoundary) setX(PlayerScreenRightBoundary);
}