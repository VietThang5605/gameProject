#include "PlayerArrow.h"

PlayerArrow::PlayerArrow() {
  angleDelta = 3;
}

void PlayerArrow::setAngleDelta(double delta) {
  angleDelta = delta;
}

void PlayerArrow::moveAngle() {
  if (getAngle() + angleDelta > 70) angleDelta = -angleDelta;
  if (getAngle() + angleDelta < -70) angleDelta = -angleDelta;
  setAngle(getAngle() + angleDelta);
}