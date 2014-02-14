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
	if(GetWindowRect(winHandle, &rect))
	{
		maxWidth = GetSystemMetrics(SM_CXSCREEN);
		maxHeight = GetSystemMetrics(SM_CYSCREEN);
		printf("%i : %i\n", maxWidth, maxHeight);
	}
}

void UpdateWindowSize(void)
{
	RECT rect;
	HWND winHandle = AESysGetWindowHandle();
	double ratio, updateRatio;
	int width, height;

	if(GetClientRect(winHandle, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		if(!width || !height)
			return;
		if(width / (double)height >= 16.0f / 9.0f)
		{
			if(width > winWidth && height > winHeight)
			{
				ratio = height / winHeight;
				if(ratio > width / winWidth)
				{
					ratio = height / 1080.0;
					SetLoadRatio(ratio);
					updateRatio = height / (double)winHeight;
					winWidth = width;
					winHeight = height;
				}
				else
				{
					ratio = width / 1920.0;
					SetLoadRatio(ratio);
					updateRatio = width / (double)winWidth;
					winWidth = width;
					winHeight = height;
				}

			}
			else
			{
				ratio = height / 1080.0;
				SetLoadRatio(ratio);
				updateRatio = height / (double)winHeight;
				winWidth = width;
				winHeight = height;
			}
		}
		else
		{
			ratio = width / 1920.0;
			SetLoadRatio(ratio);
			updateRatio = width / (double)winWidth;
			winWidth = width;
			winHeight = height;
		}
		if(winWidth > maxWidth)
		{
			ratio = maxWidth / 1920.0;
			SetLoadRatio(ratio);
			updateRatio = maxWidth / (double)winWidth;
			winWidth = maxWidth;
			winHeight = maxHeight;
		}
		if(winHeight > maxHeight)
		{
			ratio = maxHeight / 1080.0;
			SetLoadRatio(ratio);
			updateRatio = maxHeight / (double)winHeight;
			winWidth = maxWidth;
			winHeight = maxHeight;
		}
		ResizeObjects((float)updateRatio);
	}
}

