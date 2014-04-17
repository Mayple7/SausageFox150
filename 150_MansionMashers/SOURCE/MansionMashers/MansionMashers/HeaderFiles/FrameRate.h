/*****************************************************************************/
/*!
\file				FrameRate.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_FRAMERATE
#define FOX_FRAMERATE

#define FRAMERATE 60

void StartFoxFrame(void);
void EndFoxFrame(void);
float GetDeltaTime(void);

float animationTest; //To test clunky animation

#endif