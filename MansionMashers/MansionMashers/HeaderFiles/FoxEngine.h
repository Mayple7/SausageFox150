#ifndef FOX_ENGINE
#define FOX_ENGINE

#define FALSE 0
#define TRUE 1

#define GROUNDLEVEL -350

//-------------------------------------------------------------------------
// Fox engine includes

#include "Camera.h"
#include "Movement.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "MemoryManager.h"
#include "Physics.h"
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

//Special level include
#include "PauseMenu.h"

float GetLoadRatio(void);
void SetLoadRatio(double ratio);

#endif