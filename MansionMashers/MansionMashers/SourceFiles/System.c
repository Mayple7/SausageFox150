/*****************************************************************************/
/*
File:				System.c
Author:				Juli Gregg (j.gregg)
Creation Date:		Feb 2, 2014

Purpose:			System Functions

Functions:			
					- FoxSystemInitialize
					- FoxSystemExit
					- InitWindow
					- SaveSettings
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/
/*****************************************************************************/

#include <shlobj.h>
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../FMODHeaders/fmod.h"
#include "../HeaderFiles/Sound.h"

FMOD_RESULT result;
int winWidth, winHeight;
int maxWidth, maxHeight;

/*************************************************************************/
/*!
	\brief
	Function to initialize stuff needed for engine at start
*/
/*************************************************************************/
void FoxSystemInitialize(void)
{
	char Buffer[200];
	FILE *fp;
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, Buffer);
	
	strcpy(GameData, Buffer);
	strcat(GameData, "\\DigiPen\\MansionMashers\\GameData.cfg");
	strcpy(Settings, Buffer);
	strcat(Settings, "\\DigiPen\\MansionMashers\\Settings.cfg");
	fp = fopen(Settings, "rt");
	if(!fp)
	{
		fp = fopen(Settings, "wt");
		if(!fp)
		{
			strcpy(Settings, Buffer);
			strcat(Settings, "\\DigiPen\\MansionMashers");

			CreateDirectory(Settings, NULL);
			strcat(Settings, "\\Settings.cfg");

			fp = fopen(Settings, "wt");

			if(!fp)
			{
				strcpy(Settings, Buffer);
				strcat(Settings, "\\DigiPen");

				CreateDirectory(Settings, NULL);
				strcat(Settings, "\\MansionMashers");
				CreateDirectory(Settings, NULL);

				strcat(Settings, "\\Settings.cfg");

				fp = fopen(Settings, "wt");
			}

		}

		fprintf(fp, "SFX: 100\nBGM: 100\nCheats: 0\nLookAtMouse: 0");
		fclose(fp);
		fp = fopen(Settings, "rt");
	}

	if(fp)
	{
		int num = 0;
		num = fscanf(fp, "%*s %f %*s %f %*s %d %*s %d", &SFXVolume, &BGMVolume, &Cheats, &LookAtMouse);
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

	//Initilize sound stuff and FMOD
	FMODInit();
	ResetChannelGroupList();
	ChannelController = CreateChannelGroups();
	SetChannelGroupVolume(EffectType, SFXVolume);
	SetChannelGroupVolume(MusicType, BGMVolume);

	InitWindow();
}

/*************************************************************************/
/*!
	\brief
	Exits system (mostly calls sound system stuff)
*/
/*************************************************************************/
void FoxSystemExit(void)
{
	//Free Channel Groups(Effects & Volume) and then Quit Fmod
	FreeChannelGroupList();
	FMODQuit();
}

/*************************************************************************/
/*!
	\brief
	Sets up window for correct aspect ration
*/
/*************************************************************************/
void InitWindow(void)
{
	RECT rect;
	HWND winHandle = AESysGetWindowHandle();
	Matrix3 mapScale, mapTrans;
	double ratio;

	if(GetClientRect(winHandle, &rect))
	{
		//Set up a scalar matrix for drawing
		winWidth = rect.right - rect.left;
		winHeight = rect.bottom - rect.top;

		Matrix3Translate(&mapTrans, 0.0f, 0.0f);

		printf("Window Ratio: %f, %f\n", winWidth / 1920.0, winHeight / 1080.0);

		//Decide whether the width is shorter or the height is shorter
		if(winWidth / (double)winHeight >= 16.0f / 9.0f)
		{
			//Height too short
			ratio = winHeight / 1080.0;
			SetLoadRatio(ratio);
			Matrix3ScaleMatrix(&mapScale, (float)ratio, (float)ratio);
		}
		else
		{
			//Width too skinny
			ratio = winWidth / 1920.0;
			SetLoadRatio(ratio);
			Matrix3ScaleMatrix(&mapScale, (float)ratio, (float)ratio);
		}

		Matrix3Mult(&LoadRatioMtx, &mapScale, &mapTrans);
	}
}

/*************************************************************************/
/*!
	\brief
	Saves the settings to a file
*/
/*************************************************************************/
void SaveSettings(void)
{
	FILE *fp;
	
	fp = fopen(Settings, "wt");
	if(fp)
	{
		fprintf(fp, "SFX: %d\nBGM: %d\nCheats: %d\nLookAtMouse: %d", (int)(SFXVolume * 100), (int)(BGMVolume * 100), Cheats, LookAtMouse);
		fclose(fp);
	}
}
