#include "Bullet.h"

Bullet::Bullet() {
  velocityX = 20;
  velocityY = 20;
}

void Bullet::move() {
  double Degrees = getAngle() * 3.14159 / 180;
  setX(getX() + velocityX * -sin(Degrees));
  setY(getY() + velocityY * abs(cos(Degrees)));
}