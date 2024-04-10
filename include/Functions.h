#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <string.h>

using std::string;

void logError(const char* msg, const char* error);

string IntToString(int x);

string TimeToString(double x);