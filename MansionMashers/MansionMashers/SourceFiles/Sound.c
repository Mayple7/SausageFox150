/*
/*****************************************************************************/
/*!
\file				Sound.c
\author				Juli Gregg (j.gregg)
\date				Feb 2, 2014

\brief				Functions for Sound/FMOD

\par				Functions:
\li					GetSoundSystem
\li					FMODErrCheck
\li					FMODInit
\li					FMODQuit
\li					CreateSound
\li					PlayAudio
\li					ReleaseSound
\li					InitSoundStruct
\li					TogglePauseSound
\li					ToggglePauseChannel

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/ 

#include "../FMODHeaders/fmod.h"
#include "../FMODHeaders/fmod_errors.h"
#include "../HeaderFiles/FoxEngine.h"



/*	In SystemInit:	FMOD_System_Create, FMODInit
	In Update:	FMOD_System_Update
	In LvlInit: create sounds, create channel groups
	In LvlUpdate: play sound
	In LvlFree: sound release, release channel groups
	In SystemExit: Close & release system
*/

static int success;
static FMOD_SYSTEM *FMsystem;

/*************************************************************************/
/*!
	\brief
	Gives a pointer to sound system

	\return
	FMOD System pointer.
*/
/*************************************************************************/
FMOD_SYSTEM * GetSoundSystem(void)
{
	return FMsystem;
}

/*************************************************************************/
/*!
	\brief
	Helper function for FMOD error checking

	\param result
	FMOD's return for error checking on functions
*/
/*************************************************************************/
void FMODErrCheck(FMOD_RESULT result)
{
	if(result != FMOD_OK)
		printf("FMOD Error: (%d) %s\n", result, FMOD_ErrorString(result));
	else
		success = TRUE;
}

/*************************************************************************/
/*!
	\brief
	Creates FMOD system (when game starts)
*/
/*************************************************************************/
void FMODInit(void)
{
	FMOD_RESULT result;

	result = FMOD_System_Create(&FMsystem);
	FMODErrCheck(result);

	result = FMOD_System_Init(FMsystem, MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
	FMODErrCheck(result);

	if(success == TRUE)
	{
		printf("FMOD CREATED AND INITIALIZED\n");
		success = FALSE;
	}

}

/*************************************************************************/
/*!
	\brief
	Quits FMOD system (when game ends)
*/
/*************************************************************************/
void FMODQuit(void)
{
	FMOD_RESULT result;

	result = FMOD_System_Close(FMsystem);
	FMODErrCheck(result);
	result = FMOD_System_Release(FMsystem);
	FMODErrCheck(result);

	if(success == TRUE)
	{
		printf("FMOD CLOSED AND RELEASED\n");
		success = FALSE;
	}
}

/*************************************************************************/
/*!
	\brief
	Creates a sound from file

	\param Filename
	Name of the sound file

	\param snd
	A pointer to a sound object struct

	\param type
	Used to tell if object should be stored in memory or streamed
*/
/*************************************************************************/
void CreateSound(char *Filename, FoxSound *snd, int type)
{
	FMOD_RESULT result;
	InitSoundStruct(snd, type);
	
	if(type == SmallSnd)
		result = FMOD_System_CreateSound(FMsystem, Filename, FMOD_DEFAULT, NULL, &snd->Sound);
	else if(type == LargeSnd)
		result = FMOD_System_CreateStream(FMsystem, Filename, FMOD_DEFAULT, NULL, &snd->Sound);
	FMODErrCheck(result);
	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Plays the sound, checks if sound is playing so won't restart sound

	\param snd
	Pointer to a sound object struct

*/
/*************************************************************************/
void PlayAudio(FoxSound *snd, FoxChannels *channels)
{
	FMOD_RESULT result;
	
	if(snd->Channel)
	{
		result = FMOD_Channel_IsPlaying(snd->Channel, &snd->Playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		    FMODErrCheck(result);
	}
	
	
	if(snd->Playing == FALSE)
	{
		result = FMOD_System_PlaySound(FMsystem, FMOD_CHANNEL_FREE, snd->Sound, TRUE, &snd->Channel);
		FMODErrCheck(result);
		snd->Paused = TRUE;
	
		if(snd->Type == SmallSnd)
		{
			result = FMOD_Channel_SetChannelGroup(snd->Channel, channels->Effects);
			FMODErrCheck(result);
		}
		else if(snd->Type == LargeSnd)
		{
			result = FMOD_Channel_SetChannelGroup(snd->Channel, channels->Music);
			FMODErrCheck(result);
		}

		TogglePauseSound(snd);
	}

	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Allows fmod to free sound

	\param sound
	A pointer to the sound to be freed
*/
/*************************************************************************/
void ReleaseSound(FMOD_SOUND *sound)
{
	FMOD_RESULT result;
	result = FMOD_Sound_Release(sound);
	FMODErrCheck(result);
	
	if(success == TRUE)
	{
		printf("SOUND RELEASED\n");
		success = FALSE;
	}
}

/*************************************************************************/
/*!
	\brief
	Initializes a sound struct when sound is created

	\param snd
	A pointer to a sound object struct

	\param type
	Label of what kind of sound this is.
*/
/*************************************************************************/
void InitSoundStruct(FoxSound *snd, int type)
{
	snd->Sound = 0;
	snd->Channel = 0;
	snd->Playing = FALSE;
	snd->Paused = FALSE;
	snd->Type = type;
}

/*************************************************************************/
/*!
	\brief
	Pauses/UnPauses a sound/channel

	\param
	Pointer to sound object struct
*/
/*************************************************************************/

void TogglePauseSound(FoxSound * snd)
{
	FMOD_RESULT result;

	if(snd->Paused == FALSE)
	{
		result = FMOD_Channel_SetPaused(snd->Channel, TRUE);
		FMODErrCheck(result);
		snd->Paused = TRUE;
	}
	else if(snd->Paused == TRUE)
	{
		result = FMOD_Channel_SetPaused(snd->Channel, FALSE);
		FMODErrCheck(result);
		snd->Paused = FALSE;
	}
	
	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Pauses/UnPauses all sounds on a channel

	\param
	Pointer to channel group object struct
*/
/*************************************************************************/

void TogglePauseChannel(FoxChannels * chnls, int ChnlType)
{
	FMOD_RESULT result;

	if(ChnlType == EffectType)
	{
		if(chnls->EffectsPaused == FALSE)
		{
			result = FMOD_ChannelGroup_SetPaused(chnls->Effects, TRUE);
			FMODErrCheck(result);
			chnls->EffectsPaused = TRUE;
		}
		else if(chnls->EffectsPaused == TRUE)
		{
			result = FMOD_ChannelGroup_SetPaused(chnls->Effects, FALSE);
			FMODErrCheck(result);
			chnls->EffectsPaused = FALSE;
		}
	}
	else if(ChnlType == MusicType)
	{
		if(chnls->MusicPaused == FALSE)
		{
			result = FMOD_ChannelGroup_SetPaused(chnls->Music, TRUE);
			FMODErrCheck(result);
			chnls->MusicPaused = TRUE;
		}
		else if(chnls->MusicPaused == TRUE)
		{
			result = FMOD_ChannelGroup_SetPaused(chnls->Music, FALSE);
			FMODErrCheck(result);
			chnls->MusicPaused = FALSE;
		}
	}
	
	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Create channel groups

	\param
	Pointer to channel group object struct
*/
/*************************************************************************/

void CreateChannelGroups(FoxChannels *chnl)
{
	FMOD_RESULT result;

	result = FMOD_System_CreateChannelGroup(FMsystem, NULL, &chnl->Effects);
	FMODErrCheck(result);

	result = FMOD_System_CreateChannelGroup(FMsystem, NULL, &chnl->Music);
	FMODErrCheck(result);

	success = FALSE;

}

/*************************************************************************/
/*!
	\brief
	FMOD releases channel groups

	\param
	Pointer to channel group object struct
*/
/*************************************************************************/
void ReleaseChannelGroups(FoxChannels *chnl)
{
	FMOD_RESULT result;

	result = FMOD_ChannelGroup_Release(chnl->Effects);
	FMODErrCheck(result);
	result = FMOD_ChannelGroup_Release(chnl->Music);

	success = FALSE;
}