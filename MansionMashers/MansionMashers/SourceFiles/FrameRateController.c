/*
/*****************************************************************************/
/*!
\file				FrameRateController.c
\author				Juli Gregg (j.gregg)
\date				Feb 3, 2014

\brief				Functions for Sound/FMOD

\par				Functions:
\li					StartFoxFrame
\li					EndFoxFrame
\li					GetDeltaTime


\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/ 

#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"

static unsigned long CycleStart, CycleEnd; 		//for framerate controller
static unsigned long DeltaTime;

/*************************************************************************/
/*!
	\brief
	Get Current time
*/
/*************************************************************************/
void StartFoxFrame(void)
{
	CycleStart = timeGetTime();
}

/*************************************************************************/
/*!
	\brief
	Gets time and keeps game from going to fast

*/
/*************************************************************************/
void EndFoxFrame(void)
{
	CycleEnd = timeGetTime();
	DeltaTime = CycleEnd - CycleStart;
	while(DeltaTime < 1000.0f/FRAMERATE)
	{
		CycleEnd = timeGetTime();
		DeltaTime = CycleEnd - CycleStart;
	}
	//printf("%d\n", DeltaTime);
}

/*************************************************************************/
/*!
	\brief
	Converts deltatime to seconds

	\return
	Returns deltatime in seconds

*/
/*************************************************************************/
float GetDeltaTime(void)
{
	return DeltaTime / 1000.0f;	
}
