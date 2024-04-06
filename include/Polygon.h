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

class Shape {
public:
  // virtual bool isColliding(const Shape* poly) const = 0;
protected:
  Point coords;
};

class Rectangle : public Shape {
public:
  Rectangle(double x, double y, double w, double h, double angle = 0, Point* rotPoint = NULL);
  Rectangle(Entity &p_entity);
  // virtual bool isColliding(const Shape* poly) const;
private:
  vector<Point> vertices;
  vector<Vector> axis;
  double angle;
  Point center;
};

void removeRepeat(vector<Vector> &v);