#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <vector>
#include <math.h>

#include "Constants.h"

using std::vector;

class Vector {
public:
  Vector(double x = 0, double y = 0);
  bool isCollinear(const Vector &v) const;
  
  double x, y;
};

class Point {
public:
  Point(double x = 0, double y = 0);
  void setX(double x);
  void setY(double y);

  double x, y;
};

Point rotatePointAroundAngle(double x, double y, Point* rot_point, double angle);