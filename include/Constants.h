#pragma once

const int FPS = 40;
const int frameDelay = 1000 / FPS;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const double PI = 3.14159265358979323846;
const double INF = 1e9;

const double maxAngle = 70;
const int increaseAngleDeltaCooldown = 5 * FPS;

const int vulnerableTime = 2 * FPS;

enum skill_ID {
  skillQ_ID,
  skillW_ID,
  skillE_ID,
  skillR_ID,
  skill_ID_Total
};

const int skill_Cooldown[skill_ID_Total] = {
  3 * FPS,
  6 * FPS,
  10 * FPS,
  30 * FPS
};

const int skill_castTime[skill_ID_Total] = {
  FPS / 4,
  FPS / 4,
  FPS / 2,
  FPS
};

enum sfx_ID {
  Q1_sfx_ID,
  Q2_sfx_ID,
  Q_hit_sfx_ID,
  W_sfx_ID,
  W_hit_sfx_ID,
  W_hit_crashed_sfx_ID,
  E_sfx_ID,
  sfx_ID_Total
};

enum ButtonName {
  VSAI_Button,
  VSPlayer_Button,
  Help_Button,
  Exit_Button,
  Button_Total
};

enum Player_Type {
  Human,
  Bot,
  Player_Type_Total
};