/*
/*****************************************************************************/
/*!
\file				FrameRateController.c
\author				Juli Gregg (j.gregg)
\date				Feb 3, 2014

\brief				Functions for the framerate controller

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

static LARGE_INTEGER CycleStart, CycleEnd, Freq; 		//for framerate controller
static double DeltaTime;

/*************************************************************************/
/*!
	\brief
	Getting CPU Cycles
*/
/*************************************************************************/
void StartFoxFrame(void)
{
	DEVMODE *freq = (DEVMODE *) malloc(sizeof(DEVMODE));
	freq->dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, freq);

	//Start the first frame
	QueryPerformanceCounter(&CycleStart);
}

double testFrameTime(void)
{
	//For testing
	QueryPerformanceCounter(&CycleEnd);
	QueryPerformanceFrequency(&Freq);
	return ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Freq.QuadPart);
}

/*************************************************************************/
/*!
	\brief
	Gets CPU Cycles & Frequency and calculates DeltaTime in order
	to keep game from going to fast.
*/
/*************************************************************************/
void EndFoxFrame(void)
{
	//Watch over framerate
	QueryPerformanceCounter(&CycleEnd);
	QueryPerformanceFrequency(&Freq);
	DeltaTime = ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Freq.QuadPart);
	
	while(DeltaTime < animationTest / FRAMERATE) //animationTest is NOT PERMANENT
	{
		QueryPerformanceCounter(&CycleEnd);
		DeltaTime = ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Freq.QuadPart);
	}
}

/*************************************************************************/
/*!
	\brief
	DeltaTime (60fps = .016667)

	\return
	Returns DeltaTime
*/
/*************************************************************************/
float GetDeltaTime(void)
{
	return (float)DeltaTime;	
}
