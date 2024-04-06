#pragma once

const int FPS = 30;
const int frameDelay = 1000 / FPS;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const double PI = 3.14159265358979323846;
const double INF = 1e9;

enum skill_ID {
  skillQ_ID,
  skillW_ID,
  skillE_ID,
  skillR_ID,
  skill_ID_Total
};

enum Player_Type {
  Human,
  Bot,
  Player_Type_Total
};