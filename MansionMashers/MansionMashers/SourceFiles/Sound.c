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
\li					CreateChannelGroups
\li					ReleaseChannelGroups
\li					UpdateSoundSystem

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
FoxSound *CreateSound(char *Filename, int type)
{
	FMOD_RESULT result;
	FoxSound *snd = AddSound();

	InitSoundStruct(snd, type);
	
	if(type == SmallSnd)
		result = FMOD_System_CreateSound(FMsystem, Filename, FMOD_DEFAULT, NULL, &snd->Sound);
	else if(type == LargeSnd)
		result = FMOD_System_CreateStream(FMsystem, Filename, FMOD_DEFAULT, NULL, &snd->Sound);
	FMODErrCheck(result);
	success = FALSE;

	return snd;
}

/*************************************************************************/
/*!
	\brief
	Plays the sound, checks if sound is playing so won't restart sound

	\param snd
	Pointer to a sound object struct

*/
/*************************************************************************/
void PlayAudio(FoxSound *snd)
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
			result = FMOD_Channel_SetChannelGroup(snd->Channel, ChannelController->Effects);
			FMODErrCheck(result);
		}
		else if(snd->Type == LargeSnd)
		{
			result = FMOD_Channel_SetChannelGroup(snd->Channel, ChannelController->Music);
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
	snd->sndID = 1;
}

/*************************************************************************/
/*!
	\brief
	Pauses/UnPauses a sound/channel

	\param
	Pointer to sound object struct
*/
/*************************************************************************/

void TogglePauseSound(FoxSound *snd)
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

void TogglePauseChannel(int ChnlType)
{
	FMOD_RESULT result;

	if(ChnlType == EffectType)
	{
		if(ChannelController->EffectsPaused == FALSE)
		{
			result = FMOD_ChannelGroup_SetPaused(ChannelController->Effects, TRUE);
			FMODErrCheck(result);
			ChannelController->EffectsPaused = TRUE;
		}
		else if(ChannelController->EffectsPaused == TRUE)
		{
			result = FMOD_ChannelGroup_SetPaused(ChannelController->Effects, FALSE);
			FMODErrCheck(result);
			ChannelController->EffectsPaused = FALSE;
		}
	}
	else if(ChnlType == MusicType)
	{
		if(ChannelController->MusicPaused == FALSE)
		{
			result = FMOD_ChannelGroup_SetPaused(ChannelController->Music, TRUE);
			FMODErrCheck(result);
			ChannelController->MusicPaused = TRUE;
		}
		else if(ChannelController->MusicPaused == TRUE)
		{
			result = FMOD_ChannelGroup_SetPaused(ChannelController->Music, FALSE);
			FMODErrCheck(result);
			ChannelController->MusicPaused = FALSE;
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

FoxChannels* CreateChannelGroups(void)
{
	FMOD_RESULT result;
	FoxChannels * chnl = AddChannelGroups();

	chnl->sndID = 1;
	chnl->EffectsPaused = FALSE;
	chnl->MusicPaused = FALSE;

	result = FMOD_System_CreateChannelGroup(FMsystem, NULL, &chnl->Effects);
	FMODErrCheck(result);

	result = FMOD_System_CreateChannelGroup(FMsystem, NULL, &chnl->Music);
	FMODErrCheck(result);

	success = FALSE;

	return chnl;
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
	FMODErrCheck(result);

	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Set a channel group's volume

	\param chnl
	Pointer to channel group object struct
	
	\param type
	Which channel group to adjust

	\param volume
	Value to set channel group to
*/
/*************************************************************************/
void SetChannelGroupVolume(int type, float volume)
{
	FMOD_RESULT result;

	switch (type)
	{
		case AllTypes:
			result = FMOD_ChannelGroup_SetVolume(ChannelController->Music, volume);
			FMODErrCheck(result);
		case EffectType:
			result = FMOD_ChannelGroup_SetVolume(ChannelController->Effects, volume);
			FMODErrCheck(result);
			break;
		case MusicType:
			result = FMOD_ChannelGroup_SetVolume(ChannelController->Music, volume);
			FMODErrCheck(result);
			break;
	}

	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Set a channels/sounds volume

	\param snd
	Pointer to sound object struct

	\param volume
	Value to set sound to
*/
/*************************************************************************/
void SetSoundVolume(FoxSound *snd, float volume)
{
	FMOD_RESULT result;

	result = FMOD_Channel_SetVolume(snd->Channel, volume);
	FMODErrCheck(result);

	success = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Gets the volume of a sound

	\param snd
	Pointer to sound object struct

	\return
	Returns the current volume of that sound
*/
/*************************************************************************/
float GetSoundVolume(FoxSound *snd)
{
	FMOD_RESULT result;
	float volume;

	result = FMOD_Channel_GetVolume(snd->Channel, &volume);
	FMODErrCheck(result);

	success = FALSE;

	return volume;
}

/*************************************************************************/
/*!
	\brief
	Gets the volume of a channel group

	\param snd
	Pointer to channel group object struct

	\param type
	Which channel group's volume to get

	\return
	Returns the current volume of specified channel group
*/
/*************************************************************************/
float GetChannelGroupVolume(int type)
{
	FMOD_RESULT result;
	float volume;

	switch (type)
	{
		case EffectType:
			result = FMOD_ChannelGroup_GetVolume(ChannelController->Effects, &volume);
			FMODErrCheck(result);
			break;
		case MusicType:
			result = FMOD_ChannelGroup_GetVolume(ChannelController->Music, &volume);
			FMODErrCheck(result);
			break;
	}

	success = FALSE;

	return volume;
}

/*************************************************************************/
/*!
	\brief
	Updates sound system.
*/
/*************************************************************************/
void UpdateSoundSystem(void)
{
	FMOD_System_Update(FMsystem);
}

/*************************************************************************/
/*!
	\brief
	Converts a number volume to a string

	\param
	Pointer to a character string

	\param 
	The number volume
*/
/*************************************************************************/
char * VolumetoString(char *string, float volume)
{
	int i;
	int voltemp = (int)volume;

	for(i = 0; i <= 4; i++)
	{
		if (voltemp == 100)
		{
			string[0] = '1';
			string[1] = '0';
			string[2] = '0';
			string[3] = '\0';
			break;
		}
		else if(voltemp > 9)
		{
			int temp = voltemp;
			temp /= 10;
			string[i] = temp + 48;
			voltemp -= 10 * temp;
			if(voltemp == 0)
			{
				string[i + 1] = '0';
				string[i + 2] = '\0';
				break;
			}
		}
		else if(voltemp >= 0 && voltemp <= 9)
		{
			string[i] = voltemp + 48;
			string[i + 1] = '\0';
			break;
		}
	}

	return string;
}

/*************************************************************************/
/*!
	\brief
	Adds a sound to the soundList

	\return
	Returns a pointer to a FoxSound struct
*/
/*************************************************************************/
FoxSound *AddSound(void)
{
	int i;

	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if(soundList[i].sndID == 0 || soundList[i].sndID == -1)
		{
			printf("sl : %i\n", &soundList[i]);
			return &soundList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a channel group to the channelGroupList

	\return
	Returns a pointer to a FoxChannels struct
*/
/*************************************************************************/
FoxChannels * AddChannelGroups(void)
{
	int i;

	for(i = 0; i < MAX_CHANNEL_GROUPS; i++)
	{
		if(channelGroupList[i].sndID == 0 || channelGroupList[i].sndID == -1)
			return &channelGroupList[i];
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Sets the objects in the soundList and channelGroupsList to zero

*/
/*************************************************************************/

void ResetSoundList(void)
{
	int i;

	soundList  = (FoxSound *) CallocMyAlloc(MAX_SOUND_CHANNELS, sizeof(FoxSound));

	if(soundList)
	{
		for(i = 0; i < MAX_SOUND_CHANNELS; i++)
		{
			soundList[i].sndID = -1;
		}
		printf("Sound List Setup Successful\n");
	}
	else
		printf("Sound List Setup Failed\n");

}

void ResetChannelGroupList(void)
{
	int i;

	channelGroupList = (FoxChannels *) CallocMyAlloc(MAX_CHANNEL_GROUPS, sizeof(FoxChannels));

	if(channelGroupList)
	{
		for(i = 0; i < MAX_CHANNEL_GROUPS; i++)
		{
			channelGroupList[i].sndID = -1;
		}
		printf("Channel List Setup Successful\n");
	}
	else
		printf("Channel List Setup Failed\n");

}

/*************************************************************************/
/*!
	\brief
	Frees a sound from the sound list

	\param
	Pointer to a FoxSound struct
*/
/*************************************************************************/
void freeSound(FoxSound * soundObj)
{
	if(soundObj && soundObj->sndID)
	{
		soundObj->sndID = 0;
		ReleaseSound(soundObj->Sound);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees a channel group from the list

	\param
	Pointer to channel group object struct
*/
/*************************************************************************/
void freeChannelGroups(FoxChannels * chanGrpObj)
{
	if(chanGrpObj && chanGrpObj->sndID)
	{
		chanGrpObj->sndID = 0;
		ReleaseChannelGroups(chanGrpObj);
	}
}

/*************************************************************************/
/*!
	\brief
	Frees sounds and channel groups from lists and release memory
*/
/*************************************************************************/
void FreeSoundList(void)
{
	int i;

	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if(soundList[i].sndID)
			freeSound(&soundList[i]);
	}

	for(i = 0; i < MAX_CHANNEL_GROUPS; i++)
	{
		if(channelGroupList[i].sndID)
			freeChannelGroups(&channelGroupList[i]);
	}
	
	FreeMyAlloc(soundList);
}

void FreeChannelGroupList(void)
{
	int i;

	for(i = 0; i < MAX_CHANNEL_GROUPS; i++)
	{
		if(channelGroupList[i].sndID)
			freeChannelGroups(&channelGroupList[i]);
	}
	
	FreeMyAlloc(channelGroupList);
}

void CreatePauseSound(FoxSound * snd, char *Filename, int type)
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