#ifndef FOX_SOUND
#define FOX_SOUND

#include "../FMODHeaders/fmod.h"

#define MAX_SOUND_CHANNELS 100 

typedef enum SoundType
{
	SmallSnd,
	LargeSnd,
};

typedef enum ChannelType
{
	EffectType,
	MusicType,
};

typedef struct FoxSound
{
	FMOD_SOUND *Sound;
	FMOD_CHANNEL *Channel;
	FMOD_BOOL Playing;
	FMOD_BOOL Paused;
	int Type;
} FoxSound;

typedef struct FoxChannels
{

	FMOD_CHANNELGROUP * Effects;
	FMOD_BOOL EffectsPaused;
	FMOD_CHANNELGROUP * Music;
	FMOD_BOOL MusicPaused;

} FoxChannels;

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

#endif