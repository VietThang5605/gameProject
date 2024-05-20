#pragma once

/// General
const int FPS = 40;
const int frameDelay = 1000 / FPS;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int INT_INF = 1e9;

const double PI = 3.14159265358979323846;
const double DOUBLE_INF = 1e9;

enum GameMode {
  Classic_Mode,
  Special_Mode,
  GameMode_Total
};

///Utilites
enum TimeConvertType {
  Skill_TimeConvertType,
  Countdown_TimeConvertType,
  TimeConvertType_Total
};

/// Button
enum ButtonName {
  VSAI_Button, VSPlayer_Button,
  Help_Button,
  Exit_Button,
  Continue_Button, Restart_Button, PlayAgain_Button,
  BackToMenu_Button,
  Normal_Button, Hard_Button, Impossible_Button,
  Back_Button, Next_Button,
  Classic_Button, Special_Button,
  Button_Total
};

/// SFX and Music
enum music_ID {
  GameEnd_music_ID,
  music_ID_Total,
};

enum sfx_ID {
  Click_sfx_ID,
  Count_down_sfx_ID, Count_down_start_sfx_id,
  Q1_sfx_ID, Q2_sfx_ID, Q_hit_sfx_ID,
  W_sfx_ID, W_hit_sfx_ID, W_hit_crashed_sfx_ID,
  E_sfx_ID,
  R_sfx_ID, R_hit_sfx_ID,
  Ghost_start_sfx_ID, Ghost_end_sfx_ID,
  Hourglass_start_sfx_ID, Hourglass_end_sfx_ID,
  sfx_ID_Total
};

///PlayerArrow
const int ArrowDistanceToPlayer = 10;

/// Player
const int PlayerScreenLeftBoundary = 50;
const int PlayerScreenRightBoundary = SCREEN_WIDTH - 200;

const double maxAngle = 60;
const double angleDelta = 3;
const double angleDeltaMax = 6;
const int increaseAngleDeltaCooldown = 5 * FPS;

const int vulnerableTime = 2 * FPS;

const int PlayerStartHealth = 30;
const int PlayerVelocity = 18;
const int PlayerTeleportVelocity = 540;

const int PlayerBonusVelocity = 6;

enum skill_ID {
  skillQ_ID, skillW_ID, skillE_ID, skillR_ID,
  skillGhost_ID,
  skillHourglass_ID,
  skill_ID_Total
};

const int skill_cooldown_start[skill_ID_Total] = {
  1 * FPS, 3 * FPS, 3 * FPS, 15 * FPS,
  10 * FPS,
  30 * FPS,
};

const int skill_cooldown[skill_ID_Total] = {
  3 * FPS, 6 * FPS, 10 * FPS, 30 * FPS,
  30 * FPS,
  50 * FPS
};

const int skill_castTime[skill_ID_Total] = {
  FPS / 4, FPS / 4, FPS / 2, FPS,
  0,
  2 * FPS
};

const int skill_damage[skill_ID_Total] = {
  2, 2, 0, 6,
  0,
  0
};

const int skill_effectTime[skill_ID_Total] = {
  0, 0, 0, 0,
  5 * FPS,
  2 * FPS
};

const int SummonerSpellStartID = skillGhost_ID;
const int ItemStartID = skillHourglass_ID;

/// AI
enum AIGameMode {
  AIGameMode_Normal,
  AIGameMode_Hard,
  AIGameMode_Impossible,
  AIGameMode_Total,
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

enum HeuristicValue {
  ShootSkillQ_Value = 200,
  ShootSkillW_Value = 250,
  ShootSkillR_Value = 500,
  ShootedBySkillQ_Value = 3000,
  ShootedBySkillR_Value = 5000,
};

enum HeuristicValueID {
  Distance_To_Screen_Bound,
  HeuristicValueID_Total
};

const int Distance_To_Shoot[skill_ID_Total][AIGameMode_Total] = {
  {60, 70, 70}, ///skillQ
  {60, 70, 70}, ///skillW
  {0, 0, 0}, ///skillE
  {250, 200, 200} ///skillR
};

const int HeuristicValueByMode[HeuristicValueID_Total][AIGameMode_Total] = {
  {-INT_INF, 300, 400} ///Distance_To_Screen_Bound
};
