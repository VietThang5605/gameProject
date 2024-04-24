#include "Button.h"

void Button::init(SDL_Texture* not_selected_texture, SDL_Texture* selected_texture) {
  this->not_selected_texture = not_selected_texture;
  this->selected_texture = selected_texture;
  texture = not_selected_texture;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

bool Button::isClicked(int x, int y, bool mouseClicked) {
  if ((getX() <= x && x <= getX() + getWidth()) && (getY() <= y && y <= getY() + getHeight())) {
    selected = true;
    texture = selected_texture;
  }
  else {
    selected = false;
    texture = not_selected_texture;
  }
  if (selected && mouseClicked) {
    return true;
  }
  return false;
}