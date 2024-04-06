#include "Bullet.h"

Bullet::Bullet() {
  velocityX = 40;
  velocityY = 40;
}

void Bullet::move() {
  double radAngle = getAngle() * PI / 180;
  setX(getX() + velocityX * -sin(radAngle));
  setY(getY() + velocityY * abs(cos(radAngle)));
}

void Bullet::setVelocityX(int x) {
  velocityX = x;
}

void Bullet::setVelocityY(int y) {
  velocityY = y;
}

int Bullet::getVelocityX() {
  return velocityX;
}

int Bullet::getVelocityY() {
  return velocityY;
}