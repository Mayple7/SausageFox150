#ifndef FOX_SOUND
#define FOX_SOUND

//Includes
#include "../FMODHeaders/fmod.h"

//Defines
#define MAX_SOUND_CHANNELS 150
#define MAX_CHANNEL_GROUPS 1

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
	int hasPlayed;
} FoxSound;

typedef struct FoxChannels
{
	FMOD_CHANNELGROUP *Effects;
	FMOD_BOOL EffectsPaused;
	FMOD_CHANNELGROUP *Music;
	FMOD_BOOL MusicPaused;
	int sndID;
} FoxChannels;

//Global Sound Type Variables
FoxSound *soundList;
FoxChannels *channelGroupList;
FoxChannels *ChannelController;

//Functions
void FMODErrCheck(FMOD_RESULT result);
void FMODInit(void);
FMOD_SYSTEM * GetSoundSystem(void);
void UpdateSoundSystem(void);
void FMODQuit(void);

void ResetSoundList(void);
FoxSound * AddSound(void);
void FreeSoundList(void);
void freeSound(FoxSound * soundObj);

void ResetChannelGroupList(void);
FoxChannels * AddChannelGroups(void);
void FreeChannelGroupList(void);
void freeChannelGroups(FoxChannels * chanGrpObj);

FoxSound *CreateSound(char *Filename, int type);
void CreatePauseSound(FoxSound * snd, char *Filename, int type);
void InitSoundStruct(FoxSound *SoundStruct, int Soundtype); 
void ReleaseSound(FMOD_SOUND *Sound);

void PlayAudio(FoxSound *snd);
int FoxSoundCheckIsPlaying(FoxSound *snd);

FoxChannels* CreateChannelGroups(void);
void ReleaseChannelGroups(FoxChannels *chnl);

void SetSoundVolume(FoxSound *snd, float volume);
float GetSoundVolume(FoxSound *snd);
void SetChannelGroupVolume(int type, float volume);
float GetChannelGroupVolume(int type);
char * VolumetoString(char *string, float volume);

void TogglePauseSound(FoxSound * SoundStruct);
void TogglePauseChannel(int ChnlType);

#endif