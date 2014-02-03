/*
File:				Sound.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Feb 2, 2014

Purpose:			Sound Functions

Functions:			
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

#include "../fmod.h"
#include "../fmod_errors.h"
#include <stdio.h>
#include "../HeaderFiles/Sound.h"



/*	In SystemInit:	FMOD_System_Create, FMODInit
	In Update:	FMOD_System_Update
	In LvlLoad: create sounds
	In LvlUpdate: playsound
	In LvlUnload: sound release
	In SystemExit: Close & release system
*/

int success = 0;


void FMODErrCheck(FMOD_RESULT result)
{
	if(result != FMOD_OK)
		printf("FMOD Error: (%d) %s\n", result, FMOD_ErrorString(result));
	else
		success = 1;
}


void FMODInit(FMOD_SYSTEM *system)
{
	FMOD_RESULT result;

	result = FMOD_System_Create(&system);
	FMODErrCheck(result);

	result = FMOD_System_Init(system, MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
	FMODErrCheck(result);

	if(success = 1)
	{
		printf("FMOD CREATED AND INITIALIZED\n");
		success = 0;
	}

}

void FMODQuit(FMOD_SYSTEM *system)
{
	FMOD_RESULT result;

	result = FMOD_System_Close(system);
	FMODErrCheck(result);
	result = FMOD_System_Release(system);
	FMODErrCheck(result);

	if(success = 1)
	{
		printf("FMOD CLOSED AND RELEASED\n");
		success = 0;
	}
}