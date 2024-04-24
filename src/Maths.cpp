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