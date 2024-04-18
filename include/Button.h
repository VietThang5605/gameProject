#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"

class Button : public Entity {
public:
  void init(SDL_Texture* not_selected_texture, SDL_Texture* selected_texture);
  bool isClicked(int x, int y, SDL_Event &gEvent);
private:
  bool selected;
  SDL_Texture* not_selected_texture;
  SDL_Texture* selected_texture;
};