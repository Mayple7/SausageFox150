#ifndef FOX_ENGINE
#define FOX_ENGINE

#define FALSE 0
#define TRUE 1

#define GROUNDLEVEL -350

//-------------------------------------------------------------------------
// Fox engine includes

#include "FoxMath.h"
#include "Camera.h"
#include "Movement.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "MemoryManager.h"
#include "RigidBody.h"
#include "Sprite.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "SearchIntArray.h"
#include "TextCreation.h"
#include "Sound.h"
#include "System.h"
#include "FrameRate.h"
#include "Input.h"
#include "HUD.h"

//Special level include
#include "PauseMenu.h"

int pause;

Matrix3 LoadRatioMtx;

float GetLoadRatio(void);
void SetLoadRatio(double ratio);

int GetWindowWidth(void);
int GetWindowHeight(void);

void SaveSettings(void);

int Cheats;
int LookAtMouse;//If you like that sort of thing, life is about choice isn't it?
char Settings[200];
char GameData[200];

#endif