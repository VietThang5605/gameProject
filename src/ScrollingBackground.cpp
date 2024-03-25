#include "ScrollingBackground.h"

ScrollingBackground::ScrollingBackground() {
  texture = NULL;
  scrollingOffset = 0;
  width = height = 0;
}
ScrollingBackground::~ScrollingBackground() {};

void ScrollingBackground::init(SDL_Texture* p_tex, int offset) {
  texture = p_tex;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  scrollingOffset = offset;
}

SDL_Texture* ScrollingBackground::getTexture() {
  return texture;
}

int ScrollingBackground::getScrollingOffset() {
  return scrollingOffset;
}

int ScrollingBackground::getWidth() {
  return width;
}

void ScrollingBackground::scroll(int distance) {
  scrollingOffset -= distance;
  if (scrollingOffset < 0) scrollingOffset = width;
}