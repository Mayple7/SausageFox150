#ifndef FOX_SOUND
#define FOX_SOUND

//Includes
#include "../FMODHeaders/fmod.h"

//Defines
#define MAX_SOUND_CHANNELS 40
#define MAX_CHANNEL_GROUPS 2

//Global Basic Type Variables
float SFXVolume, BGMVolume;

//Global Enums
typedef enum SoundType
{
	SmallSnd,
	LargeSnd,
};

typedef enum ChannelType
{
	EffectType,
	MusicType,
	AllTypes,
};

//Global Structs
typedef struct FoxSound
{
	FMOD_SOUND *Sound;
	FMOD_CHANNEL *Channel;
	FMOD_BOOL Playing;
	FMOD_BOOL Paused;
	int Type;
	int sndID;
} FoxSound;

typedef struct FoxChannels
{

	FMOD_CHANNELGROUP * Effects;
	FMOD_BOOL EffectsPaused;
	FMOD_CHANNELGROUP * Music;
	FMOD_BOOL MusicPaused;
	int sndID;
} FoxChannels;

//Global Sound Type Variables
FoxSound * soundList;
FoxChannels * channelGroupList;
FoxChannels * ChannelController;

//Functions
FMOD_SYSTEM * GetSoundSystem(void);
void FMODErrCheck(FMOD_RESULT result);
void FMODInit(void);
void FMODQuit(void);

FoxSound* CreateSound(char *Filename, int type);
//void CreateSound(char *Filename, FoxSound *SoundStruct, int Soundtype);

void PlayAudio(FoxSound *snd);
//void PlayAudio(FoxSound *SoundStruct, FoxChannels *ChannelGroups);

void ReleaseSound(FMOD_SOUND *Sound);
void InitSoundStruct(FoxSound *SoundStruct, int Soundtype); 
void TogglePauseSound(FoxSound * SoundStruct);

void TogglePauseChannel(int ChnlType);
//void TogglePauseChannel(FoxChannels * ChannelGroups, int ChannelType);

//void CreateChannelGroups(FoxChannels *chnl);
FoxChannels* CreateChannelGroups(void);

void ReleaseChannelGroups(FoxChannels *chnl);

void SetChannelGroupVolume(int type, float volume);
//void SetChannelGroupVolume(FoxChannels *chnl, int type, float volume);

void SetSoundVolume(FoxSound *snd, float volume);
float GetSoundVolume(FoxSound *snd);

float GetChannelGroupVolume(int type);
//float GetChannelGroupVolume(FoxChannels * chnl, int type);

void UpdateSoundSystem(void);
char * VolumetoString(char *string, float volume);

FoxSound * AddSound(void);
FoxChannels * AddChannelGroups(void);

void ResetSoundList(void);
void freeSound(FoxSound * soundObj);
void freeChannelGroups(FoxChannels * chanGrpObj);
void FreeSoundList(void);

void ResetChannelGroupList(void);
void FreeChannelGroupList(void);

void CreatePauseSound(FoxSound * snd, char *Filename, int type);

#endif