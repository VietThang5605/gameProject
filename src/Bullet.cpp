#include "Bullet.h"

Bullet::Bullet() {
  velocityX = 45;
  velocityY = 45;
  rotPoint.x = rotPoint.y = -INT_INF;
}

void Bullet::init(SDL_Texture* p_tex, double width, double height, int frames, int lines, int skill_ID) {
  Entity::init(p_tex, width, height, frames, lines);
  this->skill_ID = skill_ID;
}

void Bullet::setVelocityX(double x) {
  velocityX = x;
}

void Bullet::setVelocityY(double y) {
  velocityY = y;
}

double Bullet::getVelocityX() {
  return velocityX;
}

double Bullet::getVelocityY() {
  return velocityY;
}

void Bullet::move() {
  double radAngle = getAngle() * PI / 180;
  setX(getX() + velocityX * -sin(radAngle));
  setY(getY() + velocityY * abs(cos(radAngle)));
  if (velocityX > 0 && velocityY > 0) ///player1
    setRotPoint(getX() + getWidth() / 2, getY());
  else
    setRotPoint(getX() + getWidth() / 2, getY() + getHeight());
}

bool Bullet::isOnScreen() {
  return !(((getX() > SCREEN_WIDTH || getX() < 0) && (getY() > SCREEN_HEIGHT || getY() < -200)) || ((getY() > SCREEN_HEIGHT + 200 || getY() < -200)));
}

int Bullet::getSkillId() {
  return skill_ID;
}