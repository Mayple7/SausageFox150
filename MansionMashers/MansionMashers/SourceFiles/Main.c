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
#include "../HeaderFiles/StartSlides.h"
#include "../HeaderFiles/MainMenu.h"
#include "../HeaderFiles/TestLevel.h"
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../FMODHeaders/fmod.h"
#include "../HeaderFiles/ObjectManager.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals
int GameRunning;
double winWidth;
double winHeight;

// ---------------------------------------------------------------------------
// Static function protoypes
LRESULT CALLBACK MyWinCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp);

// ---------------------------------------------------------------------------
// main

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Variable declaration	
	int Previous;										//Local State Variables
	int Current;										//Local State Variables
	int Next;											//Local State Variables
	FMOD_SYSTEM *SoundSystem = 0;						//Local Sound System
	LARGE_INTEGER CycleStart, CycleEnd, Frequency; 		//for framerate controller
	double DeltaTime = 0;
	int FrameRate = 60;									//Make a define in the future
	int debugConsole = 1;
	//FMOD_SYSTEM *FMsystem = 0;
	AESysInitInfo sysInitInfo;
	WNDCLASS	winClass;
	HWND winHandle;
	RECT rect;

	//Creates the console window
	if(debugConsole && AllocConsole())
	{
		FILE* file;
 
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Alpha Engine - Console");
	}

	// Initialize the system 
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 1920;
	sysInitInfo.mWinHeight			= 1080;
	sysInitInfo.mCreateConsole		= 0;
	sysInitInfo.mMaxFrameRate		= 0;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
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

	ShowWindow	(winHandle, SW_SHOWMAXIMIZED);
	UpdateWindow(winHandle);

	winWidth = rect.right - rect.left;
	winHeight = rect.bottom - rect.top;

	sysInitInfo.mCreateWindow		= 0;
	sysInitInfo.mWindowHandle		= winHandle;

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
	FoxSystemInitialize();
	SoundSystem = GetSoundSystem();
	GSMInitialize(GS_MainMenu);

	while(GameRunning)
	{
		//Setting local states
		Previous = GetCurrentState();
		Current = GetCurrentState();
		Next = GetNextState();

		//Checking if wanting to quit
		if(Current == GS_Quit)
		{
			FoxSystemExit();
			AESysExit();
			return 0;
		}
		//Checking if wanting to restart
		else if(Current == GS_Restart)
		{
			SetCurrentState(Previous);
			SetNextState(Previous);
			Current = Previous;
			Next = Previous;
		}
		else
		{
			GSMUpdate(Current);
			GSMPointers.pLoad();
		}

		GSMPointers.pInit();
		
		while(Current == Next)
		{
			AESysFrameStart();
			
			//FrameRate Controller
			QueryPerformanceCounter(&CycleStart);			

			AEInputUpdate();
			GSMPointers.pUpdate();
			FMOD_System_Update(SoundSystem);
			GSMPointers.pDraw();
			Next = GetNextState();

			//FrameRate Controller
			QueryPerformanceCounter(&CycleEnd);				
			QueryPerformanceFrequency(&Frequency);			
			DeltaTime = ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Frequency.QuadPart);
			
			//Framerate controller
			while(DeltaTime < (1.0/FrameRate))
			{
				QueryPerformanceCounter(&CycleEnd);
				DeltaTime = ((double)(CycleEnd.QuadPart - CycleStart.QuadPart) / (double)Frequency.QuadPart);
			}
			//printf("%f\n", DeltaTime);

			AESysFrameEnd();
		}

		GSMPointers.pFree();

		if(Next != GS_Restart)
			GSMPointers.pUnload();
		
		SetPreviousState(Current);
		SetCurrentState(Next);

	}
	//End of GSM

	return 1;
}

LRESULT CALLBACK MyWinCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp) 
{
	HDC dc;   
	PAINTSTRUCT ps;
	RECT rect;

	switch (msg)
	{
/*	case WM_SIZE:
		if(GetClientRect(AESysGetWindowHandle(), &rect))
		{
			double ratio;
			double width = rect.right - rect.left;
			double height = rect.bottom - rect.top;
			if(width / height >= 16.0f / 9.0f)
			{
				ratio = height / winHeight;
				winWidth = winWidth * ratio;
				winHeight = winHeight * ratio;
			}
			else
			{
				ratio = width / winWidth;
				winWidth = winWidth * ratio;
				winHeight = winHeight * ratio;
			}
			ResizeObjects(ratio);
		}
		break;*/
	// when the window is created
	case WM_CREATE:
		printf("Hi Juli, I'm still here!\n");
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
