#ifndef FOX_SOUND
#define FOX_SOUND

#include "../FMODHeaders/fmod.h"

#define MAX_SOUND_CHANNELS 100 

FMOD_SYSTEM * GetSoundSystem(void);
void FMODErrCheck(FMOD_RESULT result);
void FMODInit(void);
void FMODQuit(void);
void CreateSound(char *Filename, FMOD_SOUND **sound);
void PlayAudio(FMOD_SOUND *sound, FMOD_CHANNEL **channel);
void ReleaseSound(FMOD_SOUND *sound);

#endif