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

#include "../FMODHeaders/fmod.h"
#include "../FMODHeaders/fmod_errors.h"
//#include <stdio.h>  // There is no reason to include this -Dan
#include "../HeaderFiles/Sound.h"
#include "../HeaderFiles/FoxEngine.h"



/*	In SystemInit:	FMOD_System_Create, FMODInit
	In Update:	FMOD_System_Update
	In LvlLoad: create sounds
	In LvlUpdate: playsound
	In LvlUnload: sound release
	In SystemExit: Close & release system
*/

// Also don't initialize global variables it will cause problems in the future.
int success;
FMOD_SYSTEM *FMsystem;

FMOD_SYSTEM * GetSoundSystem(void)
{
	return FMsystem;
}

void FMODErrCheck(FMOD_RESULT result)
{
	if(result != FMOD_OK)
		printf("FMOD Error: (%d) %s\n", result, FMOD_ErrorString(result));
	else
		success = 1;
}


void FMODInit()
{
	FMOD_RESULT result;

	result = FMOD_System_Create(&FMsystem);
	FMODErrCheck(result);

	result = FMOD_System_Init(FMsystem, MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
	FMODErrCheck(result);

	if(success = 1)
	{
		printf("FMOD CREATED AND INITIALIZED\n");
		success = 0;
	}

}

void FMODQuit()
{
	FMOD_RESULT result;

	result = FMOD_System_Close(FMsystem);
	FMODErrCheck(result);
	result = FMOD_System_Release(FMsystem);
	FMODErrCheck(result);

	if(success = 1)
	{
		printf("FMOD CLOSED AND RELEASED\n");
		success = 0;
	}
}

void CreateSound(char *Filename, FMOD_SOUND **sound)
{
	FMOD_RESULT result;
	result = FMOD_System_CreateSound(FMsystem, Filename, FMOD_HARDWARE, 0, sound);
	FMODErrCheck(result);
	success = 0;
}


void PlayAudio(FoxSound *sound)
{
	FMOD_RESULT result;
	FMOD_BOOL Playing = FALSE;
	
	if(sound->channel)
	{
		result = FMOD_Channel_IsPlaying(sound->channel, &Playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
		    FMODErrCheck(result);
		}
	}
	
	
	if(Playing == FALSE)
	{
		result = FMOD_System_PlaySound(FMsystem, FMOD_CHANNEL_FREE, sound->sound, 0, &sound->channel);
		FMODErrCheck(result);
	}
	success = 0;
}

void ReleaseSound(FMOD_SOUND *sound)
{
	FMOD_RESULT result;
	result = FMOD_Sound_Release(sound);
	FMODErrCheck(result);
	
	if(success = 1)
	{
		printf("SOUND RELEASED\n");
		success = 0;
	}
}

void InitSoundStruct(FoxSound *snd)
{
	snd->sound = 0;
	snd->channel = 0;
}

/* result = FMOD_Channel_IsPlaying(channel, &playing);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))*/

