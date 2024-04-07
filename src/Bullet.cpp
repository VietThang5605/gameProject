#include "Bullet.h"

Bullet::Bullet() {
  velocityX = 50;
  velocityY = 50;
}

void Bullet::move() {
  double radAngle = getAngle() * PI / 180;
  setX(getX() + velocityX * -sin(radAngle));
  setY(getY() + velocityY * abs(cos(radAngle)));
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