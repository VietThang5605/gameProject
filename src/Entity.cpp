#include "Entity.h"

Entity::Entity() {
  x = y = 0;
  width = height = 0;
  currentFrame = 0;
  texture = NULL;
  flip = SDL_FLIP_NONE;
  // center = NULL;
  center.x = center.y = 0;
  rotPoint.x = rotPoint.y = -INT_INF;
}

void Entity::init(SDL_Texture* p_tex, int width, int height, int frames, int lines) {
  int w, h;
  SDL_QueryTexture(p_tex, NULL, NULL, &w, &h);
  if (width == 0 && height == 0) {
    width = w;
    height = h;
  }
  texture = p_tex;
  this->width = width;
  this->height = height;
  if (frames == 0 || lines == 0)
    return;
  w /= lines;
  h /= (frames + lines - 1) / lines;
  SDL_Rect clip;
  for (int i = 0; i < frames; i++) {
    clip = {w * i % (w * lines), (i / lines) * h, w, h};
    clips.push_back(clip);
  }
}

void Entity::setX(double p_x) {
  x = p_x;
}

void Entity::setY(double p_y) {
  y = p_y;
}

void Entity::setWidth(int width) {
  this->width = width;
}

void Entity::setHeight(int height) {
  this->height = height;
}

void Entity::setCurrentFrame(int frame) {
  currentFrame = (frame + (int)clips.size()) % (int)clips.size();
}

void Entity::setFlip(SDL_RendererFlip p_flip) {
  flip = p_flip;
}

void Entity::setAngle(double p_angle) {
  angle = p_angle;
}

void Entity::setCenter(double x, double y) {
  // center = new SDL_Point();
  center.x = x;
  center.y = y;
}

void Entity::setRotPoint(double x, double y) {
  if (rotPoint.x == -INT_INF) rotPoint = Point(x, y);
  else {
    rotPoint.setX(x);
    rotPoint.setY(y);
  }
}

void Entity::setTexture(SDL_Texture* p_tex) {
  texture = p_tex;
}

double Entity::getX() {
  return x;
}

double Entity::getY() {
  return y;
}

int Entity::getWidth() {
  return width;
}

int Entity::getHeight() {
  return height;
}

int Entity::getCurrentFrame() {
  return currentFrame;
}

double Entity::getAngle() {
  return angle;
}

Point Entity::getRotPoint() {
  return rotPoint;
}

SDL_Texture* Entity::getTexture() {
  return texture;
}

SDL_Rect* Entity::getCurrentClip() {
  return &clips[currentFrame];
}

SDL_RendererFlip Entity::getFlip() {
  return flip;
}

SDL_Point* Entity::getCenter() {
  return &center;
}

void Entity::move() {
  currentFrame = (currentFrame + 1) % (int)clips.size();
}