/*
File:				GameStateManager.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Jan 20, 2014

Purpose:			Managers running levels and switching levels

Functions:			
 
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/

// ---------------------------------------------------------------------------
// Includes
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/GSTestLevel.h"

// ---------------------------------------------------------------------------
// Globals

struct GameState State;

// ---------------------------------------------------------------------------
// Function Prototypes

void GSMInitialize(int InitState);
int GetPreviousState(void);
int GetCurrentState(void);
int GetNextState(void);
void SetNextState(int NextState);
void GSMUpdate(int CurState);

// ---------------------------------------------------------------------------
// Main Program

//Initializes states to same level to start
void GSMInitialize(int InitState)
{
	State.Previous = InitState;
	State.Current = InitState;
	State.Next = InitState;
}

//Previous
int GetPreviousState(void)
{
	return State.Previous;
}

//Current
int GetCurrentState(void)
{
	return State.Current;
}

//Next
int GetNextState(void)
{
	return State.Next;
}

//Set state for changing level
void SetNextState(int NextState)
{
	State.Next = NextState;
}

void GSMUpdate(int CurState)
{
	switch (CurState)
	{
		case GS_MainMenu:
			break;
		case GS_Level1:
			break;
		case GS_Level2:
			GSMPointers.pLoad = Load2;
			GSMPointers.pInit = Init2;
			GSMPointers.pUpdate = Update2;
			GSMPointers.pFree = Free2;
			GSMPointers.pDraw = Draw2;
			GSMPointers.pUnload = Unload2;
			break;
	}
}