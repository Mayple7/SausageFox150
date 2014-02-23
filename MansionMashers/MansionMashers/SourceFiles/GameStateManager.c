/*****************************************************************************/
/*!
\file				GameStateManager.c
\author				Juli Gregg (j.gregg)
\date				Jan 20, 2014

\brief				Manages the levels and updates function pointers for switching levels

\par				Functions:
\li					GSMInitialize
\li					GetPreviousState
\li					GetCurrentState
\li					GetNextState
\li					SetPreviousState
\li					SetCurrentState
\li					SetNextState
\li					GSMUpdate
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// Includes
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/FoxLevels.h"
#include <stdio.h>

// ---------------------------------------------------------------------------
// Globals

struct GameState State;

// ---------------------------------------------------------------------------
// Function Prototypes

void GSMInitialize(int InitState);
int GetPreviousState(void);
int GetCurrentState(void);
int GetNextState(void);
void SetPreviousState(int PreviousState);
void SetCurrentState(int CurrentState);
void SetNextState(int NextState);
void GSMUpdate(int CurState);

// ---------------------------------------------------------------------------
// Main Program

/*************************************************************************/
/*!
	\brief
	Initializes states to the same level to start
	  
	\param InitState
	The initial state to start the game
*/
/*************************************************************************/
void GSMInitialize(int InitState)
{
	State.Previous = InitState;
	State.Current = InitState;
	State.Next = InitState;
}

/*************************************************************************/
/*!
	\brief
	Returns the previous state

	\return
	The enum of the previous state
*/
/*************************************************************************/
int GetPreviousState(void)
{
	return State.Previous;
}

/*************************************************************************/
/*!
	\brief
	Returns the current state
	
	\return
	The enum of the current state
*/
/*************************************************************************/
int GetCurrentState(void)
{
	return State.Current;
}

/*************************************************************************/
/*!
	\brief
	Returns the next state
	
	\return
	The enum of the next state
*/
/*************************************************************************/
int GetNextState(void)
{
	return State.Next;
}

/*************************************************************************/
/*!
	\brief
	Sets the previous state
	
	\param PreviousState
	The enum of the state to set
*/
/*************************************************************************/
void SetPreviousState(int PreviousState)
{
	if(PreviousState >= GS_MAX)
		printf("ERROR INVALID NEXT LEVEL STATE PARAMETER");
	else	
		State.Previous = PreviousState;
}

/*************************************************************************/
/*!
	\brief
	Sets the current state
	
	\param CurrentState
	The enum of the state to set
*/
/*************************************************************************/
void SetCurrentState(int CurrentState)
{
	if(CurrentState >= GS_MAX)
		printf("ERROR INVALID NEXT LEVEL STATE PARAMETER");
	else
		State.Current = CurrentState;
}

/*************************************************************************/
/*!
	\brief
	Sets the next state
	
	\param NextState
	The enum of the state to set
*/
/*************************************************************************/
void SetNextState(int NextState)
{
	if(NextState >= GS_MAX)
		printf("ERROR INVALID NEXT LEVEL STATE PARAMETER");
	else
		State.Next = NextState;
}

/*************************************************************************/
/*!
	\brief
	Updates all the function pointers for the current level
	
	\param CurState
	The enum of the state to use the function pointers
*/
/*************************************************************************/
void GSMUpdate(int CurState)
{
	switch (CurState)
	{
		case GS_SplashScreen:
			GSMPointers.pLoad = LoadStartScreen;
			GSMPointers.pInit = InitializeStartScreen;
			GSMPointers.pUpdate = UpdateStartScreen;
			GSMPointers.pFree = FreeStartScreen;
			GSMPointers.pDraw = DrawStartScreen;
			GSMPointers.pUnload = UnloadStartScreen;
			break;
		case GS_MainMenu:
			GSMPointers.pLoad = LoadMainMenu;
			GSMPointers.pInit = InitializeMainMenu;
			GSMPointers.pUpdate = UpdateMainMenu;
			GSMPointers.pFree = FreeMainMenu;
			GSMPointers.pDraw = DrawMainMenu;
			GSMPointers.pUnload = UnloadMainMenu;
			break;
		case GS_Tutorial:
			GSMPointers.pLoad = LoadTutorial;
			GSMPointers.pInit = InitializeTutorial;
			GSMPointers.pUpdate = UpdateTutorial;
			GSMPointers.pFree = FreeTutorial;
			GSMPointers.pDraw = DrawTutorial;
			GSMPointers.pUnload = UnloadTutorial;
			break;
		case GS_CharacterSelect:
			GSMPointers.pLoad = LoadCharacterSelect;
			GSMPointers.pInit = InitializeCharacterSelect;
			GSMPointers.pUpdate = UpdateCharacterSelect;
			GSMPointers.pFree = FreeCharacterSelect;
			GSMPointers.pDraw = DrawCharacterSelect;
			GSMPointers.pUnload = UnloadCharacterSelect;
			break;
		case GS_MapLevel:
			GSMPointers.pLoad = LoadMapLevel;
			GSMPointers.pInit = InitializeMapLevel;
			GSMPointers.pUpdate = UpdateMapLevel;
			GSMPointers.pFree = FreeMapLevel;
			GSMPointers.pDraw = DrawMapLevel;
			GSMPointers.pUnload = UnloadMapLevel;
			break;
		case GS_EPMenu:
			GSMPointers.pLoad = LoadEPMenu;
			GSMPointers.pInit = InitializeEPMenu;
			GSMPointers.pUpdate = UpdateEPMenu;
			GSMPointers.pFree = FreeEPMenu;
			GSMPointers.pDraw = DrawEPMenu;
			GSMPointers.pUnload = UnloadEPMenu;
			break;
		case GS_EP1Slides:
			GSMPointers.pLoad = LoadEP1Screen;
			GSMPointers.pInit = InitializeEP1Screen;
			GSMPointers.pUpdate = UpdateEP1Screen;
			GSMPointers.pFree = FreeEP1Screen;
			GSMPointers.pDraw = DrawEP1Screen;
			GSMPointers.pUnload = UnloadEP1Screen;
			break;
		case GS_EP2Slides:
			GSMPointers.pLoad = LoadEP2Screen;
			GSMPointers.pInit = InitializeEP2Screen;
			GSMPointers.pUpdate = UpdateEP2Screen;
			GSMPointers.pFree = FreeEP2Screen;
			GSMPointers.pDraw = DrawEP2Screen;
			GSMPointers.pUnload = UnloadEP2Screen;
			break;
		case GS_Level1:
			GSMPointers.pLoad = LoadLevel1;
			GSMPointers.pInit = InitializeLevel1;
			GSMPointers.pUpdate = UpdateLevel1;
			GSMPointers.pFree = FreeLevel1;
			GSMPointers.pDraw = DrawLevel1;
			GSMPointers.pUnload = UnloadLevel1;
			break;
		case GS_ArmGuy:
			GSMPointers.pLoad = LoadArmGuy;
			GSMPointers.pInit = InitializeArmGuy;
			GSMPointers.pUpdate = UpdateArmGuy;
			GSMPointers.pFree = FreeArmGuy;
			GSMPointers.pDraw = DrawArmGuy;
			GSMPointers.pUnload = UnloadArmGuy;
			break;
		case GS_Level2:
			GSMPointers.pLoad = LoadLevel2;
			GSMPointers.pInit = InitializeLevel2;
			GSMPointers.pUpdate = UpdateLevel2;
			GSMPointers.pFree = FreeLevel2;
			GSMPointers.pDraw = DrawLevel2;
			GSMPointers.pUnload = UnloadLevel2;
			break;
		case GS_HandGuy:
			GSMPointers.pLoad = LoadHandGuy;
			GSMPointers.pInit = InitializeHandGuy;
			GSMPointers.pUpdate = UpdateHandGuy;
			GSMPointers.pFree = FreeHandGuy;
			GSMPointers.pDraw = DrawHandGuy;
			GSMPointers.pUnload = UnloadHandGuy;
			break;
		case GS_Level3:
			GSMPointers.pLoad = LoadLevel3;
			GSMPointers.pInit = InitializeLevel3;
			GSMPointers.pUpdate = UpdateLevel3;
			GSMPointers.pFree = FreeLevel3;
			GSMPointers.pDraw = DrawLevel3;
			GSMPointers.pUnload = UnloadLevel3;
			break;
		case GS_YeahGuy:
			GSMPointers.pLoad = LoadYeahGuy;
			GSMPointers.pInit = InitializeYeahGuy;
			GSMPointers.pUpdate = UpdateYeahGuy;
			GSMPointers.pFree = FreeYeahGuy;
			GSMPointers.pDraw = DrawYeahGuy;
			GSMPointers.pUnload = UnloadYeahGuy;
			break;
		case GS_Level4:
			GSMPointers.pLoad = LoadLevel4;
			GSMPointers.pInit = InitializeLevel4;
			GSMPointers.pUpdate = UpdateLevel4;
			GSMPointers.pFree = FreeLevel4;
			GSMPointers.pDraw = DrawLevel4;
			GSMPointers.pUnload = UnloadLevel4;
			break;
		case GS_Kevin:
			GSMPointers.pLoad = LoadKevin;
			GSMPointers.pInit = InitializeKevin;
			GSMPointers.pUpdate = UpdateKevin;
			GSMPointers.pFree = FreeKevin;
			GSMPointers.pDraw = DrawKevin;
			GSMPointers.pUnload = UnloadKevin;
			break;
		case GS_DevMenu:
			GSMPointers.pLoad = LoadDevMenu;
			GSMPointers.pInit = InitializeDevMenu;
			GSMPointers.pUpdate = UpdateDevMenu;
			GSMPointers.pFree = FreeDevMenu;
			GSMPointers.pDraw = DrawDevMenu;
			GSMPointers.pUnload = UnloadDevMenu;
			break;
	}
}