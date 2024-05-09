#include "Polygon.h"

///Rectangle
Rectangle::Rectangle(double x, double y, double w, double h, double angle, Point center) {
  coords = Point(x, y);
  vertices.push_back(Point(x, y));
  vertices.push_back(Point(x, y + h));
  vertices.push_back(Point(x + w, y + h));
  vertices.push_back(Point(x + w, y));
  if (center.x != -INT_INF)
    rotPoint = center;
  else
    rotPoint = Point(x + w / 2, y + h / 2);
  if (angle != 0) {
    double radAngle = angle * PI / 180;
    double cosAngle = cos(radAngle);
    double sinAngle = sin(radAngle);
    for (Point &vertex: vertices) {
      vertex.x = rotPoint.x + (vertex.x - rotPoint.x) * cosAngle - (vertex.y - rotPoint.y) * sinAngle;
      vertex.y = rotPoint.y + (vertex.y - rotPoint.y) * cosAngle + (vertex.x - rotPoint.x) * sinAngle;
    }
  }
  for (int i = 0; i < (int)vertices.size(); i++) {
    double xAxis = -(vertices[(i + 1) % (int)vertices.size()].y - vertices[i].y);
    double yAxis = vertices[(i + 1) % (int)vertices.size()].x - vertices[i].x;
    axis.push_back(Vector(xAxis, yAxis));
  }
  removeRepeat(axis);
}

Rectangle::Rectangle(Entity &p_entity) {
  *this = Rectangle(p_entity.getX(), p_entity.getY(), p_entity.getWidth(), p_entity.getHeight(), p_entity.getAngle(), p_entity.getRotPoint());
}

///utility
bool isColliding(Rectangle &poly1, Rectangle &poly2) {
  Rectangle *r1 = &poly1;
  Rectangle *r2 = &poly2;
  for (int shape = 0; shape < 2; shape++) {
    if (shape == 1) {
      r1 = &poly2;
      r2 = &poly1;
    }

    for (int i = 0; i < (int)r1->axis.size(); i++) {
      Vector axis = r1->axis[i];
      
      double min_r1 = DOUBLE_INF, max_r1 = -DOUBLE_INF;
      for (int j = 0; j < (int)r1->vertices.size(); j++) {
        double dotProduct = r1->vertices[j].x * axis.x + r1->vertices[j].y * axis.y;
        if (dotProduct < min_r1) min_r1 = dotProduct;
        if (dotProduct > max_r1) max_r1 = dotProduct;
      }

      double min_r2 = DOUBLE_INF, max_r2 = -DOUBLE_INF;
      for (int j = 0; j < (int)r2->vertices.size(); j++) {
        double dotProduct = r2->vertices[j].x * axis.x + r2->vertices[j].y * axis.y;
        if (dotProduct < min_r2) min_r2 = dotProduct;
        if (dotProduct > max_r2) max_r2 = dotProduct;
      }
      
      if (min_r1 > max_r2 || min_r2 > max_r1)
        return false;
    }
  }
  return true;
}

void removeRepeat(vector<Vector> &v) {
  for (int i = 0; i < (int)v.size(); i++) {
    for (int j = i + 1; j < (int)v.size(); j++)
      if (v[i].isCollinear(v[j]))
        v.erase(v.begin() + j);
  }
}

Point rotatePointAroundAngle(double x, double y, Point* rot_point, double angle) {
  double radAngle = angle * PI / 180;
  double cosAngle = cos(radAngle);
  double sinAngle = sin(radAngle);
  x = rot_point->x + (x - rot_point->x) * cosAngle - (y - rot_point->y) * sinAngle;
  y = rot_point->y + (y - rot_point->y) * cosAngle + (x - rot_point->x) * sinAngle;
  return Point(x, y);
}