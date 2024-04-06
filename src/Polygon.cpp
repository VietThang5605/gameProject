#include "Polygon.h"

///Rectangle
Rectangle::Rectangle(double x, double y, double w, double h, double angle, Point* rotPoint) {
  coords = Point(x, y);
  vertices.push_back(Point(x, y));
  vertices.push_back(Point(x, y + h));
  vertices.push_back(Point(x + w, y + h));
  vertices.push_back(Point(x + w, y));
  if (angle != 0) {
    if (rotPoint != NULL)
      center = *rotPoint;
    else
      center = Point(x + w / 2, y + h / 2);
    double radAngle = angle * PI / 180;
    double cosAngle = cos(radAngle);
    double sinAngle = sin(radAngle);
    // cout << angle << ' ' << '(' << center.x << ", " << center.y << ')' << '\n';
    for (Point &vertex: vertices) {
      vertex.x = center.x + (vertex.x - center.x) * cosAngle - (vertex.y - center.y) * sinAngle;
      vertex.y = center.y + (vertex.y - center.y) * cosAngle + (vertex.x - center.x) * sinAngle;
      // cout << '(' << vertex.x << ", " << vertex.y << ") ";
    }
    // cout << '\n';
  }
  for (int i = 0; i < (int)vertices.size(); i++) {
    double xAxis = vertices[(i + 1) % (int)vertices.size()].x - vertices[i].x;
    double yAxis = vertices[(i + 1) % (int)vertices.size()].y - vertices[i].y;
    axis.push_back(Vector(xAxis, yAxis));
  }
  removeRepeat(axis);
}

///Circle


///utility
void removeRepeat(vector<Vector> &v) {
  for (int i = 0; i < (int)v.size(); i++) {
    for (int j = i + 1; j < (int)v.size(); j++)
      if (v[i].isCollinear(v[j]))
        v.erase(v.begin() + j);
  }
  // for (int i = 0; i < (int)v.size(); i++)
  //   std::cout << '<' << v[i].x << ", " << v[i].y << "> ";
  // cout << '\n';
}