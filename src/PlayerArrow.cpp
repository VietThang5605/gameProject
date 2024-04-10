#include "PlayerArrow.h"

PlayerArrow::PlayerArrow() {
  angleDelta = 3;
  dir = 1;
}

void PlayerArrow::increaseAngleDelta() {
  if (increaseCooldown > 0) return;
  increaseCooldown = increaseAngleDeltaCooldown;
  angleDelta += 3;
  if (angleDelta > 6)
    angleDelta = 3;
}

void PlayerArrow::moveAngle() {
  if (increaseCooldown > 0) increaseCooldown--;
  if (getAngle() + angleDelta * dir > maxAngle) dir = -dir;
  if (getAngle() + angleDelta * dir < -maxAngle) dir = -dir;
  setAngle(getAngle() + angleDelta * dir);
}