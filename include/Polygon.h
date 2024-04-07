#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <vector>
#include <math.h>

#include "Maths.h"
#include "Entity.h"
#include "Constants.h"

using std::cout;
using std::vector;

class Rectangle {
public:
  Rectangle(double x, double y, double w, double h, double angle = 0, Point* rotPoint = NULL);
  Rectangle(Entity &p_entity);
  // bool Rectangle::isColliding(Rectangle &r1, Rectangle &r2) const;

  vector<Point> vertices;
  vector<Vector> axis;
  double angle;
  Point rotPoint;
  Point coords;
};

bool isColliding(Rectangle &poly1, Rectangle &poly2);

void removeRepeat(vector<Vector> &v);