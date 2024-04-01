#include "Bullet.h"

Bullet::Bullet() {
  velocityX = 40;
  velocityY = 40;
}

void Bullet::move() {
  double Degrees = getAngle() * 3.14159 / 180;
  setX(getX() + velocityX * -sin(Degrees));
  setY(getY() + velocityY * abs(cos(Degrees)));
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