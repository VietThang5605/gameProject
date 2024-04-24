#include "PlayerArrow.h"

PlayerArrow::PlayerArrow() {
  delta = angleDelta;
  dir = 1;
}

void PlayerArrow::reset() {
  angle = 0;
  delta = angleDelta;
  increaseCooldown = 0;
}



void PlayerArrow::increaseAngleDelta() {
  if (increaseCooldown > 0) return;
  increaseCooldown = increaseAngleDeltaCooldown;
  delta += angleDelta;
  if (delta > angleDeltaMax)
    delta = angleDelta;
}

void PlayerArrow::moveAngle() {
  if (increaseCooldown > 0) increaseCooldown--;
  if (getAngle() + delta * dir > maxAngle) dir = -dir;
  if (getAngle() + delta * dir < -maxAngle) dir = -dir;
  setAngle(getAngle() + delta * dir);
}