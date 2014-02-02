/*
File:				System.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Feb 2, 2014

Purpose:			System Functions

Functions:			
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

#include <stdio.h>
#include "../fmod.h"
#include "../HeaderFiles/Sound.h"

FMOD_RESULT result;

void FoxSystemInitialize(FMOD_SYSTEM *FMsystem)
{
	FMODInit(FMsystem);
}

void FoxSystemExit(FMOD_SYSTEM *FMsystem)
{
	
	FMODQuit(FMsystem);

}
