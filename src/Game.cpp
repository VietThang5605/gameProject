#include "Game.h"

enum GameState {
  Credit,
  MainMenu,
  GamePlay,
  GamePause,
  Help_gameState_start, Help1, Help2, Help3, Help4, Help5, Help6, Help_gameState_end,
  YouWin, YouLose, Player1Win, Player2Win, GameDraw,
  SelectAIMode, SelectMode, SelectItemMode,
  GameState_Total
};

enum KeyPress {
  KEY_PRESS_LEFT, KEY_PRESS_RIGHT,
  KEY_PRESS_U, KEY_PRESS_I, KEY_PRESS_O, KEY_PRESS_P, KEY_PRESS_7, KEY_PRESS_8, KEY_PRESS_9,

  KEY_PRESS_A, KEY_PRESS_D,
  KEY_PRESS_E, KEY_PRESS_R, KEY_PRESS_T, KEY_PRESS_Y, KEY_PRESS_3, KEY_PRESS_4, KEY_PRESS_5,

  KEY_PRESS_ESC, KEY_PRESS_F11, KEY_PRESS_ENTER,
  KEY_PRESS_TOTAL
};

int old_key[KEY_PRESS_TOTAL];
int new_key[KEY_PRESS_TOTAL];

const Uint8* keystate = SDL_GetKeyboardState(NULL);

int MouseX, MouseY;
bool mouseClicked;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

SDL_Color white = {255, 255, 255};
SDL_Color black = {0, 0, 0};

enum Font_ID {
  font32, font32_outline,
  font40,
  font50,
  font80,
  font100,
  Font_ID_Total
};

TTF_Font* Font[Font_ID_Total];

Mix_Chunk* SFX[sfx_ID_Total] = { NULL };
Mix_Music* MUSIC[music_ID_Total] = { NULL };

enum GameTextureName {
  waterTexture,
  grassTexture,
  ezrealTexture, ezrealHourglassTexture,
  skillArrow,
  skillQ,
  skillW,
  skillW_ground,
  skillR,
  GameTexture_Total
};

enum Health_Hud {
  HealthBar_Hud_ID,
  HealthColor_Hud_ID,
  Health_Hud_Total
};

enum AnimationName {
  count_down1,
  count_down2,
  count_down3,
  Animation_Total
};

enum CreditName {
  Main_Credit,
  Sub_Credit,
  Credit_Total
};

enum IconName {
  Github_Icon,
  Icon_Total
};

SDL_Texture* GameTexture[GameTexture_Total] = { NULL };
SDL_Texture* health_Hud[Health_Hud_Total] = { NULL };
SDL_Texture* skill_Hud[skill_ID_Total] = { NULL };
SDL_Texture* GameAnimation[Animation_Total] = { NULL };
SDL_Texture* GameCredit[Credit_Total] = { NULL };
SDL_Texture* GameIcon[Icon_Total] = { NULL };
SDL_Texture* Background[GameState_Total] = { NULL };
SDL_Texture* Button_Texture[Button_Total][2] = { NULL };

vector<Entity> waterBackground;
Entity CreditEntity[Credit_Total];
Entity Icon[Icon_Total];
Entity player_health_Hud[3][Health_Hud_Total];
Entity player_skill_Hud[3][skill_ID_Total];
Entity skillW_effect;

Button button[Button_Total];

PlayerArrow player_Arrow[3];
Player player[3];

Bullet bullet[skill_ID_Total];
vector<Bullet> player_bullets[3];

Game::Game() {
  gWindow = NULL;
  gRenderer = NULL;
  gameRunning = true;
  gameState = Credit;
  fullscreen = false;
  gameDelayTime = 4 * FPS;
};

void Game::resetGameDelayTime(int time) {
  gameDelayTime = time;
}

void Game::resetGame() {
  resetGameDelayTime(3 * FPS);
  UsingItemTime = UsingItemTime_Start;
  TimeBeforeRandomItem = TimeBeforeRandomItem_Start;
  gameEndDelayTime = 2 * FPS;
  gameTimer = 3 * 60 * FPS;
  player[1].reset(player[1].getType());
  player[2].reset(player[2].getType());
  for (int id = 1; id <= 2; id++) {
    player_Arrow[id].reset();
    player_bullets[id].erase(player_bullets[id].begin(), player_bullets[id].end());
  }
}

void Game::init() {
  initSDL();
  loadMedia();
}

void Game::initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING & ~(SDL_INIT_TIMER | SDL_INIT_HAPTIC))!= 0)
    logError("Failed to initialize SDL.", SDL_GetError());

  gWindow = SDL_CreateWindow("Ezreal Mirror Shooting", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
  if (gWindow == NULL)
    logError("Failed to create window.", SDL_GetError());

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (gRenderer == NULL)
    logError("Failted to create renderer.", SDL_GetError());

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
    logError("Failed to initialize SDL_image.", IMG_GetError());

  if (TTF_Init() != 0)
    logError("Failed to initialize SDL_ttf.", TTF_GetError());

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
    logError("Failed to initialize SDL_mixer.", Mix_GetError());
  Mix_AllocateChannels(24);
}

void Game::loadMedia() {
  ///Fonts
  Font[font32] = loadFont("res/fonts/cocogoose.ttf", 32);
  Font[font32_outline] = loadFont("res/fonts/cocogoose.ttf", 32); TTF_SetFontOutline(Font[font32_outline], 1);
  Font[font40] = loadFont("res/fonts/DTM-Sans.ttf", 40);
  Font[font50] = loadFont("res/fonts/DTM-Sans.ttf", 50);
  Font[font80] = loadFont("res/fonts/DTM-Sans.ttf", 80);
  Font[font100] = loadFont("res/fonts/DTM-Sans.ttf", 100);

  ///Textures
  health_Hud[HealthBar_Hud_ID] = loadTexture("res/images/hud/HealthBar.png");
  health_Hud[HealthColor_Hud_ID] = loadTexture("res/images/hud/HealthColor.png");
  skill_Hud[skillQ_ID] = loadTexture("res/images/hud/ezreal_q.png");
  skill_Hud[skillW_ID] = loadTexture("res/images/hud/ezreal_w.png");
  skill_Hud[skillE_ID] = loadTexture("res/images/hud/ezreal_e.png");
  skill_Hud[skillR_ID] = loadTexture("res/images/hud/ezreal_r.png");
  skill_Hud[skillR_ID] = loadTexture("res/images/hud/ezreal_r.png");
  skill_Hud[skillGhost_ID] = loadTexture("res/images/hud/Ghost.png");
  skill_Hud[skillHourglass_ID] = loadTexture("res/images/hud/Zhonya's_Hourglass.png");
  skill_Hud[skillFlash_ID] = loadTexture("res/images/hud/Flash.png");
  skill_Hud[skillHeal_ID] = loadTexture("res/images/hud/Heal.png");
  skill_Hud[skillVoltaic_Cyclosword_ID] = loadTexture("res/images/hud/Voltaic_Cyclosword.png");
  // skill_Hud[skillThe_Golden_Spatula_ID] = loadTexture("res/images/hud/The_Golden_Spatula.png");
  skill_Hud[skillMystery_Box_ID] = loadTexture("res/images/hud/Mystery_Box.png");

  GameTexture[waterTexture] = loadTexture("res/images/background/Water.png");
  GameTexture[grassTexture] = loadTexture("res/images/background/Grass.png");
  GameTexture[ezrealTexture] = loadTexture("res/images/character/ezreal-Sheet.png");
  GameTexture[ezrealHourglassTexture] = loadTexture("res/images/character/ezreal-Sheet-Hourglass.png");

  GameTexture[skillArrow] = loadTexture("res/images/skill/skillArrow.png");
  GameTexture[skillQ] = loadTexture("res/images/skill/skillQ.png");
  GameTexture[skillW] = loadTexture("res/images/skill/skillW.png");
  GameTexture[skillW_ground] = loadTexture("res/images/skill/skillW_ground.png");
  GameTexture[skillR] = loadTexture("res/images/skill/skillR.png");

  GameAnimation[count_down1] = loadTexture("res/images/animation/count_down/count_down1.png");
  GameAnimation[count_down2] = loadTexture("res/images/animation/count_down/count_down2.png");
  GameAnimation[count_down3] = loadTexture("res/images/animation/count_down/count_down3.png");

  Background[Credit] = loadTexture("res/images/background/Grass_background.png");
  Background[MainMenu] = loadTexture("res/images/background/MainMenu_background.png");
  Background[Help1] = loadTexture("res/images/background/Help1_background.png");
  Background[Help2] = loadTexture("res/images/background/Help2_background.png");
  Background[Help3] = loadTexture("res/images/background/Help3_background.png");
  Background[Help4] = loadTexture("res/images/background/Help4_background.png");
  Background[Help5] = loadTexture("res/images/background/Help5_background.png");
  Background[Help6] = loadTexture("res/images/background/Help6_background.png");
  Background[GamePause] = loadTexture("res/images/background/GamePause_background.png");
  Background[YouWin] = loadTexture("res/images/background/YouWin_background.png");
  Background[YouLose] = loadTexture("res/images/background/YouLose_background.png");
  Background[Player1Win] = loadTexture("res/images/background/Player1Win_background.png");
  Background[Player2Win] = loadTexture("res/images/background/Player2Win_background.png");
  Background[GameDraw] = loadTexture("res/images/background/GameDraw_background.png");
  Background[SelectAIMode] = loadTexture("res/images/background/MainMenu_background.png");
  Background[SelectMode] = loadTexture("res/images/background/MainMenu_background.png");
  Background[SelectItemMode] = loadTexture("res/images/background/MainMenu_background.png");

  GameCredit[Main_Credit] = loadTextureFromText("Github.com/VietThang5605", Font[font40], black);
  CreditEntity[Main_Credit].init(GameCredit[Main_Credit], 0, 0, 0, 0);
  CreditEntity[Main_Credit].setX(SCREEN_WIDTH / 2 - CreditEntity[Main_Credit].getWidth() / 2 + 30);
  CreditEntity[Main_Credit].setY(SCREEN_HEIGHT / 2 - CreditEntity[Main_Credit].getHeight());

  GameCredit[Sub_Credit] = loadTextureFromText("Made with C++ and SDL2", Font[font40], black);
  CreditEntity[Sub_Credit].init(GameCredit[Sub_Credit], 0, 0, 0, 0);
  CreditEntity[Sub_Credit].setX(CreditEntity[Main_Credit].getX() + CreditEntity[Main_Credit].getWidth() / 2 - CreditEntity[Sub_Credit].getWidth() / 2);
  CreditEntity[Sub_Credit].setY(CreditEntity[Main_Credit].getY() + CreditEntity[Main_Credit].getHeight());

  GameIcon[Github_Icon] = loadTexture("res/images/icon/Github_Icon.png");
  Icon[Github_Icon].init(GameIcon[Github_Icon], 128, 128, 1, 1);
  Icon[Github_Icon].setX(CreditEntity[Main_Credit].getX() - Icon[Github_Icon].getWidth());
  Icon[Github_Icon].setY(CreditEntity[Main_Credit].getY() + CreditEntity[Main_Credit].getHeight() / 2 - Icon[Github_Icon].getHeight() / 2);

  Button_Texture[VSAI_Button][0] = loadTextureFromText("VS AI", Font[font80], black);
  Button_Texture[VSAI_Button][1] = loadTextureFromText("VS AI", Font[font80], white);
  button[VSAI_Button].init(Button_Texture[VSAI_Button][0], Button_Texture[VSAI_Button][1]);

  Button_Texture[VSPlayer_Button][0] = loadTextureFromText("VS Player", Font[font80], black);
  Button_Texture[VSPlayer_Button][1] = loadTextureFromText("VS Player", Font[font80], white);
  button[VSPlayer_Button].init(Button_Texture[VSPlayer_Button][0], Button_Texture[VSPlayer_Button][1]);

  Button_Texture[Help_Button][0] = loadTextureFromText("Help", Font[font80], black);
  Button_Texture[Help_Button][1] = loadTextureFromText("Help", Font[font80], white);
  button[Help_Button].init(Button_Texture[Help_Button][0], Button_Texture[Help_Button][1]);

  Button_Texture[Exit_Button][0] = loadTextureFromText("Exit", Font[font80], black);
  Button_Texture[Exit_Button][1] = loadTextureFromText("Exit", Font[font80], white);
  button[Exit_Button].init(Button_Texture[Exit_Button][0], Button_Texture[Exit_Button][1]);

  Button_Texture[Continue_Button][0] = loadTextureFromText("Continue", Font[font80], black);
  Button_Texture[Continue_Button][1] = loadTextureFromText("Continue", Font[font80], white);
  button[Continue_Button].init(Button_Texture[Continue_Button][0], Button_Texture[Continue_Button][1]);

  Button_Texture[Restart_Button][0] = loadTextureFromText("Restart", Font[font80], black);
  Button_Texture[Restart_Button][1] = loadTextureFromText("Restart", Font[font80], white);
  button[Restart_Button].init(Button_Texture[Restart_Button][0], Button_Texture[Restart_Button][1]);

  Button_Texture[PlayAgain_Button][0] = loadTextureFromText("Play Again", Font[font80], black);
  Button_Texture[PlayAgain_Button][1] = loadTextureFromText("Play Again", Font[font80], white);
  button[PlayAgain_Button].init(Button_Texture[PlayAgain_Button][0], Button_Texture[PlayAgain_Button][1]);

  Button_Texture[BackToMenu_Button][0] = loadTextureFromText("Back to menu", Font[font80], black);
  Button_Texture[BackToMenu_Button][1] = loadTextureFromText("Back to menu", Font[font80], white);
  button[BackToMenu_Button].init(Button_Texture[BackToMenu_Button][0], Button_Texture[BackToMenu_Button][1]);

  Button_Texture[Normal_Button][0] = loadTextureFromText("Normal", Font[font80], black);
  Button_Texture[Normal_Button][1] = loadTextureFromText("Normal", Font[font80], white);
  button[Normal_Button].init(Button_Texture[Normal_Button][0], Button_Texture[Normal_Button][1]);

  Button_Texture[Hard_Button][0] = loadTextureFromText("Hard", Font[font80], black);
  Button_Texture[Hard_Button][1] = loadTextureFromText("Hard", Font[font80], white);
  button[Hard_Button].init(Button_Texture[Hard_Button][0], Button_Texture[Hard_Button][1]);

  Button_Texture[Impossible_Button][0] = loadTextureFromText("Impossible", Font[font80], black);
  Button_Texture[Impossible_Button][1] = loadTextureFromText("Impossible", Font[font80], white);
  button[Impossible_Button].init(Button_Texture[Impossible_Button][0], Button_Texture[Impossible_Button][1]);

  Button_Texture[Back_Button][0] = loadTextureFromText("Back", Font[font80], black);
  Button_Texture[Back_Button][1] = loadTextureFromText("Back", Font[font80], white);
  button[Back_Button].init(Button_Texture[Back_Button][0], Button_Texture[Back_Button][1]);

  Button_Texture[Next_Button][0] = loadTextureFromText("Next", Font[font80], black);
  Button_Texture[Next_Button][1] = loadTextureFromText("Next", Font[font80], white);
  button[Next_Button].init(Button_Texture[Next_Button][0], Button_Texture[Next_Button][1]);

  Button_Texture[Classic_Button][0] = loadTextureFromText("Classic", Font[font80], black);
  Button_Texture[Classic_Button][1] = loadTextureFromText("Classic", Font[font80], white);
  button[Classic_Button].init(Button_Texture[Classic_Button][0], Button_Texture[Classic_Button][1]);

  Button_Texture[Special_Button][0] = loadTextureFromText("Special", Font[font80], black);
  Button_Texture[Special_Button][1] = loadTextureFromText("Special", Font[font80], white);
  button[Special_Button].init(Button_Texture[Special_Button][0], Button_Texture[Special_Button][1]);

  Button_Texture[Gacha_Button][0] = loadTextureFromText("Gacha", Font[font80], black);
  Button_Texture[Gacha_Button][1] = loadTextureFromText("Gacha", Font[font80], white);
  button[Gacha_Button].init(Button_Texture[Gacha_Button][0], Button_Texture[Gacha_Button][1]);

  Button_Texture[All_Random_Button][0] = loadTextureFromText("All Random", Font[font80], black);
  Button_Texture[All_Random_Button][1] = loadTextureFromText("All Random", Font[font80], white);
  button[All_Random_Button].init(Button_Texture[All_Random_Button][0], Button_Texture[All_Random_Button][1]);

  ///Sounds
  SFX[Click_sfx_ID] = loadSFX("res/audio/sfx/Click.wav");
  SFX[Count_down_sfx_ID] = loadSFX("res/audio/sfx/Count_down.wav");
  SFX[Count_down_start_sfx_ID] = loadSFX("res/audio/sfx/Count_down_start.wav");
  SFX[Q1_sfx_ID] = loadSFX("res/audio/sfx/Q1.wav");
  SFX[Q2_sfx_ID] = loadSFX("res/audio/sfx/Q2.wav");
  SFX[Q_hit_sfx_ID] = loadSFX("res/audio/sfx/Q_hit.wav");
  SFX[W_sfx_ID] = loadSFX("res/audio/sfx/W.wav");
  SFX[W_hit_sfx_ID] = loadSFX("res/audio/sfx/W_hit.wav");
  SFX[W_hit_crashed_sfx_ID] = loadSFX("res/audio/sfx/W_hit_crashed.wav");
  SFX[E_sfx_ID] = loadSFX("res/audio/sfx/E.wav");
  SFX[R_sfx_ID] = loadSFX("res/audio/sfx/R.wav");
  SFX[R_hit_sfx_ID] = loadSFX("res/audio/sfx/R_hit.wav");
  SFX[Ghost_start_sfx_ID] = loadSFX("res/audio/sfx/Ghost_start.wav");
  SFX[Ghost_end_sfx_ID] = loadSFX("res/audio/sfx/Ghost_end.wav");
  SFX[Hourglass_start_sfx_ID] = loadSFX("res/audio/sfx/Hourglass_start.wav");
  SFX[Hourglass_end_sfx_ID] = loadSFX("res/audio/sfx/Hourglass_end.wav");
  SFX[Flash_sfx_ID] = loadSFX("res/audio/sfx/Flash.wav");
  SFX[Heal_sfx_ID] = loadSFX("res/audio/sfx/Heal.wav");
  SFX[Voltaic_Cyclosword_sfx_ID] = loadSFX("res/audio/sfx/Voltaic_Cyclosword.wav");
  SFX[UsingItem_start_sfx_ID] = loadSFX("res/audio/sfx/UsingItem_start.wav");

  Mix_VolumeChunk(SFX[Click_sfx_ID], 80);
  Mix_VolumeChunk(SFX[Count_down_sfx_ID], 50);
  Mix_VolumeChunk(SFX[Count_down_start_sfx_ID], 50);
  Mix_VolumeChunk(SFX[Q1_sfx_ID], 60);
  Mix_VolumeChunk(SFX[Q2_sfx_ID], 60);
  Mix_VolumeChunk(SFX[W_sfx_ID], 60);
  Mix_VolumeChunk(SFX[E_sfx_ID], 60);
  Mix_VolumeChunk(SFX[R_sfx_ID], 80);
  Mix_VolumeChunk(SFX[Q_hit_sfx_ID], 60);
  Mix_VolumeChunk(SFX[W_hit_sfx_ID], 60);
  Mix_VolumeChunk(SFX[W_hit_crashed_sfx_ID], 60);
  Mix_VolumeChunk(SFX[R_hit_sfx_ID], 60);
  Mix_VolumeChunk(SFX[Ghost_start_sfx_ID], 50);
  Mix_VolumeChunk(SFX[Ghost_end_sfx_ID], 100);
  Mix_VolumeChunk(SFX[Hourglass_start_sfx_ID], 100);
  Mix_VolumeChunk(SFX[Hourglass_end_sfx_ID], 100);
  Mix_VolumeChunk(SFX[Flash_sfx_ID], 60);
  Mix_VolumeChunk(SFX[Heal_sfx_ID], 60);
  Mix_VolumeChunk(SFX[Voltaic_Cyclosword_sfx_ID], 100);

  MUSIC[GameEnd_music_ID] = loadMusic("res/audio/music/winSound.wav");

  ///entity's initialization
  skillW_effect.init(GameTexture[skillW_ground], 128, 64, 1, 1);
  bullet[skillQ_ID].init(GameTexture[skillQ], 4 * 9, 16 * 9, 1, 1, skillQ_ID);
  bullet[skillW_ID].init(GameTexture[skillW], 8 * 14, 8 * 14, 1, 1, skillW_ID);
  bullet[skillR_ID].init(GameTexture[skillR], 224, 224, 1, 1, skillR_ID);

  for (int j = 20; j < 45; j++)
    for (int i = 0; i < 32; i++) {
      Entity entity;
      entity.init(GameTexture[waterTexture], 64, 16, 1, 1);
      entity.setX(i * 64);
      entity.setY(j * 16);
      waterBackground.push_back(entity);
    }

  ///player1's initialization
  player[1].init(GameTexture[ezrealTexture], 54 * 3, 69 * 3, 8, 8);
  player[1].setStartXY(SCREEN_WIDTH / 2 - player[1].getWidth() / 2, 150);
  player[1].reset(Human);

  player_Arrow[1].init(GameTexture[skillArrow], 8 * 10, 16 * 10, 1, 1);
  player_Arrow[1].setCenter(player_Arrow[1].getWidth() / 2, 0);
  player_Arrow[1].setAngle(0);
  player_Arrow[1].setX(player[1].getX() + player[1].getWidth() / 2 - player_Arrow[1].getWidth() / 2);
  player_Arrow[1].setY(player[1].getY() + player[1].getHeight() + ArrowDistanceToPlayer);
  player_Arrow[1].setRotPoint(player_Arrow[1].getX() + player_Arrow[1].getWidth() / 2, player_Arrow[1].getY());

  player_health_Hud[1][HealthBar_Hud_ID].init(health_Hud[HealthBar_Hud_ID], 474, 96, 1, 1);
  player_health_Hud[1][HealthBar_Hud_ID].setX(30);
  player_health_Hud[1][HealthBar_Hud_ID].setY(120 - player_health_Hud[1][HealthBar_Hud_ID].getHeight());

  player_health_Hud[1][HealthColor_Hud_ID].init(health_Hud[HealthColor_Hud_ID], 458, 80, 1, 1);
  player_health_Hud[1][HealthColor_Hud_ID].setX(30 + 8);
  player_health_Hud[1][HealthColor_Hud_ID].setY(120 - player_health_Hud[1][HealthBar_Hud_ID].getHeight() + 8);

  player_skill_Hud[1][skillR_ID].init(skill_Hud[skillR_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillR_ID].setX(SCREEN_WIDTH - 30 - player_skill_Hud[1][skillR_ID].getWidth());
  player_skill_Hud[1][skillR_ID].setY(120 - player_skill_Hud[1][skillR_ID].getHeight());

  player_skill_Hud[1][skillE_ID].init(skill_Hud[skillE_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillE_ID].setX(player_skill_Hud[1][skillR_ID].getX() - player_skill_Hud[1][skillE_ID].getWidth() - 30);
  player_skill_Hud[1][skillE_ID].setY(120 - player_skill_Hud[1][skillE_ID].getHeight());

  player_skill_Hud[1][skillW_ID].init(skill_Hud[skillW_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillW_ID].setX(player_skill_Hud[1][skillE_ID].getX() - player_skill_Hud[1][skillW_ID].getWidth() - 30);
  player_skill_Hud[1][skillW_ID].setY(120 - player_skill_Hud[1][skillW_ID].getHeight());

  player_skill_Hud[1][skillQ_ID].init(skill_Hud[skillQ_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillQ_ID].setX(player_skill_Hud[1][skillW_ID].getX() - player_skill_Hud[1][skillQ_ID].getWidth() - 30);
  player_skill_Hud[1][skillQ_ID].setY(120 - player_skill_Hud[1][skillQ_ID].getHeight());

  player_skill_Hud[1][skillGhost_ID].init(skill_Hud[skillGhost_ID], 96, 96, 1, 1);
  player_skill_Hud[1][skillGhost_ID].setX(player_skill_Hud[1][skillQ_ID].getX() - player_skill_Hud[1][skillGhost_ID].getWidth() - 30);
  player_skill_Hud[1][skillGhost_ID].setY(120 - player_skill_Hud[1][skillGhost_ID].getHeight());

  for (int id = skillHourglass_ID; id < skill_ID_Total; id++) {
    player_skill_Hud[1][id].init(skill_Hud[id], 96, 96, 1, 1);
    player_skill_Hud[1][id].setX(player_skill_Hud[1][skillGhost_ID].getX() - player_skill_Hud[1][id].getWidth() - 30);
    player_skill_Hud[1][id].setY(120 - player_skill_Hud[1][id].getHeight());
  }

  ///player2's initialization
  player[2].init(GameTexture[ezrealTexture], 54 * 3, 69 * 3, 8, 8);
  player[2].setStartXY(SCREEN_WIDTH / 2 - player[2].getWidth() / 2, 720);
  player[2].reset(Human);

  player_Arrow[2].init(GameTexture[skillArrow], 8 * 10, 16 * 10, 1, 1);
  player_Arrow[2].setCenter(player_Arrow[2].getWidth() / 2, player_Arrow[2].getHeight());
  player_Arrow[2].setFlip(SDL_FLIP_VERTICAL);
  player_Arrow[2].setAngle(0);
  player_Arrow[2].setX(player[2].getX() + player[2].getWidth() / 2 - player_Arrow[2].getWidth() / 2);
  player_Arrow[2].setY(player[2].getY() - player_Arrow[2].getHeight() - ArrowDistanceToPlayer);
  player_Arrow[2].setRotPoint(player_Arrow[2].getX() + player_Arrow[2].getWidth() / 2, player_Arrow[2].getY() + player_Arrow[2].getHeight());

  player_health_Hud[2][HealthBar_Hud_ID].init(health_Hud[HealthBar_Hud_ID], 474, 96, 1, 1);
  player_health_Hud[2][HealthBar_Hud_ID].setX(SCREEN_WIDTH - 30 - player_health_Hud[2][HealthBar_Hud_ID].getWidth());
  player_health_Hud[2][HealthBar_Hud_ID].setY(SCREEN_HEIGHT - 120);

  player_health_Hud[2][HealthColor_Hud_ID].init(health_Hud[HealthColor_Hud_ID], 458, 80, 1, 1);
  player_health_Hud[2][HealthColor_Hud_ID].setX(SCREEN_WIDTH - 30 - player_health_Hud[2][HealthBar_Hud_ID].getWidth() + 8);
  player_health_Hud[2][HealthColor_Hud_ID].setY(SCREEN_HEIGHT - 120 + 8);

  player_skill_Hud[2][skillQ_ID].init(skill_Hud[skillQ_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillQ_ID].setX(30);
  player_skill_Hud[2][skillQ_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillW_ID].init(skill_Hud[skillW_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillW_ID].setX(player_skill_Hud[2][skillQ_ID].getX() + player_skill_Hud[2][skillQ_ID].getWidth() + 30);
  player_skill_Hud[2][skillW_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillE_ID].init(skill_Hud[skillE_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillE_ID].setX(player_skill_Hud[2][skillW_ID].getX() + player_skill_Hud[2][skillW_ID].getWidth() + 30);
  player_skill_Hud[2][skillE_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillR_ID].init(skill_Hud[skillR_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillR_ID].setX(player_skill_Hud[2][skillE_ID].getX() + player_skill_Hud[2][skillE_ID].getWidth() + 30);
  player_skill_Hud[2][skillR_ID].setY(SCREEN_HEIGHT - 120);

  player_skill_Hud[2][skillGhost_ID].init(skill_Hud[skillGhost_ID], 96, 96, 1, 1);
  player_skill_Hud[2][skillGhost_ID].setX(player_skill_Hud[2][skillR_ID].getX() + player_skill_Hud[2][skillR_ID].getWidth() + 30);
  player_skill_Hud[2][skillGhost_ID].setY(SCREEN_HEIGHT - 120);

  for (int id = skillHourglass_ID; id < skill_ID_Total; id++) {
    player_skill_Hud[2][id].init(skill_Hud[id], 96, 96, 1, 1);
    player_skill_Hud[2][id].setX(player_skill_Hud[2][skillGhost_ID].getX() + player_skill_Hud[2][skillGhost_ID].getWidth() + 30);
    player_skill_Hud[2][id].setY(SCREEN_HEIGHT - 120);
  }
}

void Game::closeMedia() {
  ///Fonts
  for (int id = 0; id < Font_ID_Total; id++) {
    TTF_CloseFont(Font[id]);
    Font[id] = NULL;
  }

  ///Textures
  for (int id = 0; id < Health_Hud_Total; id++) {
    SDL_DestroyTexture(health_Hud[id]);
    health_Hud[id] = NULL;
  }

  for (int id = 0; id < skill_ID_Total; id++) {
    SDL_DestroyTexture(skill_Hud[id]);
    skill_Hud[id] = NULL;
  }

  for (int id = 0; id < GameTexture_Total; id++) {
    SDL_DestroyTexture(GameTexture[id]);
    GameTexture[id] = NULL;
  }

  for (int id = 0; id < Animation_Total; id++) {
    SDL_DestroyTexture(GameAnimation[id]);
    GameAnimation[id] = NULL;
  }

  for (int id = 0; id < Credit_Total; id++) {
    SDL_DestroyTexture(GameCredit[id]);
    GameCredit[id] = NULL;
  }

  for (int id = 0; id < Icon_Total; id++) {
    SDL_DestroyTexture(GameIcon[id]);
    GameIcon[id] = NULL;
  }

  for (int id = 0; id < GameState_Total; id++) {
    SDL_DestroyTexture(Background[id]);
    Background[id] = NULL;
  }

  for (int id = 0; id < Button_Total; id++) {
    for (int state = 0; state <= 1; state++) {
      SDL_DestroyTexture(Button_Texture[id][state]);
      Button_Texture[id][state] = NULL;
    }
  }

  ///Sounds
  for (int id = 0; id < sfx_ID_Total; id++) {
    Mix_FreeChunk(SFX[id]);
    SFX[id] = NULL;
  }

  for (int id = 0; id < music_ID_Total; id++) {
    Mix_FreeMusic(MUSIC[id]);
    MUSIC[id] = NULL;
  }
}

void Game::display() {
  SDL_RenderPresent(gRenderer);
}

void Game::clear() {
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
  SDL_RenderClear(gRenderer);
}

void Game::cleanUp() {
  closeMedia();
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
  Mix_CloseAudio();
}

bool Game::running() {
  return gameRunning;
}

SDL_Texture* Game::loadTexture(const char* p_filePath) {
  SDL_Texture* texture = NULL;
  texture = IMG_LoadTexture(gRenderer, p_filePath);

  if (texture == NULL)
    logError("Failed to load texture.", IMG_GetError());

  return texture;
}

SDL_Texture* Game::loadTextureFromText(const char* p_text, TTF_Font* font, SDL_Color textColor) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = NULL;
  message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  if (message == NULL)
    logError("Failed to create texture from surface.", SDL_GetError());

  SDL_FreeSurface(surfaceMessage);
  return message;
}

SDL_Texture* Game::loadTextureFromText(string& p_text, TTF_Font* font, SDL_Color textColor) {
  char* text = &p_text[0];
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
  SDL_Texture* message = NULL;
  message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  if (message == NULL)
    logError("Failed to create texture from surface.", SDL_GetError());

  SDL_FreeSurface(surfaceMessage);
  return message;
}

Mix_Chunk* Game::loadSFX(const char* p_filePath) {
  Mix_Chunk* sound = NULL;
  sound = Mix_LoadWAV(p_filePath);

  if (sound == NULL)
    logError("Failed to load sound effect.", Mix_GetError());

  return sound;
}

Mix_Music* Game::loadMusic(const char* p_filePath) {
  Mix_Music* music = NULL;
  music = Mix_LoadMUS(p_filePath);

  if (music == NULL)
    logError("Failed to load music.", Mix_GetError());

  return music;
}

TTF_Font* Game::loadFont(const char* p_filePath, int Size) {
  TTF_Font* font = NULL;
  font = TTF_OpenFont(p_filePath, Size);

  if (font == NULL)
    logError("Failed to load font.", TTF_GetError());

  return font;
}

void Game::render(SDL_Texture* p_tex, double x, double y, double w, double h, double angle, SDL_Point* center, SDL_RendererFlip flip) {
  SDL_Rect src;
  src.x = src.y = 0;
  SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  if (w != 0 && h != 0) {
    dst.w = w;
    dst.h = h;
  }
  else {
    dst.w = src.w;
    dst.h = src.h;
  }
  SDL_RenderCopyEx(gRenderer, p_tex, &src, &dst, angle, center, flip);
}

void Game::renderCenter(SDL_Texture* p_tex, double x, double y, double w, double h, double angle, SDL_Point* center, SDL_RendererFlip flip) {
  SDL_Rect src;
  src.x = src.y = 0;
  SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);
  SDL_Rect dst;
  if (w != 0 && h != 0) {
    dst.w = w;
    dst.h = h;
  }
  else {
    dst.w = src.w;
    dst.h = src.h;
  }
  dst.x = x - dst.w / 2;
  dst.y = y - dst.h / 2;
  SDL_RenderCopyEx(gRenderer, p_tex, &src, &dst, angle, center, flip);
}

void Game::render(Entity &p_entity, double w, double h) {
  SDL_Rect dst;
  dst.x = p_entity.getX();
  dst.y = p_entity.getY();
  if (w != 0 && h != 0) {
    dst.w = w;
    dst.h = h;
  }
  else {
    dst.w = p_entity.getWidth();
    dst.h = p_entity.getHeight();
  }
  SDL_RenderCopyEx(gRenderer, p_entity.getTexture(), p_entity.getCurrentClip(), &dst, p_entity.getAngle(), p_entity.getCenter(), p_entity.getFlip());
}

void Game::renderText(double p_x, double p_y, const char* p_text, TTF_Font* font, SDL_Color textColor) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x;
  dst.y = p_y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::renderText(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor) {
  char* text = &p_text[0];
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x;
  dst.y = p_y;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::renderTextCenter(double p_x, double p_y, const char* p_text, TTF_Font* font, SDL_Color textColor) {
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x - src.w / 2;
  dst.y = p_y - src.h / 2;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::renderTextCenter(double p_x, double p_y, string& p_text, TTF_Font* font, SDL_Color textColor) {
  char* text = &p_text[0];
  SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text, textColor);
  SDL_Texture* message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = surfaceMessage->w;
  src.h = surfaceMessage->h;

  SDL_Rect dst;
  dst.x = p_x - src.w / 2;
  dst.y = p_y - src.h / 2;
  dst.w = src.w;
  dst.h = src.h;

  SDL_RenderCopy(gRenderer, message, &src, &dst);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(message);
}

void Game::render_Skill_Hud_And_Cooldown() {
  {
    render(player_health_Hud[1][HealthBar_Hud_ID]);
    if (player[1].getHealth() > 0)
      render(player_health_Hud[1][HealthColor_Hud_ID], 1.0 * player_health_Hud[1][HealthColor_Hud_ID].getWidth() / PlayerStartHealth * player[1].getHealth(), player_health_Hud[1][HealthColor_Hud_ID].getHeight());
    string health;
    if (player[1].getHealth() > 0)
      health = "Health: " + IntToString(player[1].getHealth());
    else
      health = "Health: !!";
    SDL_Texture* player_health_texture = NULL;
    player_health_texture = loadTextureFromText(health, Font[font50], white);
    renderCenter(player_health_texture, player_health_Hud[1][HealthBar_Hud_ID].getX() + player_health_Hud[1][HealthBar_Hud_ID].getWidth() / 2, player_health_Hud[1][HealthBar_Hud_ID].getY() + player_health_Hud[1][HealthBar_Hud_ID].getHeight() / 2);
    SDL_DestroyTexture(player_health_texture);

    render(player_health_Hud[2][HealthBar_Hud_ID]);
    if (player[2].getHealth() > 0)
      render(player_health_Hud[2][HealthColor_Hud_ID], 1.0 * player_health_Hud[2][HealthColor_Hud_ID].getWidth() / PlayerStartHealth * player[2].getHealth(), player_health_Hud[2][HealthColor_Hud_ID].getHeight());
    if (player[2].getHealth() > 0)
      health = "Health: " + IntToString(player[2].getHealth());
    else
      health = "Health: !!";
    player_health_texture = loadTextureFromText(health, Font[font50], white);
    renderCenter(player_health_texture, player_health_Hud[2][HealthBar_Hud_ID].getX() + player_health_Hud[2][HealthBar_Hud_ID].getWidth() / 2, player_health_Hud[2][HealthBar_Hud_ID].getY() + player_health_Hud[2][HealthBar_Hud_ID].getHeight() / 2);
    SDL_DestroyTexture(player_health_texture);
  }
  for (int id = 1; id <= 2; id++) {
    for (int skill_id = 0; skill_id <= skillGhost_ID; skill_id++) {
      if (skill_id > skillR_ID && gameMode != Special_Mode)
        continue;
      
      render(player_skill_Hud[id][skill_id]);
      if (player[id].getSkillCooldown(skill_id) > 0) {
        std::string text = TimeToString(player[id].getSkillCooldown(skill_id), Skill_TimeConvertType);
        renderTextCenter(player_skill_Hud[id][skill_id].getX() + player_skill_Hud[id][skill_id].getWidth() / 2,
                        player_skill_Hud[id][skill_id].getY() + player_skill_Hud[id][skill_id].getHeight() / 2,
                        text,
                        Font[font32],
                        white);
        renderTextCenter(player_skill_Hud[id][skill_id].getX() + player_skill_Hud[id][skill_id].getWidth() / 2,
                        player_skill_Hud[id][skill_id].getY() + player_skill_Hud[id][skill_id].getHeight() / 2,
                        text,
                        Font[font32_outline],
                        black);
      }
    }
  }
  if (gameMode == Special_Mode) {
    for (int id = 1; id <= 2; id++) {
      if (player[id].getItemID() != ItemStartID) {
        render(player_skill_Hud[id][player[id].getItemID()]);
      }
    }
  }
}

void Game::PlaySkillSFX(int skill_ID) {
  switch (skill_ID) {
    case (skillQ_ID): {
      Mix_PlayChannel(-1, SFX[Q1_sfx_ID], 0);
      Mix_PlayChannel(-1, SFX[Q2_sfx_ID], 0);
      break;
    }
    case (skillW_ID): {
      Mix_PlayChannel(-1, SFX[W_sfx_ID], 0);
      break;
    }
    case (skillE_ID): {
      Mix_PlayChannel(-1, SFX[E_sfx_ID], 0);
      break;
    }
    case (skillR_ID): {
      Mix_PlayChannel(-1, SFX[R_sfx_ID], 0);
      break;
    }
    case (skillGhost_ID): {
      Mix_PlayChannel(-1, SFX[Ghost_start_sfx_ID], 0);
      break;
    }
    case (skillHourglass_ID): {
      Mix_PlayChannel(-1, SFX[Hourglass_start_sfx_ID], 0);
      break;
    }
    case (skillFlash_ID): {
      Mix_PlayChannel(-1, SFX[Flash_sfx_ID], 0);
      break;
    }
    case (skillHeal_ID): {
      Mix_PlayChannel(-1, SFX[Heal_sfx_ID], 0);
      break;
    }
  }
}

void Game::PlaySFX(int sfx_ID) {
  switch (sfx_ID) {
    case (Click_sfx_ID): {
      Mix_PlayChannel(-1, SFX[Click_sfx_ID], 0);
      break;
    }
    case (Count_down_sfx_ID): {
      Mix_PlayChannel(Count_down_sfx_Channel, SFX[Count_down_sfx_ID], 0);
      break;
    }
    case (Count_down_start_sfx_ID): {
      Mix_PlayChannel(Count_down_start_sfx_Channel, SFX[Count_down_start_sfx_ID], 0);
      break;
    }
    case (Q_hit_sfx_ID): {
      Mix_PlayChannel(-1, SFX[Q_hit_sfx_ID], 0);
      break;
    }
    case (W_hit_sfx_ID): {
      Mix_PlayChannel(-1, SFX[W_hit_sfx_ID], 0);
      break;
    }
    case (W_hit_crashed_sfx_ID): {
      Mix_PlayChannel(-1, SFX[W_hit_crashed_sfx_ID], 0);
      break;
    }
    case (R_hit_sfx_ID): {
      Mix_PlayChannel(-1, SFX[R_hit_sfx_ID], 0);
      break;
    }
    case (Ghost_end_sfx_ID): {
      Mix_PlayChannel(-1, SFX[Ghost_end_sfx_ID], 0);
      break;
    }
    case (Hourglass_end_sfx_ID): {
      Mix_PlayChannel(-1, SFX[Hourglass_end_sfx_ID], 0);
      break;
    }
    case (Voltaic_Cyclosword_sfx_ID): {
      Mix_PlayChannel(-1, SFX[Voltaic_Cyclosword_sfx_ID], 0);
      break;
    }
    case (UsingItem_start_sfx_ID): {
      Mix_PlayChannel(-1, SFX[UsingItem_start_sfx_ID], 0);
      break;
    }
  }
}

void Game::PlayMusic(int music_ID) {
  switch (music_ID) {
    case (GameEnd_music_ID): {
      Mix_PlayMusic(MUSIC[music_ID], 0);
      break;
    }
  }
}

void Game::PlayPlayerSFX() {
  for (int player_id = 1; player_id <= 2; player_id++) {
    for (int skill_id = skillR_ID + 1; skill_id < skill_ID_Total; skill_id++) {
      if (player[player_id].getEffectTime(skill_id) != 1)
        continue;
      switch (skill_id) {
        case (skillGhost_ID): {
          PlaySFX(Ghost_end_sfx_ID);
          break;
        }
        case (skillHourglass_ID): {
          PlaySFX(Hourglass_end_sfx_ID);
          break;
        }
      }
    }
  }
}

bool Game::isPlayerCollidingBullet(Player p_player, Bullet p_bullet) {
  if (p_player.isInvulnerable())
    return false;
  p_player.setWidth(p_player.getWidth() - 30);
  if (p_player.getFlip() == SDL_FLIP_HORIZONTAL)
    p_player.setX(p_player.getX() + 30);

  Rectangle playerRec(p_player);
  Rectangle bulletRec(p_bullet);

  return isColliding(playerRec, bulletRec);
}

void Game::setArrowToPlayer(bool isMaximizing, Player tmp_player, PlayerArrow &tmp_arrow) {
  if (isMaximizing == Human) {
    tmp_arrow.setX(tmp_player.getX() + tmp_player.getWidth() / 2 - tmp_arrow.getWidth() / 2);
    tmp_arrow.setY(tmp_player.getY() - tmp_arrow.getHeight() - ArrowDistanceToPlayer);
    tmp_arrow.setRotPoint(tmp_arrow.getX() + tmp_arrow.getWidth() / 2, tmp_arrow.getY() + tmp_arrow.getHeight());
  }
  else {
    tmp_arrow.setX(tmp_player.getX() + tmp_player.getWidth() / 2 - tmp_arrow.getWidth() / 2);
    tmp_arrow.setY(tmp_player.getY() + tmp_player.getHeight() + ArrowDistanceToPlayer);
    tmp_arrow.setRotPoint(tmp_arrow.getX() + tmp_arrow.getWidth() / 2, tmp_arrow.getY());
  }
}

double Game::CalculateArrowAndBulletAccuracy(bool isMaximizing, vector<Player> tmp_player, Entity tmp_arrow) {
  int player_id = isMaximizing;
  int enemy_id = isMaximizing ^ 1;

  Point arrow_Root;
  if (player_id == Human) {
    arrow_Root.x = tmp_arrow.getX() + tmp_arrow.getWidth() / 2;
    arrow_Root.y = tmp_arrow.getY() + tmp_arrow.getHeight();
  }
  else {
    arrow_Root.x = tmp_arrow.getX() + tmp_arrow.getWidth() / 2;
    arrow_Root.y =  tmp_arrow.getY();
  }

  Vector vec;
  if (player_id == Human)
    vec = Vector(sin(PI / 2 + tmp_arrow.getAngle() * PI / 180), -cos(PI / 2 + tmp_arrow.getAngle() * PI / 180));
  else
    vec = Vector(sin(-PI / 2 + tmp_arrow.getAngle() * PI / 180), -cos(-PI / 2 + tmp_arrow.getAngle() * PI / 180));

  int Enemy_Y_Current;
  if (player_id == Human)
    Enemy_Y_Current = tmp_player[enemy_id].getY() + tmp_player[enemy_id].getWidth();
  else
    Enemy_Y_Current = tmp_player[enemy_id].getY();

  Point arrow_Root_At_Enemy_Y_current((vec.x * arrow_Root.x - vec.y * Enemy_Y_Current + vec.y * arrow_Root.y) / vec.x, Enemy_Y_Current);

  return abs(arrow_Root_At_Enemy_Y_current.x - (tmp_player[enemy_id].getX() + tmp_player[enemy_id].getWidth() / 2));
}

Bullet Game::makeBullet(int skill_ID, Player &tmp_player, PlayerArrow &tmp_arrow) {
  Bullet bul = bullet[skill_ID];
  bul.setX(tmp_player.getX() + tmp_player.getWidth() / 2 - bul.getWidth() / 2);
  bul.setY(tmp_player.getY() + tmp_player.getHeight());
  bul.setAngle(tmp_arrow.getAngle());
  bul.setCenter(bul.getWidth() / 2, 0);
  bul.setRotPoint(bul.getX() + bul.getWidth() / 2, bul.getY());
  return bul;
}

double Game::HeuristicEvaluation(vector<Player> tmp_player, vector<PlayerArrow> tmp_arrow, vector<vector<Bullet>> tmp_bullets) {
  double Score = 0;

  bool Human_HasDamagedBullet = 0;
  for (Bullet bul : tmp_bullets[Human])
    if (bul.getSkillId() != skillW_ID) {
      Human_HasDamagedBullet = 1;
      break;
    }
  if (Human_HasDamagedBullet) {
    for (Bullet bul : tmp_bullets[Human]) {
      if (bul.getSkillId() == skillR_ID) {
        if (gameAIMode == AIGameMode_Normal) {
          Score -= CalculateArrowAndBulletAccuracy(Human, tmp_player, bul);
          continue;
        }
        bool collision = 0;
        while (bul.isOnScreen()) {
          if (isPlayerCollidingBullet(tmp_player[Bot], bul)) {
            collision = 1;
            break;
          }
          bul.move();
        }
        if (collision) {
          Score += ShootedBySkillR_Value;
          if (bul.getAngle() <= 0)
            Score -= (tmp_player[Bot].getX() - PlayerScreenLeftBoundary);
          else
            Score -= (PlayerScreenRightBoundary - tmp_player[Bot].getX());
        }
        else
          Score -= CalculateArrowAndBulletAccuracy(Human, tmp_player, bul);
        continue;
      }
      else if (bul.getSkillId() == skillQ_ID) {
        if (gameAIMode == AIGameMode_Normal || gameAIMode == AIGameMode_Hard) {
          Score -= CalculateArrowAndBulletAccuracy(Human, tmp_player, bul);
          continue;
        }
        bool collision = 0;
        while (bul.isOnScreen()) {
          if (isPlayerCollidingBullet(tmp_player[Bot], bul)) {
            collision = 1;
            break;
          }
          bul.move();
        }
        if (collision)
          Score += ShootedBySkillQ_Value;
        Score -= CalculateArrowAndBulletAccuracy(Human, tmp_player, bul);
      }
      else
        Score -= CalculateArrowAndBulletAccuracy(Human, tmp_player, bul);
    }
  }

  if (Human_HasDamagedBullet == 0) {
    for (Bullet bul : tmp_bullets[Bot]) {
      if (bul.getSkillId() == skillQ_ID)
        Score -= ShootSkillQ_Value;
      if (bul.getSkillId() == skillR_ID) {
        if (tmp_player[Human].getHealth() >= skill_damage[skillR_ID])
          Score -= ShootSkillR_Value;
        else if (tmp_player[Human].getSkillCooldown(skillQ_ID) > skill_castTime[skillR_ID] 
                && tmp_player[Human].getSkillCooldown(skillQ_ID) < skill_cooldown[skillQ_ID])
        {
          Score -= ShootSkillR_Value;
        }
      }
      if (tmp_player[Human].getHealth() > skill_damage[skillQ_ID] && bul.getSkillId() == skillW_ID
        && tmp_player[Bot].getSkillCooldown(skillQ_ID) <= FPS / 2)
      {
        Score -= ShootSkillW_Value;
      }
    }
  }

  for (int id = 0; id < Player_Type_Total; id++) {
    if (id == Bot) {
      if (Human_HasDamagedBullet == 0)
        if (tmp_player[Bot].getSkillCooldown(skillQ_ID) <= FPS / 2 
          || tmp_player[Bot].getSkillCooldown(skillQ_ID) == skill_cooldown[skillQ_ID]) 
        {
          //also checking state when AI shoot bullet
          Score += CalculateArrowAndBulletAccuracy(id, tmp_player, tmp_arrow[id]);
        }
    }
    if (id == Human) {
      if (tmp_bullets[Human].empty()) {
        if (tmp_player[Bot].getSkillCooldown(skillQ_ID) > FPS / 2 
          && tmp_player[Bot].getSkillCooldown(skillQ_ID) < skill_cooldown[skillQ_ID])
        {
          //not checking state when AI shoot bullet
          if (tmp_player[Human].getSkillCooldown(skillQ_ID) <= FPS / 2) {
            int distance = HeuristicValueByMode[Distance_To_Screen_Bound][gameAIMode];
            if (tmp_player[Bot].getX() - PlayerScreenLeftBoundary <= distance) {
              Score += DOUBLE_INF - (tmp_player[Bot].getX() - PlayerScreenLeftBoundary);
            }
            else if (PlayerScreenRightBoundary - tmp_player[Bot].getX() <= distance) {
              Score += DOUBLE_INF - (PlayerScreenRightBoundary - tmp_player[Bot].getX());
            }
            else
              Score -= CalculateArrowAndBulletAccuracy(id, tmp_player, tmp_arrow[id]);
          }
          else
              Score -= CalculateArrowAndBulletAccuracy(id, tmp_player, tmp_arrow[id]);
        }
      }
    }
  }

  return Score;
}

double Game::Minimax(int depth, bool isMaximizing, double Alpha, double Beta, vector<Player> &tmp_player, vector<PlayerArrow> &tmp_arrow, vector<vector<Bullet>> &tmp_bullets) {
  if (depth == 0) return HeuristicEvaluation(tmp_player, tmp_arrow, tmp_bullets);

  int player_id = isMaximizing;
  int enemy_id = isMaximizing ^ 1;

  double bestScore = HeuristicEvaluation(tmp_player, tmp_arrow, tmp_bullets);
  if (isMaximizing)
    Alpha = Max(Alpha, bestScore);
  else
    Beta = Min(Beta, bestScore);

  if (Beta <= Alpha)
    return bestScore;

  if (tmp_player[player_id].getCastTimeCooldown() > 0) {
    if (isMaximizing) {
      bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
    }
    else {
      bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
    }
    return bestScore;
  }

  for (int move_type = 0; move_type < AI_Move_Type_Total; move_type++) {
    switch (move_type) {
      case (Do_Nothing): {
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Move_Left): {
        int lastX = tmp_player[player_id].getX(), lastY = tmp_player[player_id].getY();
        tmp_player[player_id].moveLeft();
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        SDL_RendererFlip lastFlip = tmp_player[player_id].getFlip();
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setX(lastX); tmp_player[player_id].setY(lastY);
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        tmp_player[player_id].setFlip(lastFlip);
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Move_Right): {
        int lastX = tmp_player[player_id].getX(), lastY = tmp_player[player_id].getY();
        tmp_player[player_id].moveRight();
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        SDL_RendererFlip lastFlip = tmp_player[player_id].getFlip();
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setX(lastX); tmp_player[player_id].setY(lastY);
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        tmp_player[player_id].setFlip(lastFlip);
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_Q): { ///Only AI
        if (player_id == Human)
          break;
        if (tmp_player[player_id].getSkillCooldown(skillQ_ID) > 0)
          break;
        if (tmp_player[enemy_id].isInvulnerable())
          break;
        if (CalculateArrowAndBulletAccuracy(player_id, tmp_player, tmp_arrow[player_id]) > Distance_To_Shoot[skillQ_ID][gameAIMode])
          break;

        tmp_bullets[player_id].push_back(makeBullet(skillQ_ID, tmp_player[player_id], tmp_arrow[player_id]));
        tmp_player[player_id].setCastTimeCooldown(skill_castTime[skillQ_ID]);
        tmp_player[player_id].setSkillCooldown(skill_cooldown[skillQ_ID], skillQ_ID);
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setCastTimeCooldown(0);
        tmp_player[player_id].setSkillCooldown(0, skillQ_ID);
        tmp_bullets[player_id].pop_back();
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_W): { ///Only AI
        if (player_id == Human)
          break;
        if (tmp_player[player_id].getSkillCooldown(skillW_ID) > 0)
          break;
        if (tmp_player[enemy_id].isInvulnerable())
          break;
        if (CalculateArrowAndBulletAccuracy(player_id, tmp_player, tmp_arrow[player_id]) > Distance_To_Shoot[skillW_ID][gameAIMode])
          break;

        tmp_bullets[player_id].push_back(makeBullet(skillW_ID, tmp_player[player_id], tmp_arrow[player_id]));
        tmp_player[player_id].setCastTimeCooldown(skill_castTime[skillW_ID]);
        tmp_player[player_id].setSkillCooldown(skill_cooldown[skillW_ID], skillW_ID);
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setCastTimeCooldown(0);
        tmp_player[player_id].setSkillCooldown(0, skillW_ID);
        tmp_bullets[player_id].pop_back();

        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_E_Left): { ///Only AI
        if (player_id == Human)
          break;
        if (tmp_player[player_id].getSkillCooldown(skillE_ID) > 0)
          break;
        if (tmp_bullets[enemy_id].empty())
          break;
        bool ok = 0;
        for (Bullet bul : tmp_bullets[enemy_id]) {
          if (bul.getSkillId() == skillW_ID)
            continue;
          bul.move();
          if (isPlayerCollidingBullet(tmp_player[player_id], bul)) {
            ok = 1;
            break;
          }
        }
        if (ok == 0)
          break;

        tmp_player[player_id].setCastTimeCooldown(skill_castTime[skillE_ID]);
        tmp_player[player_id].setSkillCooldown(skill_cooldown[skillE_ID], skillE_ID);
        int lastX = tmp_player[player_id].getX(), lastY = tmp_player[player_id].getY();
        tmp_player[player_id].moveLeft();
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        SDL_RendererFlip lastFlip = tmp_player[player_id].getFlip();
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setCastTimeCooldown(0);
        tmp_player[player_id].setSkillCooldown(0, skillE_ID);
        tmp_player[player_id].setX(lastX); tmp_player[player_id].setY(lastY);
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        tmp_player[player_id].setFlip(lastFlip);
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_E_Right): { ///Only AI
        if (player_id == Human)
          break;
        if (tmp_player[player_id].getSkillCooldown(skillE_ID) > 0)
          break;
        if (tmp_bullets[enemy_id].empty())
          break;
        bool ok = 0;
        for (Bullet bul : tmp_bullets[enemy_id]) {
          if (bul.getSkillId() == skillW_ID)
            continue;
          bul.move();
          if (isPlayerCollidingBullet(tmp_player[player_id], bul)) {
            ok = 1;
            break;
          }
        }
        if (ok == 0)
          break;

        tmp_player[player_id].setCastTimeCooldown(skill_castTime[skillE_ID]);
        tmp_player[player_id].setSkillCooldown(skill_cooldown[skillE_ID], skillE_ID);
        int lastX = tmp_player[player_id].getX(), lastY = tmp_player[player_id].getY();
        tmp_player[player_id].moveRight();
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        SDL_RendererFlip lastFlip = tmp_player[player_id].getFlip();
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setCastTimeCooldown(0);
        tmp_player[player_id].setSkillCooldown(0, skillE_ID);
        tmp_player[player_id].setX(lastX); tmp_player[player_id].setY(lastY);
        setArrowToPlayer(player_id, tmp_player[player_id], tmp_arrow[player_id]);
        tmp_player[player_id].setFlip(lastFlip);
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
      case (Use_R): { ///Only AI
        if (player_id == Human)
          break;
        if (gameAIMode == AIGameMode_Impossible) {
          if (abs(tmp_arrow[player_id].getAngle()) < 20)
            break;
        }
        if (tmp_player[player_id].getSkillCooldown(skillR_ID) > 0)
          break;
        if (tmp_player[enemy_id].isInvulnerable())
            break;
        if (CalculateArrowAndBulletAccuracy(player_id, tmp_player, tmp_arrow[player_id]) > Distance_To_Shoot[skillR_ID][gameAIMode])
          break;

        tmp_bullets[player_id].push_back(makeBullet(skillR_ID, tmp_player[player_id], tmp_arrow[player_id]));
        tmp_player[player_id].setCastTimeCooldown(skill_castTime[skillR_ID]);
        tmp_player[player_id].setSkillCooldown(skill_cooldown[skillR_ID], skillR_ID);
        if (isMaximizing) {
          bestScore = Max(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Alpha = Max(Alpha, bestScore);
        }
        else {
          bestScore = Min(bestScore, Minimax(depth - 1, isMaximizing ^ 1, Alpha, Beta, tmp_player, tmp_arrow, tmp_bullets));
          Beta = Min(Beta, bestScore);
        }
        tmp_player[player_id].setCastTimeCooldown(0);
        tmp_player[player_id].setSkillCooldown(0, skillR_ID);
        tmp_bullets[player_id].pop_back();
        if (Beta <= Alpha)
          return bestScore;
        break;
      }
    }
  }

  return bestScore;
}

void Game::ProcessingAIMove(int player_id) {
  if (gameTimer == 0 || player[player_id].isDead() || player[3  - player_id].isDead()) {
    player[player_id].setCurrentFrame(0);
    return;
  }

  vector<Player> tmp_player(2);
  tmp_player[Bot] = player[player_id];
  tmp_player[Human] = player[3 - player_id];

  vector<PlayerArrow> tmp_arrow(2);
  tmp_arrow[Bot] = player_Arrow[player_id];
  tmp_arrow[Human] = player_Arrow[3 - player_id];

  vector<vector<Bullet>> tmp_bullets(3);
  tmp_bullets[Bot] = player_bullets[player_id];
  tmp_bullets[Human] = player_bullets[3 - player_id];

  if (gameMode == Special_Mode) {
    if (UsingItemTime > 0) {
      if (tmp_player[Bot].getItemID() == skillFlash_ID) {
        if (tmp_player[Bot].getSkillCooldown(skillQ_ID) == 0 && tmp_player[Bot].getSkillCooldown(skillW_ID) > 0) {
          if (tmp_player[Bot].getFlip() == SDL_FLIP_NONE)
            tmp_player[Bot].setX(tmp_player[Bot].getX() + PlayerFlashtVelocity);
          else
            tmp_player[Bot].setX(tmp_player[Bot].getX() - PlayerFlashtVelocity);
          if (tmp_player[Bot].getX() < PlayerScreenLeftBoundary)
            tmp_player[Bot].setX(PlayerScreenLeftBoundary);
          if (tmp_player[Bot].getX() > PlayerScreenRightBoundary)
            tmp_player[Bot].setX(PlayerScreenRightBoundary);
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          if (CalculateArrowAndBulletAccuracy(Bot, tmp_player, tmp_arrow[Bot]) < Distance_To_Shoot[skillQ_ID][gameAIMode])
            ProcessingSkill(player_id, skillFlash_ID);
          tmp_player[Bot] = player[player_id];
          tmp_arrow[Bot] = player_Arrow[player_id];
        }
      }
    }
  }

  double bestScore = DOUBLE_INF;
  int bestMove = -1;
  int depth = 7;

  if (player[player_id].getCastTimeCooldown() == 0) {
    for (int move_type = 0; move_type < AI_Move_Type_Total; move_type++) {
      switch (move_type) {
        case (Do_Nothing): {
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          break;
        }
        case (Move_Left): {
          int lastX = tmp_player[Bot].getX(), lastY = tmp_player[Bot].getY();
          tmp_player[Bot].moveLeft();
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          SDL_RendererFlip lastFlip = tmp_player[Bot].getFlip();
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setX(lastX); tmp_player[Bot].setY(lastY);
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          tmp_player[Bot].setFlip(lastFlip);
          break;
        }
        case (Move_Right): {
          int lastX = tmp_player[Bot].getX(), lastY = tmp_player[Bot].getY();
          tmp_player[Bot].moveRight();
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          SDL_RendererFlip lastFlip = tmp_player[Bot].getFlip();
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setX(lastX); tmp_player[Bot].setY(lastY);
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          tmp_player[Bot].setFlip(lastFlip);
          break;
        }
        case (Use_Q): {
          if (tmp_player[Bot].getSkillCooldown(skillQ_ID) > 0)
            break;
          if (tmp_player[Human].isInvulnerable())
            break;
          if (CalculateArrowAndBulletAccuracy(Bot, tmp_player, tmp_arrow[Bot]) > Distance_To_Shoot[skillQ_ID][gameAIMode])
            break;

          tmp_bullets[Bot].push_back(makeBullet(skillQ_ID, tmp_player[Bot], tmp_arrow[Bot]));
          tmp_player[Bot].setCastTimeCooldown(skill_castTime[skillQ_ID]);
          tmp_player[Bot].setSkillCooldown(skill_cooldown[skillQ_ID], skillQ_ID);
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setCastTimeCooldown(0);
          tmp_player[Bot].setSkillCooldown(0, skillQ_ID);
          tmp_bullets[Bot].pop_back();
          break;
        }
        case (Use_W): {
          if (tmp_player[Bot].getSkillCooldown(skillW_ID) > 0)
            break;
          if (tmp_player[Human].isInvulnerable())
            break;
          if (CalculateArrowAndBulletAccuracy(Bot, tmp_player, tmp_arrow[Bot]) > Distance_To_Shoot[skillW_ID][gameAIMode])
            break;

          tmp_bullets[Bot].push_back(makeBullet(skillW_ID, tmp_player[Bot], tmp_arrow[Bot]));
          tmp_player[Bot].setCastTimeCooldown(skill_castTime[skillW_ID]);
          tmp_player[Bot].setSkillCooldown(skill_cooldown[skillW_ID], skillW_ID);
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setCastTimeCooldown(0);
          tmp_player[Bot].setSkillCooldown(0, skillW_ID);
          tmp_bullets[Bot].pop_back();
          break;
        }
        case (Use_E_Left): {
          if (tmp_player[Bot].getSkillCooldown(skillE_ID) > 0)
            break;
          if (tmp_bullets[Human].empty())
            break;
          bool ok = 0;
          for (Bullet bul : tmp_bullets[Human]) {
            if (bul.getSkillId() == skillW_ID)
              continue;
            bul.move();
            Rectangle BotRec(tmp_player[Bot]);
            Rectangle BulRec(bul);
            if (isPlayerCollidingBullet(tmp_player[Bot], bul)) {
              ok = 1;
              break;
            }
          }
          if (ok == 0)
            break;

          tmp_player[Bot].setCastTimeCooldown(skill_castTime[skillE_ID]);
          tmp_player[Bot].setSkillCooldown(skill_cooldown[skillE_ID], skillE_ID);
          int lastX = tmp_player[Bot].getX(), lastY = tmp_player[Bot].getY();
          tmp_player[Bot].moveLeft();
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          SDL_RendererFlip lastFlip = tmp_player[Bot].getFlip();
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setCastTimeCooldown(0);
          tmp_player[Bot].setSkillCooldown(0, skillE_ID);
          tmp_player[Bot].setX(lastX); tmp_player[Bot].setY(lastY);
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          tmp_player[Bot].setFlip(lastFlip);
          break;
        }
        case (Use_E_Right): {
          if (tmp_player[Bot].getSkillCooldown(skillE_ID) > 0)
            break;
          if (tmp_bullets[Human].empty())
            break;
          bool ok = 0;
          for (Bullet bul : tmp_bullets[Human]) {
            if (bul.getSkillId() == skillW_ID)
              continue;
            bul.move();
            if (isPlayerCollidingBullet(tmp_player[Bot], bul)) {
              ok = 1;
              break;
            }
          }
          if (ok == 0)
            break;

          tmp_player[Bot].setCastTimeCooldown(skill_castTime[skillE_ID]);
          tmp_player[Bot].setSkillCooldown(skill_cooldown[skillE_ID], skillE_ID);
          int lastX = tmp_player[Bot].getX(), lastY = tmp_player[Bot].getY();
          tmp_player[Bot].moveRight();
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          SDL_RendererFlip lastFlip = tmp_player[Bot].getFlip();
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setCastTimeCooldown(0);
          tmp_player[Bot].setSkillCooldown(0, skillE_ID);
          tmp_player[Bot].setX(lastX); tmp_player[Bot].setY(lastY);
          setArrowToPlayer(Bot, tmp_player[Bot], tmp_arrow[Bot]);
          tmp_player[Bot].setFlip(lastFlip);
          break;
        }
        case (Use_R): {
          if (gameAIMode == AIGameMode_Impossible) {
            if (abs(tmp_arrow[Bot].getAngle()) < 20)
              break;
          }
          if (tmp_player[Bot].getSkillCooldown(skillR_ID) > 0)
            break;
          if (tmp_player[Human].isInvulnerable())
            break;
          if (CalculateArrowAndBulletAccuracy(Bot, tmp_player, tmp_arrow[Bot]) > Distance_To_Shoot[skillR_ID][gameAIMode])
            break;

          tmp_bullets[Bot].push_back(makeBullet(skillR_ID, tmp_player[Bot], tmp_arrow[Bot]));
          tmp_player[Bot].setCastTimeCooldown(skill_castTime[skillR_ID]);
          tmp_player[Bot].setSkillCooldown(skill_cooldown[skillR_ID], skillR_ID);
          if (minimize(bestScore, Minimax(depth, 1, -DOUBLE_INF, DOUBLE_INF, tmp_player, tmp_arrow, tmp_bullets)))
            bestMove = move_type;
          tmp_player[Bot].setCastTimeCooldown(0);
          tmp_player[Bot].setSkillCooldown(0, skillR_ID);
          tmp_bullets[Bot].pop_back();
          break;
        }
      }
    }
  }

  if (gameMode == Special_Mode) {
    if (tmp_player[Bot].getSkillCooldown(skillGhost_ID) == 0) {
      ProcessingSkill(player_id, skillGhost_ID);
    }
    if (UsingItemTime > 0) {
      if (tmp_player[Bot].getItemID() == skillHeal_ID 
        && (tmp_player[Bot].getHealth() < PlayerStartHealth || UsingItemTime == 1)) {
        ProcessingSkill(player_id, tmp_player[Bot].getItemID());
      }
      if (tmp_player[Bot].getItemID() == skillHourglass_ID && tmp_player[Bot].getSkillCooldown(skillE_ID) > 0) {
        bool getDamaged = 0;
        for (Bullet bul : tmp_bullets[Human]) {
          if (bul.getSkillId() == skillW_ID)
            continue;
          bul.move();
          if (isPlayerCollidingBullet(tmp_player[Bot], bul)) {
            getDamaged = 1;
            break;
          }
        }
        if (getDamaged) {
          ProcessingSkill(player_id, skillHourglass_ID);
          return;
        }
      }
      if (tmp_player[Bot].getItemID() == skillMystery_Box_ID) {
        generateItem(player_id);
      }
    }
  }

  if (player[player_id].getCastTimeCooldown() == 0) {
    switch (bestMove) {
      case (Do_Nothing): {
        player[player_id].setCurrentFrame(0);
        break;
      }
      case (Move_Left): {
        player[player_id].moveLeft();
        break;
      }
      case (Move_Right): {
        player[player_id].moveRight();
        break;
      }
      case (Use_Q): {
        ProcessingSkill(player_id, skillQ_ID);
        break;
      }
      case (Use_W): {
        ProcessingSkill(player_id, skillW_ID);
        break;
      }
      case (Use_E_Left): {
        ProcessingSkill(player_id, skillE_ID);
        player[player_id].moveLeft();
        break;
      }
      case (Use_E_Right): {
        ProcessingSkill(player_id, skillE_ID);
        player[player_id].moveRight();
        break;
      }
      case (Use_R): {
        ProcessingSkill(player_id, skillR_ID);
        break;
      }
    }
  }
}

void Game::ProcessingSkill(int player_id, int skill_ID) {
  if (player[player_id].getCastTimeCooldown() > 0) {
    switch (skill_ID) {
      case (skillHourglass_ID): case (skillHeal_ID):
      {
        ///Processing
        break;
      }
      default: {
        ///Do nothing
        return;
      }
    }
  }
  else {
    switch (skill_ID) {
      case (skillVoltaic_Cyclosword_ID): case (skillMystery_Box_ID):
      {
        return;
      }
    }
  }
  
  if (skill_ID == ItemStartID)
    return;

  if (player[player_id].getSkillCooldown(skill_ID) != 0)
    return;

  player[player_id].setCastTimeCooldown(skill_castTime[skill_ID]);
  player[player_id].setSkillCooldown(skill_cooldown[skill_ID], skill_ID);
  if (skill_ID > skillR_ID) {
    player[player_id].setEffectTime(skill_effectTime[skill_ID], skill_ID);
    if (skill_ID > skillGhost_ID)
      player[player_id].resetItemID();
  }
  PlaySkillSFX(skill_ID);

  switch (skill_ID) {
    case (skillE_ID): {

      break;
    }
    case (skillQ_ID): case (skillW_ID): case (skillR_ID): {
      Bullet tmp = bullet[skill_ID];
      if (player_id == 1) {
        tmp.setX(player[1].getX() + player[1].getWidth() / 2 - tmp.getWidth() / 2);
        tmp.setY(player[1].getY() + player[1].getHeight());
        tmp.setAngle(player_Arrow[1].getAngle());
        tmp.setCenter(tmp.getWidth() / 2, 0);
        tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY()); ///reset after each frame in void move();
      }
      else if (player_id == 2) {
        tmp.setX(player[player_id].getX() + player[player_id].getWidth() / 2 - tmp.getWidth() / 2);
        tmp.setY(player[player_id].getY() - tmp.getHeight());
        tmp.setAngle(player_Arrow[player_id].getAngle());
        tmp.setCenter(tmp.getWidth() / 2, tmp.getHeight());
        tmp.setRotPoint(tmp.getX() + tmp.getWidth() / 2, tmp.getY() + tmp.getHeight()); ///reset after each frame in void move();
        tmp.setFlip(SDL_FLIP_VERTICAL);
        tmp.setVelocityX(-tmp.getVelocityX());
        tmp.setVelocityY(-tmp.getVelocityY());
      }
      player_bullets[player_id].push_back(tmp);
      if (skill_ID == skillR_ID)
        player[player_id].setSkillDelay(FPS);
      break;
    }
    case (skillGhost_ID): {
      player[player_id].resetSummonerSpellID();
      break;
    }
    case (skillFlash_ID): {
      if (player[player_id].getFlip() == SDL_FLIP_NONE) {
        player[player_id].setX(player[player_id].getX() + PlayerFlashtVelocity);
        if (player[player_id].getX() > PlayerScreenRightBoundary) player[player_id].setX(PlayerScreenRightBoundary);
      }
      else {
        player[player_id].setX(player[player_id].getX() - PlayerFlashtVelocity);
        if (player[player_id].getX() < PlayerScreenLeftBoundary) player[player_id].setX(PlayerScreenLeftBoundary);
      }
      break;
    }
    case (skillHeal_ID): {
      int health = player[player_id].getHealth() + HealHealth;
      if (health > PlayerStartHealth)
        health = PlayerStartHealth;
      player[player_id].setHealth(health);
      break;
    }
  }
}

void Game::handleEvents() {
  ///player1
  if (player[1].getType() == Human) {
    if (keystate[SDL_SCANCODE_LEFT]) new_key[KEY_PRESS_LEFT] = 1;
    if (keystate[SDL_SCANCODE_RIGHT]) new_key[KEY_PRESS_RIGHT] = 1;
    if (keystate[SDL_SCANCODE_U]) new_key[KEY_PRESS_U] = 1;
    if (keystate[SDL_SCANCODE_I]) new_key[KEY_PRESS_I] = 1;
    if (keystate[SDL_SCANCODE_O]) new_key[KEY_PRESS_O] = 1;
    if (keystate[SDL_SCANCODE_P]) new_key[KEY_PRESS_P] = 1;
    if (keystate[SDL_SCANCODE_7]) new_key[KEY_PRESS_7] = 1;
    if (keystate[SDL_SCANCODE_8]) new_key[KEY_PRESS_8] = 1;
    if (keystate[SDL_SCANCODE_9]) new_key[KEY_PRESS_9] = 1;
  }

  ///player2
  if (player[2].getType() == Human) {
    if (keystate[SDL_SCANCODE_A]) new_key[KEY_PRESS_A] = 1;
    if (keystate[SDL_SCANCODE_D]) new_key[KEY_PRESS_D] = 1;
    if (keystate[SDL_SCANCODE_E]) new_key[KEY_PRESS_E] = 1;
    if (keystate[SDL_SCANCODE_R]) new_key[KEY_PRESS_R] = 1;
    if (keystate[SDL_SCANCODE_T]) new_key[KEY_PRESS_T] = 1;
    if (keystate[SDL_SCANCODE_Y]) new_key[KEY_PRESS_Y] = 1;
    if (keystate[SDL_SCANCODE_3]) new_key[KEY_PRESS_3] = 1;
    if (keystate[SDL_SCANCODE_4]) new_key[KEY_PRESS_4] = 1;
    if (keystate[SDL_SCANCODE_5]) new_key[KEY_PRESS_5] = 1;
  }

  if (new_key[KEY_PRESS_LEFT] && new_key[KEY_PRESS_RIGHT]) {
    new_key[KEY_PRESS_LEFT] = new_key[KEY_PRESS_RIGHT] = 0;
  }
  if (new_key[KEY_PRESS_A] && new_key[KEY_PRESS_D]) {
    new_key[KEY_PRESS_A] = new_key[KEY_PRESS_D] = 0;
  }

  ///utility
  if (keystate[SDL_SCANCODE_F11]) new_key[KEY_PRESS_F11] = 1;
  if (keystate[SDL_SCANCODE_ESCAPE]) new_key[KEY_PRESS_ESC] = 1;
  if (keystate[SDL_SCANCODE_RETURN]) new_key[KEY_PRESS_ENTER] = 1;

  int width, height;
  SDL_GetWindowSize(gWindow, &width, &height);
  SDL_GetMouseState(&MouseX, &MouseY);
  MouseX = (int)((double)SCREEN_WIDTH / width * MouseX);
  MouseY = (int)((double)SCREEN_HEIGHT / height * MouseY);
  mouseClicked = false;

  while (SDL_PollEvent(&gEvent)) {
    if (gEvent.type == SDL_QUIT) {
      gameRunning = false;
      break;
    }
    if (gEvent.type == SDL_MOUSEBUTTONDOWN && gEvent.button.button == SDL_BUTTON_LEFT) {
      mouseClicked = true;
    }
  }
}

int Game::randInt(int l, int r) {
  return l + rnd() % (r - l + 1);
}

void Game::generateItem(int player_id) {
  int randomItemID;
  while (1) {
    randomItemID = randInt(skillGhost_ID + 1, skill_ID_Total - 1);
    if (randomItemID == player[player_id].getLastItemId())
      continue;
    if (randomItemID == skillMystery_Box_ID)
      continue;
    else if (randomItemID == skillHeal_ID) {
      if (player[1].getHealth() <= HealthThreshold && player[2].getHealth() <= HealthThreshold)
        break;
    }
    else
      break;
  }
  player[player_id].setItemId(randomItemID);
  player[player_id].setLastItemId(randomItemID);
}

void Game::update() {
  ///Function keys
  if (new_key[KEY_PRESS_F11] && old_key[KEY_PRESS_F11] == 0) {
    if (fullscreen == false) {
      SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
      fullscreen = true;
    }
    else {
      SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_SHOWN);
      fullscreen = false;
    }
  }

  if ((new_key[KEY_PRESS_ESC] && old_key[KEY_PRESS_ESC] == 0) || (new_key[KEY_PRESS_ENTER] && old_key[KEY_PRESS_ENTER] == 0)){
    if (gameState == GamePlay) {
      gameState = GamePause;
    }
  }

  ///Player
  if (gameState == GamePlay && gameDelayTime == 0) {
    ///player1
    if (player[1].isDead() == 0) {
      player[1].updatePlayerEffects();
      player[1].updateCooldown();
      if (gameMode == Special_Mode && UsingItemTime > 0) {
        if (player[1].getItemID() == skillHourglass_ID) {
          if (new_key[KEY_PRESS_9] && old_key[KEY_PRESS_9] == 0)
              ProcessingSkill(1, player[1].getItemID());
        }
      }
      if (player[1].getType() == Bot) ProcessingAIMove(1);
      else {
        if (player[1].getCastTimeCooldown() == 0) {
          if (gameMode == Special_Mode) {
            if (UsingItemTime > 0) {
              if (new_key[KEY_PRESS_9] && old_key[KEY_PRESS_9] == 0)
                ProcessingSkill(1, player[1].getItemID());
            }

            if (new_key[KEY_PRESS_8] && old_key[KEY_PRESS_8] == 0)
              ProcessingSkill(1, player[1].getSummonerSpellID());
          }

          if (new_key[KEY_PRESS_U] && old_key[KEY_PRESS_U] == 0)
            ProcessingSkill(1, skillQ_ID);

          if (new_key[KEY_PRESS_I] && old_key[KEY_PRESS_I] == 0)
            ProcessingSkill(1, skillW_ID);

          if ((new_key[KEY_PRESS_LEFT] || new_key[KEY_PRESS_RIGHT])
              && new_key[KEY_PRESS_O] && old_key[KEY_PRESS_O] == 0)
          {
            ProcessingSkill(1, skillE_ID);
          }

          if (new_key[KEY_PRESS_P] && old_key[KEY_PRESS_P] == 0)
            ProcessingSkill(1, skillR_ID);

          if (new_key[KEY_PRESS_7] && old_key[KEY_PRESS_7] == 0)
            player_Arrow[1].increaseAngleDelta();

          if (new_key[KEY_PRESS_LEFT])
            player[1].moveLeft();
          else if (old_key[KEY_PRESS_LEFT])
            player[1].setCurrentFrame(0);

          if (new_key[KEY_PRESS_RIGHT])
            player[1].moveRight();
          else if (old_key[KEY_PRESS_RIGHT])
            player[1].setCurrentFrame(0);
          
          if (player[1].getEffectTime(skillHourglass_ID) == 0) {
            if (new_key[KEY_PRESS_LEFT] == 0 && new_key[KEY_PRESS_RIGHT] == 0)
              player[1].setCurrentFrame(0);
          }
        }
      }
    }

    if (player[1].getCastTimeCooldown() > 0 && player[1].getEffectTime(skillHourglass_ID) == 0)
      player[1].setCurrentFrame(0);

    ///player2
    if (player[2].isDead() == 0) {
      player[2].updatePlayerEffects();
      player[2].updateCooldown();
      if (gameMode == Special_Mode && UsingItemTime > 0) {
        if (player[2].getItemID() == skillHourglass_ID) {
          if (new_key[KEY_PRESS_5] && old_key[KEY_PRESS_5] == 0)
              ProcessingSkill(2, player[2].getItemID());
        }
      }
      if (player[2].getCastTimeCooldown() == 0) {
        if (gameMode == Special_Mode) {
          if (UsingItemTime > 0) {
            if (new_key[KEY_PRESS_5] && old_key[KEY_PRESS_5] == 0)
              ProcessingSkill(2, player[2].getItemID());
          }

          if (new_key[KEY_PRESS_4] && old_key[KEY_PRESS_4] == 0)
            ProcessingSkill(2, player[2].getSummonerSpellID());
        }
        
        if (new_key[KEY_PRESS_E] && old_key[KEY_PRESS_E] == 0)
          ProcessingSkill(2, skillQ_ID);

        if (new_key[KEY_PRESS_R] && old_key[KEY_PRESS_R] == 0)
          ProcessingSkill(2, skillW_ID);

        if ((new_key[KEY_PRESS_A] || new_key[KEY_PRESS_D])
            && new_key[KEY_PRESS_T] && old_key[KEY_PRESS_T] == 0)
        {
          ProcessingSkill(2, skillE_ID);
        }

        if (new_key[KEY_PRESS_Y] && old_key[KEY_PRESS_Y] == 0)
          ProcessingSkill(2, skillR_ID);

        if (new_key[KEY_PRESS_3] && old_key[KEY_PRESS_3] == 0)
          player_Arrow[2].increaseAngleDelta();

        if (new_key[KEY_PRESS_A])
          player[2].moveLeft();
        else if (old_key[KEY_PRESS_A])
          player[2].setCurrentFrame(0);

        if (new_key[KEY_PRESS_D])
          player[2].moveRight();
        else if (old_key[KEY_PRESS_D])
          player[2].setCurrentFrame(0);
        
        if (player[2].getEffectTime(skillHourglass_ID) == 0) {
            if (new_key[KEY_PRESS_A] == 0 && new_key[KEY_PRESS_D] == 0)
              player[2].setCurrentFrame(0);
          }
      }
    }

    if (player[2].getCastTimeCooldown() > 0 && player[2].getEffectTime(skillHourglass_ID) == 0)
      player[2].setCurrentFrame(0);
  }

  ///Game event
  if (gameState == GamePlay && gameMode == Special_Mode && gameDelayTime == 0) {
    if (UsingItemTime > 0) {
      UsingItemTime--;
      if (player[1].getItemID() == skillMystery_Box_ID) {
        if (new_key[KEY_PRESS_9] && old_key[KEY_PRESS_9] == 0)
          generateItem(1);
      }
      if (player[2].getItemID() == skillMystery_Box_ID) {
        if (new_key[KEY_PRESS_5] && old_key[KEY_PRESS_5] == 0)
          generateItem(2);
      }
      if (player[1].getItemID() == ItemStartID && player[2].getItemID() == ItemStartID)
        UsingItemTime = 0;
    }
    else {
      player[1].resetItemID();
      player[2].resetItemID();
      if (TimeBeforeRandomItem > 0) {
        TimeBeforeRandomItem--;
      }
      else {
        UsingItemTime = UsingItemTime_Const;
        TimeBeforeRandomItem = TimeBeforeRandomItem_Const;
        if (gameItemMode == Gacha_Item_Mode) {
          player[1].setItemId(skillMystery_Box_ID);
          player[2].setItemId(skillMystery_Box_ID);
        }
        else {
          generateItem(1);
          player[2].setItemId(player[1].getItemID());
          player[2].setLastItemId(player[1].getItemID());
        }
        PlaySFX(UsingItem_start_sfx_ID);
      }
    }
  }

  ///reset
  for (int i = 0; i < KEY_PRESS_TOTAL; i++) {
    old_key[i] = new_key[i];
    new_key[i] = 0;
  }
}

void Game::render_GameBackground() {
  for (Entity& e : waterBackground) {
    render(e);
    int x = e.getX() - 1;
    if (x + e.getWidth() <= 0) x = SCREEN_WIDTH;
    e.setX(x);
  }

  for (int j = 0; j < 13; j++)
    for (int i = 0; i * 96 < SCREEN_WIDTH; i++)
      render(GameTexture[grassTexture], i * 96, j * 32);

  for (int j = 0; j < 13; j++)
    for (int i = 0; i * 96 < SCREEN_WIDTH; i++)
      render(GameTexture[grassTexture], i * 96, SCREEN_HEIGHT - 32 - j * 32);
}

void Game::render_Player() {
  render_Skill_Hud_And_Cooldown();
  PlayPlayerSFX();

  if (player[1].getHealth() > 0) {
    if (player[1].getVulnerable() > 0) {
      Entity effect = skillW_effect;
      effect.setX(player[1].getX() + player[1].getWidth() / 2 - effect.getWidth() / 2);
      effect.setY(player[1].getY() + player[1].getHeight() - effect.getHeight() / 2);
      if (player[1].isShowing())
      render(effect);
    }
    if (player[1].isInvulnerable())
      player[1].setTexture(GameTexture[ezrealHourglassTexture]);
    else
      player[1].setTexture(GameTexture[ezrealTexture]);
    if (player[1].isShowing())
      render(player[1]);

    if (gameDelayTime == 0) {
      player_Arrow[1].setX(player[1].getX() + player[1].getWidth() / 2 - player_Arrow[1].getWidth() / 2);
      player_Arrow[1].setY(player[1].getY() + player[1].getHeight() + ArrowDistanceToPlayer);
      player_Arrow[1].setRotPoint(player_Arrow[1].getX() + player_Arrow[1].getWidth() / 2, player_Arrow[1].getY());
      if (player[1].isShowing())
        render(player_Arrow[1]);
      player_Arrow[1].moveAngle();
    }
  }

  if (player[2].getHealth() > 0) {
    if (player[2].getVulnerable() > 0) {
      Entity effect = skillW_effect;
      effect.setX(player[2].getX() + player[2].getWidth() / 2 - effect.getWidth() / 2);
      effect.setY(player[2].getY() + player[2].getHeight() - effect.getHeight() / 2);
      if (player[2].isShowing())
        render(effect);
    }
    if (player[2].isInvulnerable())
      player[2].setTexture(GameTexture[ezrealHourglassTexture]);
    else
      player[2].setTexture(GameTexture[ezrealTexture]);
    if (player[2].isShowing())
      render(player[2]);

    if (gameDelayTime == 0) {
      player_Arrow[2].setX(player[2].getX() + player[2].getWidth() / 2 - player_Arrow[2].getWidth() / 2);
      player_Arrow[2].setY(player[2].getY() - player_Arrow[2].getHeight() - ArrowDistanceToPlayer);
      player_Arrow[2].setRotPoint(player_Arrow[2].getX() + player_Arrow[2].getWidth() / 2, player_Arrow[2].getY() + player_Arrow[2].getHeight());
      if (player[2].isShowing())
        render(player_Arrow[2]);
      player_Arrow[2].moveAngle();
    }
  }
}

void Game::render_Bullet() {
  for (int id = 1; id <= 2; id++) {
    for (Bullet &bullet : player_bullets[id]) {
      if (gameDelayTime > 0) {
        if (bullet.getSkillId() == skillR_ID) {
          if (player[id].getSkillDelay() == 0)
            render(bullet);
        }
        else
          render(bullet);
        continue;
      }

      int enemy_id = 3 - id;
      Player &Enemy = player[3 - id];

      if (bullet.getSkillId() == skillR_ID) {
        if (player[id].getSkillDelay() == 0)
          render(bullet);
      }
      else
        render(bullet);

      if (Enemy.isDead() == 0 && isPlayerCollidingBullet(player[enemy_id], bullet)) {
        switch (bullet.getSkillId()) {
          case (skillQ_ID): {
            PlaySFX(Q_hit_sfx_ID);
            if (player[id].getItemID() == skillVoltaic_Cyclosword_ID) {
              PlaySFX(Voltaic_Cyclosword_sfx_ID);
              Enemy.setEffectTime(skill_effectTime[player[id].getItemID()], player[id].getItemID());
            }
            int damge = skill_damage[skillQ_ID];
            if (Enemy.getVulnerable() > 0) {
              PlaySFX(W_hit_crashed_sfx_ID);
              damge += skill_damage[skillW_ID];
              Enemy.setVulnerable(0);
            }
            Enemy.setHealth(Enemy.getHealth() - damge);
            break;
          }
          case (skillW_ID): {
            PlaySFX(W_hit_sfx_ID);
            Enemy.setVulnerable(vulnerableTime);
            break;
          }
          case (skillR_ID): {
            if (Enemy.getDamagedDelay() == 0) {
              PlaySFX(R_hit_sfx_ID);
              int damage = skill_damage[skillR_ID];
              Enemy.setDamagedDelay(5 * FPS);
              if (Enemy.getVulnerable() > 0) {
                PlaySFX(W_hit_crashed_sfx_ID);
                damage += skill_damage[skillW_ID];
                Enemy.setVulnerable(0);
              }
              Enemy.setHealth(Enemy.getHealth() - damage);
            }
            break;
          }
        }
        if (bullet.getSkillId() != skillR_ID)
          player_bullets[id].erase(player_bullets[id].begin() + (&bullet - &player_bullets[id][0]));
        else
          bullet.move();
      }
      else if (!bullet.isOnScreen()) {
        player_bullets[id].erase(player_bullets[id].begin() + (&bullet - &player_bullets[id][0]));
      }
      else {
        if (bullet.getSkillId() == skillR_ID) {
          if (player[id].getSkillDelay() == 0)
            bullet.move();
        }
        else
          bullet.move();
      }
    }
  }
}

void Game::render_GameEndButtons() {
  if (button[PlayAgain_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    resetGame();
    gameState = GamePlay;
  }
  button[PlayAgain_Button].setX(SCREEN_WIDTH / 2 - button[PlayAgain_Button].getWidth() / 2);
  button[PlayAgain_Button].setY(SCREEN_HEIGHT / 2 - button[PlayAgain_Button].getHeight() - 50);
  render(button[PlayAgain_Button]);

  if (button[BackToMenu_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameState = MainMenu;
  }
  button[BackToMenu_Button].setX(SCREEN_WIDTH / 2 - button[BackToMenu_Button].getWidth() / 2);
  button[BackToMenu_Button].setY(button[PlayAgain_Button].getY() + button[PlayAgain_Button].getHeight() + 10);
  render(button[BackToMenu_Button]);

  if (button[Exit_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameRunning = false;
  }
  button[Exit_Button].setX(SCREEN_WIDTH / 2 - button[Exit_Button].getWidth() / 2);
  button[Exit_Button].setY(button[BackToMenu_Button].getY() + button[BackToMenu_Button].getHeight() + 10);
  render(button[Exit_Button]);
}

void Game::render_Credit() {
  render(Background[Credit], 0, 0);
  if (gameDelayTime == 0) {
    PlaySFX(Click_sfx_ID);
    gameState = MainMenu;
  }
  else {
    if (gameDelayTime <= 4 * FPS) {
      if (gameDelayTime == 4 * FPS)
        PlaySFX(Click_sfx_ID);
      render(Icon[Github_Icon].getTexture(), Icon[Github_Icon].getX(), Icon[Github_Icon].getY() + sin(SDL_GetTicks64() / 100) * 2, Icon[Github_Icon].getWidth(), Icon[Github_Icon].getHeight());
      render(CreditEntity[Main_Credit].getTexture(), CreditEntity[Main_Credit].getX(), CreditEntity[Main_Credit].getY() + sin(SDL_GetTicks64() / 100) * 2, CreditEntity[Main_Credit].getWidth(), CreditEntity[Main_Credit].getHeight());
    }
    if (gameDelayTime <= 2 * FPS) {
      if (gameDelayTime == 2 * FPS)
        PlaySFX(Click_sfx_ID);
      render(CreditEntity[Sub_Credit].getTexture(), CreditEntity[Sub_Credit].getX(), CreditEntity[Sub_Credit].getY() + sin(SDL_GetTicks64() / 100) * 2, CreditEntity[Sub_Credit].getWidth(), CreditEntity[Sub_Credit].getHeight());
    }
  }
  gameDelayTime--;
}

void Game::render_MainMenu() {
  render(Background[MainMenu], 0, 0);

  if (button[VSAI_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    player[1].setType(Bot);
    player[2].setType(Human);
    gameVSType = VSAI_Type;
    gameState = SelectAIMode;
  }
  button[VSAI_Button].setX(SCREEN_WIDTH / 2 - button[VSAI_Button].getWidth() / 2);
  button[VSAI_Button].setY(SCREEN_HEIGHT / 2 - button[VSAI_Button].getHeight() - 100);
  render(button[VSAI_Button]);

  if (button[VSPlayer_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    player[1].setType(Human);
    player[2].setType(Human);
    gameVSType = VSPlayer_Type;
    gameState = SelectMode;
  }
  button[VSPlayer_Button].setX(SCREEN_WIDTH / 2 - button[VSPlayer_Button].getWidth() / 2);
  button[VSPlayer_Button].setY(button[VSAI_Button].getY() + button[VSAI_Button].getHeight() + 10);
  render(button[VSPlayer_Button]);

  if (button[Help_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameState = Help1;
  }
  button[Help_Button].setX(SCREEN_WIDTH / 2 - button[Help_Button].getWidth() / 2);
  button[Help_Button].setY(button[VSPlayer_Button].getY() + button[VSPlayer_Button].getHeight() + 10);
  render(button[Help_Button]);

  if (button[Exit_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameRunning = false;
  }
  button[Exit_Button].setX(SCREEN_WIDTH / 2 - button[Exit_Button].getWidth() / 2);
  button[Exit_Button].setY(button[Help_Button].getY() + button[Help_Button].getHeight() + 10);
  render(button[Exit_Button]);
}

void Game::render_SelectAIMode() {
  render(Background[SelectAIMode], 0, 0);

  if (button[Normal_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameAIMode = AIGameMode_Normal;
    gameState = SelectMode;
  }
  button[Normal_Button].setX(SCREEN_WIDTH / 2 - button[Normal_Button].getWidth() / 2);
  button[Normal_Button].setY(SCREEN_HEIGHT / 2 - button[Normal_Button].getHeight() - 100);
  render(button[Normal_Button]);

  if (button[Hard_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameAIMode = AIGameMode_Hard;
    gameState = SelectMode;
  }
  button[Hard_Button].setX(SCREEN_WIDTH / 2 - button[Hard_Button].getWidth() / 2);
  button[Hard_Button].setY(button[Normal_Button].getY() + button[Normal_Button].getHeight() + 10);
  render(button[Hard_Button]);

  if (button[Impossible_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameAIMode = AIGameMode_Impossible;
    gameState = SelectMode;
  }
  button[Impossible_Button].setX(SCREEN_WIDTH / 2 - button[Impossible_Button].getWidth() / 2);
  button[Impossible_Button].setY(button[Hard_Button].getY() + button[Hard_Button].getHeight() + 10);
  render(button[Impossible_Button]);

  if (button[Back_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameState = MainMenu;
  }
  button[Back_Button].setX(SCREEN_WIDTH / 2 - button[Back_Button].getWidth() / 2);
  button[Back_Button].setY(button[Impossible_Button].getY() + button[Impossible_Button].getHeight() + 10);
  render(button[Back_Button]);
}

void Game::render_SelectMode() {
  render(Background[SelectMode], 0, 0);

  if (button[Classic_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    resetGame();
    gameMode = Classic_Mode;
    gameState = GamePlay;
  }
  button[Classic_Button].setX(SCREEN_WIDTH / 2 - button[Classic_Button].getWidth() / 2);
  button[Classic_Button].setY(SCREEN_HEIGHT / 2 - button[Classic_Button].getHeight() - 100);
  render(button[Classic_Button]);

  if (button[Special_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    resetGame();
    gameMode = Special_Mode;
    gameState = SelectItemMode;
  }
  button[Special_Button].setX(SCREEN_WIDTH / 2 - button[Special_Button].getWidth() / 2);
  button[Special_Button].setY(button[Classic_Button].getY() + button[Classic_Button].getHeight() + 10);
  render(button[Special_Button]);

  if (button[Back_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    if (gameVSType == VSAI_Type)
      gameState = SelectAIMode;
    else if (gameVSType == VSPlayer_Type)
      gameState = MainMenu;
  }
  button[Back_Button].setX(SCREEN_WIDTH / 2 - button[Back_Button].getWidth() / 2);
  button[Back_Button].setY(button[Special_Button].getY() + button[Special_Button].getHeight() + 10);
  render(button[Back_Button]);
}

void Game::render_SelectItemMode() {
  render(Background[SelectItemMode], 0, 0);

  if (button[Gacha_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    resetGame();
    gameItemMode = Gacha_Item_Mode;
    gameState = GamePlay;
  }
  button[Gacha_Button].setX(SCREEN_WIDTH / 2 - button[Gacha_Button].getWidth() / 2);
  button[Gacha_Button].setY(SCREEN_HEIGHT / 2 - button[Gacha_Button].getHeight() - 100);
  render(button[Gacha_Button]);

  if (button[All_Random_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    resetGame();
    gameItemMode = All_Random_Item_Mode;
    gameState = GamePlay;
  }
  button[All_Random_Button].setX(SCREEN_WIDTH / 2 - button[All_Random_Button].getWidth() / 2);
  button[All_Random_Button].setY(button[Gacha_Button].getY() + button[Gacha_Button].getHeight() + 10);
  render(button[All_Random_Button]);

  if (button[Back_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameState = SelectMode;
  }
  button[Back_Button].setX(SCREEN_WIDTH / 2 - button[Back_Button].getWidth() / 2);
  button[Back_Button].setY(button[All_Random_Button].getY() + button[All_Random_Button].getHeight() + 10);
  render(button[Back_Button]);
}

void Game::render_GamePlay() {
  if (gameTimer == 0 || player[1].isDead() || player[2].isDead()) {
    if (gameEndDelayTime > 0) gameEndDelayTime--;
    else {
      Mix_Resume(-1);
      PlayMusic(GameEnd_music_ID);
      if ((player[1].isDead() && player[2].isDead()) || (player[1].getHealth() == player[2].getHealth())) {
        gameState = GameDraw;
      }
      else if (player[1].getType() == Bot) {
        if (player[1].getHealth() < player[2].getHealth())
          gameState = YouWin;
        else
          gameState = YouLose;
      }
      else {
        if (player[1].getHealth() < player[2].getHealth())
          gameState = Player2Win;
        else
          gameState = Player1Win;
      }
      return;
    }
  }
  render_GameBackground();
  render_Player();
  if (gameTimer > 0) render_Bullet();
  {
    string text = TimeToString(gameTimer, Countdown_TimeConvertType);
    renderTextCenter(SCREEN_WIDTH / 2, 50, text, Font[font32], black);
    if (gameDelayTime == 0 && gameTimer > 0) gameTimer--;
  }
  if (gameDelayTime > 0)
    Mix_Pause(-1);
  else
    Mix_Resume(-1);
  Mix_Resume(Click_sfx_Channel);
  if (2 * FPS < gameDelayTime && gameDelayTime <= 3 * FPS) {
    Mix_Resume(Count_down_sfx_Channel);
    if (gameDelayTime == 3 * FPS)
      PlaySFX(Count_down_sfx_ID);
    renderCenter(GameAnimation[count_down1], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  }
  if (1 * FPS < gameDelayTime && gameDelayTime <= 2 * FPS) {
    Mix_Resume(Count_down_sfx_Channel);
    if (gameDelayTime == 2 * FPS)
      PlaySFX(Count_down_sfx_ID);
    renderCenter(GameAnimation[count_down2], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  }
  if (0 * FPS < gameDelayTime && gameDelayTime <= 1 * FPS) {
    Mix_Resume(Count_down_sfx_Channel);
    if (gameDelayTime == 1 * FPS)
      PlaySFX(Count_down_sfx_ID);
    renderCenter(GameAnimation[count_down3], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  }
  if (gameDelayTime == 1) {
    Mix_Resume(Count_down_start_sfx_Channel);
    PlaySFX(Count_down_start_sfx_ID);
  }
  if (gameDelayTime > 0) gameDelayTime--;
}

void Game::render_GamePause() {
  Mix_Pause(-1);
  render(Background[GamePause], 0, 0);

  if (button[Continue_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    Mix_Resume(Click_sfx_Channel);
    PlaySFX(Click_sfx_ID); ///error
    resetGameDelayTime(3 * FPS);
    gameState = GamePlay;
  }
  button[Continue_Button].setX(SCREEN_WIDTH / 2 - button[Continue_Button].getWidth() / 2);
  button[Continue_Button].setY(SCREEN_HEIGHT / 2 - button[Continue_Button].getHeight() - 100);
  render(button[Continue_Button]);

  if (button[Restart_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    Mix_HaltChannel(-1);
    Mix_Resume(-1);
    PlaySFX(Click_sfx_ID);
    resetGame();
    gameState = GamePlay;
  }
  button[Restart_Button].setX(SCREEN_WIDTH / 2 - button[Restart_Button].getWidth() / 2);
  button[Restart_Button].setY(button[Continue_Button].getY() + button[Continue_Button].getHeight() + 10);
  render(button[Restart_Button]);

  if (button[BackToMenu_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    Mix_HaltChannel(-1);
    Mix_Resume(-1);
    PlaySFX(Click_sfx_ID);
    gameState = MainMenu;
  }
  button[BackToMenu_Button].setX(SCREEN_WIDTH / 2 - button[BackToMenu_Button].getWidth() / 2);
  button[BackToMenu_Button].setY(button[Restart_Button].getY() + button[Restart_Button].getHeight() + 10);
  render(button[BackToMenu_Button]);

  if (button[Exit_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    Mix_HaltChannel(-1);
    Mix_Resume(-1);
    PlaySFX(Click_sfx_ID);
    gameRunning = false;
  }
  button[Exit_Button].setX(SCREEN_WIDTH / 2 - button[Exit_Button].getWidth() / 2);
  button[Exit_Button].setY(button[BackToMenu_Button].getY() + button[BackToMenu_Button].getHeight() + 10);
  render(button[Exit_Button]);
}

void Game::render_YouWin() {
  render(Background[YouWin], 0, 0);
  render_GameEndButtons();
}

void Game::render_YouLose() {
  render(Background[YouLose], 0, 0);
  render_GameEndButtons();
}

void Game::render_Player1Win() {
  render(Background[Player1Win], 0, 0);
  render_GameEndButtons();
}

void Game::render_Player2Win() {
  render(Background[Player2Win], 0, 0);
  render_GameEndButtons();
}

void Game::render_GameDraw() {
  render(Background[GameDraw], 0, 0);
  render_GameEndButtons();
}

void Game::render_Help() {
  render(Background[gameState], 0, 0);

  if (button[Back_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameState--;
    if (gameState <= Help_gameState_start)
      gameState = MainMenu;
  }
  button[Back_Button].setX(SCREEN_WIDTH / 2 - button[Back_Button].getWidth() / 2 - 200);
  button[Back_Button].setY(SCREEN_HEIGHT - 175);
  render(button[Back_Button]);

 if (button[Next_Button].isClicked(MouseX, MouseY, mouseClicked)) {
    PlaySFX(Click_sfx_ID);
    gameState++;
    if (gameState >= Help_gameState_end)
      gameState = MainMenu;
  }
  button[Next_Button].setX(SCREEN_WIDTH / 2 - button[Next_Button].getWidth() / 2 + 200);
  button[Next_Button].setY(SCREEN_HEIGHT - 175);
  render(button[Next_Button]);
}

void Game::render_Game() {
  if (gameState == Credit) {
      render_Credit();
  }
  else if (gameState == MainMenu) {
    render_MainMenu();
  }
  else if (gameState == SelectAIMode) {
    render_SelectAIMode();
  }
  else if (gameState == SelectMode) {
    render_SelectMode();
  }
  else if (gameState == SelectItemMode) {
    render_SelectItemMode();
  }
  else if (gameState == GamePlay) {
    render_GamePlay();
  }
  else if (gameState == GamePause) {
    render_GamePause();
  }
  else if (gameState == YouWin) {
    render_YouWin();
  }
  else if (gameState == YouLose) {
    render_YouLose();
  }
  else if (gameState == Player1Win) {
    render_Player1Win();
  }
  else if (gameState == Player2Win) {
    render_Player2Win();
  }
  else if (gameState == GameDraw) {
    render_GameDraw();
  }
  else if (Help_gameState_start < gameState && gameState < Help_gameState_end) {
    render_Help();
  }
}

void Game::gameLoop() {
  clear();
  handleEvents();
  update();
  render_Game();
  display();
}