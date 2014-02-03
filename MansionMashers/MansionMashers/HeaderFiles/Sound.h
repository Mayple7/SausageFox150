#ifndef FOX_SOUND
#define FOX_SOUND

#include "../FMODHeaders/fmod.h"

#define MAX_SOUND_CHANNELS 100 

typedef struct FoxSound
{
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;
} FoxSound;

FMOD_SYSTEM * GetSoundSystem(void);
void FMODErrCheck(FMOD_RESULT result);
void FMODInit(void);
void FMODQuit(void);
void CreateSound(char *Filename, FMOD_SOUND **sound);
void PlayAudio(FoxSound *);
void ReleaseSound(FMOD_SOUND *sound);
void InitSoundStruct(FoxSound *snd);

#endif