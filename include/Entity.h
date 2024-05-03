#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <vector>

#include "Maths.h"

using std::vector;

class Entity {
public:
  Entity();
  ~Entity();

  void init(SDL_Texture* p_tex, int width, int height, int frames, int lines);
  void move();
  void setX(double p_x);
  void setY(double p_y);
  void setWidth(int width);
  void setHeight(int height);
  void setCurrentFrame(int frame);
  void setFlip(SDL_RendererFlip p_flip);
  void setAngle(double angle);
  void setCenter(double x, double y);
  void setRotPoint(double x, double y);
  
  double getX();
  double getY();
  int getWidth();
  int getHeight();
  int getCurrentFrame();
  double getAngle();
  Point* getRotPoint();
  SDL_Texture* getTexture();
  SDL_Rect* getCurrentClip();
  SDL_RendererFlip getFlip();
  SDL_Point* getCenter();
  
protected:
  double x, y;
  int width, height;
  int currentFrame;
  double angle;
  SDL_Texture* texture;
  SDL_RendererFlip flip;
  SDL_Point* center;
  vector<SDL_Rect> clips;
  Point* rotPoint;
};