#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class ScrollingBackground {
public:
  ScrollingBackground();
  ~ScrollingBackground();
  void init(SDL_Texture* p_tex, int offset);
  SDL_Texture* getTexture();
  int getScrollingOffset();
  int getWidth();
  void scroll(int distance);
private:
  SDL_Texture* texture;
  int scrollingOffset;
  int width, height;
};