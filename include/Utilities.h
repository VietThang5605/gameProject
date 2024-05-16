#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <string.h>

#include "Constants.h"

using std::string;

void logError(const char* msg, const char* error);

string IntToString(int x);

string TimeToString(int Timer, int ConvertType);

template<class T> T max(T a, T b) {
  if (a > b) {
    return a;
  }
  return b;
}

template<class T> T min(T a, T b) {
  if (a < b) {
    return a;
  }
  return b;
}