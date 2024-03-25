#pragma once;

#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using std::vector;

class Entity {
public:
  Entity();
  ~Entity();

  void init(SDL_Texture* p_tex, int width, int height, int frames, int lines);
  void move();
  void setX(int p_x);
  void setY(int p_y);
  void setCurrentFrame(int frame);
  void setFlip(SDL_RendererFlip p_flip);
  void setAngle(double angle);
  void setCenter(int x, int y);
  
  int getX();
  int getY();
  int getWidth();
  int getHeight();
  int getCurrentFrame();
  double getAngle();
  SDL_Texture* getTexture();
  SDL_Rect* getCurrentClip();
  SDL_RendererFlip getFlip();
  SDL_Point* getCenter();
  
private:
  int x, y;
  int width, height;
  int currentFrame;
  double angle;
  SDL_Texture* texture;
  SDL_RendererFlip flip;
  SDL_Point* center;
  vector<SDL_Rect> clips;
};