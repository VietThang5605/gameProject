#pragma once

const int FPS = 40;
const int frameDelay = 1000 / FPS;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int ScreenLeftBoundary = 50;
const int ScreenRightBoundary = SCREEN_WIDTH - 200;

const int INT_INF = 1e9;

const double PI = 3.14159265358979323846;
const double DOUBLE_INF = 1e9;

const double maxAngle = 60;
const double angleDelta = 3;
const double angleDeltaMax = 6;
const int increaseAngleDeltaCooldown = 5 * FPS;

const int vulnerableTime = 2 * FPS;

const int PlayerStartHealth = 30;
const int PlayerVelocity = 18;
const int PlayerTeleportVelocity = 540;

enum skill_ID {
  skillQ_ID,
  skillW_ID,
  skillE_ID,
  skillR_ID,
  skill_ID_Total
};

const int skill_cooldown_start[skill_ID_Total] = {
  1 * FPS,
  3 * FPS,
  3 * FPS,
  15 * FPS,
};

const int skill_cooldown[skill_ID_Total] = {
  3 * FPS,
  6 * FPS,
  10 * FPS,
  30 * FPS,
};

const int skill_castTime[skill_ID_Total] = {
  FPS / 4,
  FPS / 4,
  FPS / 2,
  FPS
};

const int skill_damage[skill_ID_Total] = {
  2,
  2,
  0,
  6,
};

enum music_ID {
  GameEnd_music_ID,
  music_ID_Total,
};

enum sfx_ID {
  Click_sfx_ID,
  Count_down_sfx_ID,
  Count_down_start_sfx_id,
  Q1_sfx_ID,
  Q2_sfx_ID,
  Q_hit_sfx_ID,
  W_sfx_ID,
  W_hit_sfx_ID,
  W_hit_crashed_sfx_ID,
  E_sfx_ID,
  R_sfx_ID,
  R_hit_sfx_ID,
  sfx_ID_Total
};

enum ButtonName {
  VSAI_Button,
  VSPlayer_Button,
  Help_Button,
  Exit_Button,
  Continue_Button,
  Restart_Button,
  PlayAgain_Button,
  BackToMenu_Button,
  Button_Total
};

enum Player_Type {
  Bot,
  Human,
  Player_Type_Total
};

enum AI_Move_Type {
  Do_Nothing,
  Move_Left, Move_Right,
  Use_Q, Use_W, Use_E_Left, Use_E_Right, Use_R,
  AI_Move_Type_Total
};