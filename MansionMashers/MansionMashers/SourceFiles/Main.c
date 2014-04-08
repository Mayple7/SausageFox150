/*****************************************************************************/
/*!
\file				Main.c
\author				Dan Muller (d.muller)
\date				Jan 7, 2014

\brief				The main function for the game and holds the game loop

\par				Functions:
\li					SetCamera
\li					ResetCamera
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
//#include <windows.h>
//#include <vld.h>
#include "../HeaderFiles/StartSlides.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/TestLevel.h"
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../FMODHeaders/fmod.h"
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/Input.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int GameRunning;
int winWidth;
int winHeight;
double loadRatio;

// ---------------------------------------------------------------------------
// Static function protoypes
LRESULT CALLBACK MyWinCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp);

// ---------------------------------------------------------------------------
// main

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Variable declaration		
	AESysInitInfo sysInitInfo;
	WNDCLASS	winClass;
	HWND winHandle;
	RECT rect;

	srand(timeGetTime());								// Sets the random number gen seed

#if defined _DEBUG
	//Creates the console window
	if(AllocConsole())
	{
		FILE* file;
 
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Alpha Engine - Console");
	}
#endif

	// Initialize the system 
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 1920;
	sysInitInfo.mWinHeight			= 1080;
	sysInitInfo.mCreateConsole		= 0;
	sysInitInfo.mMaxFrameRate		= 0;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;
//#ifndef _DEBUG
	//sysInitInfo.mWindowStyle		= WS_POPUP & ~WS_THICKFRAME;
//#else
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
//#endif
	AESysInit (&sysInitInfo);


	winClass.style			= sysInitInfo.mClassStyle;
	winClass.lpfnWndProc	= MyWinCallBack;//(pSysInitInfo->mpWinCallBack) ? pSysInitInfo->mpWinCallBack : winCallBack;
	winClass.cbClsExtra		= 0;
	winClass.cbWndExtra		= 0;
	winClass.hInstance		= sysInitInfo.mAppInstance;
	winClass.hIcon			= LoadIcon(instanceH,MAKEINTRESOURCE(101));
	winClass.hCursor		= LoadCursor(NULL,IDC_ARROW);
	winClass.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winClass.lpszMenuName	= NULL;
	winClass.lpszClassName	= "Window Class Name";

	RegisterClass(&winClass);

	rect.left = 0;
	rect.top = 0;
	rect.right = sysInitInfo.mWinWidth;//WinWidth;
	rect.bottom = sysInitInfo.mWinHeight;//WinHeight;

	AdjustWindowRect(&rect, sysInitInfo.mWindowStyle, 0);

	winHandle = CreateWindow(winClass.lpszClassName, "Mansion Mashers", sysInitInfo.mWindowStyle, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, sysInitInfo.mAppInstance, NULL);

	ShowWindow(winHandle, SW_SHOWMAXIMIZED);
	
	UpdateWindow(winHandle);
	
	sysInitInfo.mCreateWindow = 0;
	sysInitInfo.mWindowHandle = winHandle;

	if(0 == AESysInit (&sysInitInfo))
	{
		GameRunning = 0;
		printf("System Init Failed!\n");
	}
	else
		GameRunning = 1;
	
	// reset the system modules
	AESysReset();

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//GSM Start
	animationTest = 1.0f; //For testing
	FoxSystemInitialize();
#ifndef _DEBUG
	GSMInitialize(GS_SplashScreen);
#else
	GSMInitialize(GS_MainMenu);
#endif

	while(GameRunning)
	{
		//Memory start
		TotalMemoryAllocs = 0;
		TotalMemoryFrees  = 0;

		//Checking if wanting to quit
		if(GetCurrentState() == GS_Quit)
		{
			FoxSystemExit();
			AESysExit();
			return 0;
		}
		//Checking if wanting to restart
		else if(GetCurrentState() == GS_Restart)
		{
			SetCurrentState(GetPreviousState());
			SetNextState(GetPreviousState());
		}
		else
		{
			GSMUpdate(GetCurrentState());
			GSMPointers.pLoad();
		}

		GSMPointers.pInit();

#ifndef _DEBUG
		RemoveDebugMode();
#endif

		ObjectID = 0; //Start the object IDs
		
		while(GetCurrentState() == GetNextState())
		{
			AESysFrameStart();
			StartFoxFrame();						

			AEInputUpdate();
			

			GSMPointers.pUpdate();
			UpdateSoundSystem();
			GSMPointers.pDraw();
#ifdef _DEBUG
			if(FoxInput_KeyTriggered('R'))
			{
				int worldX, worldY;
				FoxInput_GetWorldPosition(&worldX, &worldY);
				printf("%f, %f\n", (worldX) / GetLoadRatio(), (worldY) / GetLoadRatio());
			}

#endif

			FoxInput_Update();

			EndFoxFrame();
			AESysFrameEnd();
		}

		GSMPointers.pFree();

		if(GetNextState() != GS_Restart)
			GSMPointers.pUnload();

		/*
			No algorythm here, just checking how many times the 
			functions are called for freeing and allocing.
		*/
		printf("\nMEMORY NOT FREED: %i \n\n", TotalMemoryAllocs - TotalMemoryFrees);
		if (!(TotalMemoryAllocs - TotalMemoryFrees))
			AE_ASSERT_MESG(":: MEMORY LEAKS: PLEASE GET A BUCKET AND MOP ::");
		printf("----------------A NEW DAY----------------\n\n");
		
		SetPreviousState(GetCurrentState());
		SetCurrentState(GetNextState());
	}
	//End of GSM

	return 1;
}

LRESULT CALLBACK MyWinCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp) 
{
	HDC dc;   
	PAINTSTRUCT ps;

	FoxInput_PassEvent((unsigned int) msg, (int) wp);

	if(msg == WM_SYSCOMMAND && wp == SC_MINIMIZE)
	{
		ShowWindow(hWin, SW_SHOWMINIMIZED);
		return 0;
	}
	else if(msg == WM_SYSCOMMAND && wp == SC_MAXIMIZE)
	{
		ShowWindow(hWin, SW_SHOWMAXIMIZED);
		return 0;
	}
	else if(msg == WM_SYSCOMMAND && wp == SC_RESTORE)
	{
		ShowWindow(hWin, SW_SHOWNORMAL);
		return 0;
	}

	switch (msg)
	{
	case WM_SIZE:
		break;
	// when the window is created
	case WM_CREATE:
		printf("The Windows of all Windows has been created.\n");
		break;

	// when the rectangle is drawn
	case WM_PAINT:
		dc = BeginPaint(hWin, &ps);

		// Cleans up the painting process
		EndPaint(hWin, &ps);
		break;

	// When it's time for the window to go away
	case WM_DESTROY:
		SetNextState(GS_Quit);
		break;

	// called any time the window is moved
	case WM_MOVE:
		// Invalidate the rect to force a redraw
		InvalidateRect(hWin, NULL, FALSE);
		break;

	case WM_ACTIVATE:
		// DO NOT REMOVE THIS
		// *(AESysGetAppActive()) = (LOWORD(wp) == WA_INACTIVE) ? 0 : 1;
		break;

	default:
		return DefWindowProc(hWin, msg, wp, lp);
	}

	return 0;
}

float GetLoadRatio(void)
{
	return (float)loadRatio;
}

void SetLoadRatio(double ratio)
{
	loadRatio = ratio;
}

int GetWindowWidth(void)
{
	//printf("%i\n", winWidth);
	return winWidth;
}

int GetWindowHeight(void)
{
	return winHeight;
}
