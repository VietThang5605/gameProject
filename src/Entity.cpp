#include "Entity.h"

Entity::Entity() {
  x = y = 0;
  width = height = 0;
  currentFrame = 0;
  texture = NULL;
  flip = SDL_FLIP_NONE;
  center = NULL;
}

Entity::~Entity() {}

void Entity::init(SDL_Texture* p_tex, int width, int height, int frames, int lines) {
  texture = p_tex;
  this->width = width;
  this->height = height;
  int w, h;
  SDL_QueryTexture(p_tex, NULL, NULL, &w, &h);
  w /= lines;
  h /= (frames + lines - 1) / lines;
  SDL_Rect clip;
  for (int i = 0; i < frames; i++) {
    clip = {w * i % (w * lines), (i / lines) * h, w, h};
    clips.push_back(clip);
  }
}

void Entity::setX(int p_x) {
  x = p_x;
}

void Entity::setY(int p_y) {
  y = p_y;
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

void Entity::setCenter(int x, int y) {
  center = new SDL_Point();
  center->x = x;
  center->y = y;
}

int Entity::getX() {
  return x;
}

int Entity::getY() {
  return y;
}

int Entity::getWidth() {
  return width;
}

int Entity::getHeight() {
  return height;
}

SDL_Texture* Entity::getTexture() {
  return texture;
}

int Entity::getCurrentFrame() {
  return currentFrame;
}

SDL_Rect* Entity::getCurrentClip() {
  return &clips[currentFrame];
}

SDL_RendererFlip Entity::getFlip() {
  return flip;
}

double Entity::getAngle() {
  return angle;
}

SDL_Point* Entity::getCenter() {
  return center;
}

void Entity::move() {
  currentFrame = (currentFrame + 1) % (int)clips.size();
}