#ifndef FOX_CAMERA
#define FOX_CAMERA

#include "../fmod.h"

#define MAX_SOUND_CHANNELS 100 

void FMODErrCheck(FMOD_RESULT result);
void FMODInit(FMOD_SYSTEM *system);
void FMODQuit(FMOD_SYSTEM *system);

#endif