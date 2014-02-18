/*
File:				System.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Feb 2, 2014

Purpose:			System Functions

Functions:			
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../FMODHeaders/fmod.h"
#include "../HeaderFiles/Sound.h"

FMOD_RESULT result;
int winWidth, winHeight;
int maxWidth, maxHeight;

void FoxSystemInitialize(void)
{
	FILE *fp;
	fp = fopen("../VolumeSettings.cfg", "rt");
	if(!fp)
	{
		fp = fopen("../VolumeSettings.cfg", "wt");
		fprintf(fp, "SFX: 100\nBGM: 100");
		fclose(fp);
		fp = fopen("../VolumeSettings.cfg", "rt");
	}

	if(fp)
	{
		int num = 0;
		num = fscanf(fp, "%*s %f %*s %f", &SFXVolume, &BGMVolume);
		fclose(fp);
	}
	if(SFXVolume > 100)
		SFXVolume = 100.0f;
	else if(SFXVolume < 0)
		SFXVolume = 0.0f;
	
	if(BGMVolume > 100)
		BGMVolume = 100.0f;
	else if(BGMVolume < 0)
		BGMVolume = 0.0f;


	SFXVolume /= 100.0f;
	BGMVolume /= 100.0f;

	FMODInit();
	InitWindow();
}

void FoxSystemExit(void)
{
	
	FMODQuit();

}

void InitWindow(void)
{
	RECT rect;
	HWND winHandle = AESysGetWindowHandle();
	double ratio;

	if(GetClientRect(winHandle, &rect))
	{
		winWidth = rect.right - rect.left;
		winHeight = rect.bottom - rect.top;

		if(winWidth / (double)winHeight >= 16.0f / 9.0f)
		{
			ratio = winHeight / 1080.0;
			SetLoadRatio(ratio);
			winWidth = (int)(1920 * ratio);
			winHeight = (int)(1080 * ratio);
		}
		else
		{
			ratio = winWidth / 1920.0;
			SetLoadRatio(ratio);
			winWidth = (int)(1920 * ratio);
			winHeight = (int)(1080 * ratio);
		}
	}
}