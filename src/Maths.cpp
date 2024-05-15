#include "Maths.h"

///Vector
Vector::Vector(double x, double y) {
  this->x = x;
  this->y = y;
}

bool Vector::isCollinear(const Vector &v) const {
  return (x * v.y - v.x * y == 0);
}

///Point
Point::Point(double x, double y) {
  this->x = x;
  this->y = y;
}

void Point::setX(double x) {
  this->x = x;
}

void Point::setY(double y) {
  this->y = y;
}

Point rotatePointAroundAngle(double x, double y, Point* rot_point, double angle) {
  double radAngle = angle * PI / 180;
  double cosAngle = cos(radAngle);
  double sinAngle = sin(radAngle);
  x = rot_point->x + (x - rot_point->x) * cosAngle - (y - rot_point->y) * sinAngle;
  y = rot_point->y + (y - rot_point->y) * cosAngle + (x - rot_point->x) * sinAngle;
  return Point(x, y);
}