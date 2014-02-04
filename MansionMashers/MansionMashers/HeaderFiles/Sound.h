#ifndef FOX_SOUND
#define FOX_SOUND

#include "../FMODHeaders/fmod.h"

#define MAX_SOUND_CHANNELS 100 

typedef enum SoundType
{
	SmallSnd,
	LargeSnd,
};

typedef struct FoxSound
{
	FMOD_SOUND *Sound;
	FMOD_CHANNEL *Channel;
	FMOD_BOOL Playing;
	FMOD_BOOL Paused;
} FoxSound;

FMOD_SYSTEM * GetSoundSystem(void);
void FMODErrCheck(FMOD_RESULT result);
void FMODInit(void);
void FMODQuit(void);
void CreateSound(char *Filename, FoxSound *snd, int type);
void PlayAudio(FoxSound *);
void ReleaseSound(FMOD_SOUND *sound);
void InitSoundStruct(FoxSound *snd);

#endif