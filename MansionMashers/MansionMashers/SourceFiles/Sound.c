/*
/*****************************************************************************/
/*!
\file				Sound.c
\author				Juli Gregg (j.gregg)
\date				Feb 2, 2014

\brief				Functions for Sound/FMOD

\par				Functions:

\li					FMODErrCheck

\li					FMODInit
\li					GetSoundSystem
\li					UpdateSoundSystem
\li					FMODQuit

\li					ResetSoundList
\li					AddSound
\li					FreeSoundList
\li					FreeSound

\li					ResetChannelGroupList
\li					AddChannelGroup
\li					FreeChannelGroupList
\li					FreeChannelGroup

\li					CreateSound
\li					CreatePauseSound
\li					InitSoundStruct
\li					ReleaseSound

\li					PlayAudio

\li					CreateChannelGroups
\li					ReleaseChannelGroups

\li					SetSoundVolume
\li					GetSoundVolume
\li					SetChannelGroupVolume
\li					GetChannelGroupVolume
\li					VolumetoString
\li					SaveVolume

\li					TogglePauseSound
\li					ToggglePauseChannel


\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/ 

// ---------------------------------------------------------------------------
// includes
#include "../FMODHeaders/fmod.h"
#include "../FMODHeaders/fmod_errors.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// globals

static int success;
static FMOD_SYSTEM *FMsystem;

// ---------------------------------------------------------------------------
// notes
/*	
	In SystemInit:	FMOD_System_Create, FMODInit, CreateChannelGroups, ResetChannelGroupList
	In Update:	FMOD_System_Update
	In LvlInit: ResetSoundList, CreateSounds
	In LvlUpdate: PlayAudio, TogglePause, Change Volume
	In LvlFree: FreeSoundList
	In SystemExit: FreeChannelGroupList, Close & release system
*/
// ---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////
// FMOD Error Checking/Messages

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
	//Printing error messages
	if(result != FMOD_OK)
		printf("FMOD Error: (%d) %s\n", result, FMOD_ErrorString(result));
	//Bool that certain functions use
	else
		success = TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// FMOD System Related Functions

/*************************************************************************/
/*!
	\brief
	Creates FMOD system (when game starts)
*/
/*************************************************************************/
void FMODInit(void)
{
	FMOD_RESULT result;

	//Create system and check if it succeed
	result = FMOD_System_Create(&FMsystem);
	FMODErrCheck(result);

	//Initialize system  with number of sounds permitted and check if successful
	result = FMOD_System_Init(FMsystem, MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);
	FMODErrCheck(result);

	//Success message
	if(success == TRUE)
	{
		printf("FMOD CREATED AND INITIALIZED\n");
		success = FALSE;
	}
}

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
	Updates sound system.
*/
/*************************************************************************/
void UpdateSoundSystem(void)
{
	//Updates system so sounds keep running as they should
	FMOD_System_Update(FMsystem);
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

	//Close and release system with error checking
	result = FMOD_System_Close(FMsystem);
	FMODErrCheck(result);
	result = FMOD_System_Release(FMsystem);
	FMODErrCheck(result);

	//Success message
	if(success == TRUE)
	{
		printf("FMOD CLOSED AND RELEASED\n");
		success = FALSE;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound Manager Related Functions

/*************************************************************************/
/*!
	\brief
	Allocates memory for the Sound List
*/
/*************************************************************************/

void ResetSoundList(void)
{
	int i;

	//Make some mem for dem sounds
	soundList  = (FoxSound *) CallocMyAlloc(MAX_SOUND_CHANNELS, sizeof(FoxSound));

	if(soundList)
	{
		for(i = 0; i < MAX_SOUND_CHANNELS; i++)
		{
			soundList[i].sndID = -1;
		}
		printf("SOUND LIST SETUP MORE SUCCESSFUL\n");
	}
	else
		printf("Sound List Setup Failed\n");

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

	//add a sound to the soundlist
	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if(soundList[i].sndID == 0 || soundList[i].sndID == -1)
		{
			return &soundList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Frees sounds from list and releases memory
*/
/*************************************************************************/
void FreeSoundList(void)
{
	int i;

	//free sounds that are in the list
	for(i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if(soundList[i].sndID)
			freeSound(&soundList[i]);
	}
	
	FreeMyAlloc(soundList);
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
	//free and releases and indiviudal sound
	if(soundObj && soundObj->sndID && soundObj->Sound)
	{
		soundObj->sndID = 0;
		ReleaseSound(soundObj->Sound);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Channel Group Manager Related Functions

/*************************************************************************/
/*!
	\brief
	Allocates memory for the ChannelGroup List
*/
/*************************************************************************/
void ResetChannelGroupList(void)
{
	int i;

	//Allocs space for the channel group list
	channelGroupList = (FoxChannels *) CallocMyAlloc(MAX_CHANNEL_GROUPS, sizeof(FoxChannels));
	--TotalMemoryAllocs;

	//there be only one
	if(channelGroupList)
	{
		for(i = 0; i < MAX_CHANNEL_GROUPS; i++)
		{
			channelGroupList[i].sndID = -1;
		}
		printf("CHANNEL LIST SETUP SUCCESSFUL\n");
	}
	else
		printf("Channel List Setup Failed\n");

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

	//Add channelGroup to dat list
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
	Frees channel groups from list and releases memory
*/
/*************************************************************************/
void FreeChannelGroupList(void)
{
	int i;

	//free the channel groups in the list
	for(i = 0; i < MAX_CHANNEL_GROUPS; i++)
	{
		if(channelGroupList[i].sndID)
			freeChannelGroups(&channelGroupList[i]);
	}
	
	FreeMyAlloc(channelGroupList);
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
	//free individual channel group and release it
	if(chanGrpObj && chanGrpObj->sndID)
	{
		chanGrpObj->sndID = 0;
		ReleaseChannelGroups(chanGrpObj);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound Creation/Releasing Related Functions

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

	//Initialize struct
	InitSoundStruct(snd, type);
	
	//Different way to create sound based on size
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
	Creates a sound from file for the pause menu

	\param Filename
	Name of the sound file

	\param snd
	A pointer to a sound object struct

	\param type
	Used to tell if object should be stored in memory or streamed
*/
/*************************************************************************/
void CreatePauseSound(FoxSound * snd, char *Filename, int type)
{
	FMOD_RESULT result;
	//Doesn't add a sound to the sound list must free individually

	//Initialize struct
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
	Initializes a sound struct when sound is created

	\param snd
	A pointer to a sound object struct

	\param type
	Label of what kind of sound this is.
*/
/*************************************************************************/
void InitSoundStruct(FoxSound *snd, int type)
{
	//Initialize starting variables for sound struct
	snd->Sound = 0;
	snd->Channel = 0;
	snd->Playing = FALSE;
	snd->Paused = FALSE;
	snd->Type = type;
	snd->sndID = 1;
	snd->hasPlayed = FALSE;
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

	//release a sound and do some error checking
	result = FMOD_Sound_Release(sound);
	FMODErrCheck(result);
	
	if(success == FALSE)
	{
		printf("SOUND RELEASE FAILED\n");
	}

	success = FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// PLaying Sound Related Functions

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
	
	//If there is actually a channel
	if(snd->Channel)
	{
		//Check if sound is playing already and set variable
		result = FMOD_Channel_IsPlaying(snd->Channel, &snd->Playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		    FMODErrCheck(result);
	}
	
	//If sound isn't already playing
	if(snd->Playing == FALSE)
	{
		//Play the sound but start paused so we can do some stuff first
		result = FMOD_System_PlaySound(FMsystem, FMOD_CHANNEL_FREE, snd->Sound, TRUE, &snd->Channel);
		FMODErrCheck(result);
		snd->Paused = TRUE;
	
		//Add sound to appropriate channel group so we can change the volume later
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

		//Unpause the sound now we be done
		TogglePauseSound(snd);
	}

	success = FALSE;
}

int FoxSoundCheckIsPlaying(FoxSound *snd)
{
	FMOD_RESULT result;
	
	if(!snd)
		return 0;

	//If there is a channel get and set the playing state
	if(snd->Channel)
	{
		result = FMOD_Channel_IsPlaying(snd->Channel, &snd->Playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		    FMODErrCheck(result);
	}
	
	//return the correct bool for if sound is playing
	if(snd->Playing)
	{
		snd->Playing = TRUE;
		return TRUE;
	}
	else
	{
		snd->Playing = FALSE;
		return FALSE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Creating/Releasing Channel Group Functions

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
	//add to the list
	FoxChannels * chnl = AddChannelGroups();

	//initialize struct members
	chnl->sndID = 1;
	chnl->EffectsPaused = FALSE;
	chnl->MusicPaused = FALSE;

	//Create two channel groups one for effects and one for background music
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

	//Release both channel groups with error checking
	result = FMOD_ChannelGroup_Release(chnl->Effects);
	FMODErrCheck(result);
	result = FMOD_ChannelGroup_Release(chnl->Music);
	FMODErrCheck(result);

	success = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Volume Related Functions

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

	//Sets the volume of an individual sound
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

	//Gets the volume of a specific sound
	result = FMOD_Channel_GetVolume(snd->Channel, &volume);
	FMODErrCheck(result);

	success = FALSE;

	return volume;
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

	//Sets the volume of a specific channel group (or all)
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

	//Get's the channel volume for a specific channel group
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

	//Converts and integer volume to a string to be used on text
	for(i = 0; i <= 4; i++)
	{
		//If volume is 100 just set it manually
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// Pausing Audio Functions

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

	//If the sound is unpaused pause it (with error checking)
	if(snd->Paused == FALSE)
	{
		result = FMOD_Channel_SetPaused(snd->Channel, TRUE);
		FMODErrCheck(result);
		snd->Paused = TRUE;
	}
	//If the sound is paused unpause it (with error checking)
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

	//Toggles Pause on entire channels (effects all sounds on this channel)
	if(ChnlType == EffectType)
	{
		//Checking for pause state and either pausing or unpausing it
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
		//Checking for pause state and either pausing or unpausing it
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
