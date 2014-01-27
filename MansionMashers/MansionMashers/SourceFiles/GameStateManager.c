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
#include "../HeaderFiles/StartSlides.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/TestLevel.h"
#include "../HeaderFiles/LevelShowcase.h"

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
		case GS_TestLevel:
			GSMPointers.pLoad = LoadTestLevel;
			GSMPointers.pInit = InitializeTestLevel;
			GSMPointers.pUpdate = UpdateTestLevel;
			GSMPointers.pFree = FreeTestLevel;
			GSMPointers.pDraw = DrawTestLevel;
			GSMPointers.pUnload = UnloadTestLevel;
			break;
		case GS_ShowcaseLevel:
			GSMPointers.pLoad = LoadShowcase;
			GSMPointers.pInit = InitializeShowcase;
			GSMPointers.pUpdate = UpdateShowcase;
			GSMPointers.pFree = FreeShowcase;
			GSMPointers.pDraw = DrawShowcase;
			GSMPointers.pUnload = UnloadShowcase;
			break;
		case GS_Level1:
			break;
		case GS_Level2:
			break;
	}
}