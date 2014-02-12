#ifndef FOX_SOUND
#define FOX_SOUND

#include "../FMODHeaders/fmod.h"

#define MAX_SOUND_CHANNELS 40
#define MAX_CHANNEL_GROUPS 2

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

typedef struct FoxSound
{
	FMOD_SOUND *Sound;
	FMOD_CHANNEL *Channel;
	FMOD_BOOL Playing;
	FMOD_BOOL Paused;
	int Type;
	int sndId;
} FoxSound;

typedef struct FoxChannels
{

	FMOD_CHANNELGROUP * Effects;
	FMOD_BOOL EffectsPaused;
	FMOD_CHANNELGROUP * Music;
	FMOD_BOOL MusicPaused;
	int sndID;
} FoxChannels;

FoxSound * soundList;
FoxChannels * channelGroupList;

FMOD_SYSTEM * GetSoundSystem(void);
void FMODErrCheck(FMOD_RESULT result);
void FMODInit(void);
void FMODQuit(void);
void CreateSound(char *Filename, FoxSound *SoundStruct, int Soundtype);
void PlayAudio(FoxSound *SoundStruct, FoxChannels *ChannelGroups);
void ReleaseSound(FMOD_SOUND *Sound);
void InitSoundStruct(FoxSound *SoundStruct, int Soundtype); 
void TogglePauseSound(FoxSound * SoundStruct);
void TogglePauseChannel(FoxChannels * ChannelGroups, int ChannelType);
void CreateChannelGroups(FoxChannels *chnl);
void ReleaseChannelGroups(FoxChannels *chnl);
void SetChannelGroupVolume(FoxChannels *chnl, int type, float volume);
void SetSoundVolume(FoxSound *snd, float volume);
float GetSoundVolume(FoxSound *snd);
float GetChannelGroupVolume(FoxChannels * chnl, int type);
void UpdateSoundSystem(void);
char * VolumetoString(char *string, float volume);

FoxSound * AddSound(void);
void resetSoundList(void);
void freeSound(FoxSound * soundObj);
void freeChannelGroups(FoxChannels * chanGrpObj);
void freeSoundList(void);

#endif