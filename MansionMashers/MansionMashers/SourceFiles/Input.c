#include "../HeaderFiles/FoxEngine.h"
#include <Windows.h>
#include <string.h>

#define UP 0
#define DOWN 1

#define YES 1
#define NO 0


static char buttonStates[MOUSE_BUTTON_COUNT] = { 0 };
static char buttonTriggeredStates[MOUSE_BUTTON_COUNT] = { 0 };

static char keyStates[0xff] = { 0 };
static char keyTriggeredStates[0xff] = { 0 };

static int mouseX = -1, mouseY = -1;
static char mouseWithinWindow = 0;


void FoxInput_PassEvent(unsigned int messageType, int param)
{
  switch (messageType)
  {
  case WM_LBUTTONDOWN:
    buttonStates[MOUSE_BUTTON_LEFT] = buttonTriggeredStates[MOUSE_BUTTON_LEFT] = DOWN; 
    break;
  case WM_LBUTTONUP:
    buttonStates[MOUSE_BUTTON_LEFT] = UP; 
    break;
  case WM_MBUTTONDOWN:
    buttonStates[MOUSE_BUTTON_MIDDLE] = buttonTriggeredStates[MOUSE_BUTTON_MIDDLE] = DOWN; 
    break;
  case WM_MBUTTONUP:
    buttonStates[MOUSE_BUTTON_MIDDLE] = UP; 
    break;
  case WM_RBUTTONDOWN:
    buttonStates[MOUSE_BUTTON_RIGHT] = buttonTriggeredStates[MOUSE_BUTTON_RIGHT] = DOWN;
    break;
  case WM_RBUTTONUP:
    buttonStates[MOUSE_BUTTON_RIGHT] = UP;
    break;
  case WM_KEYDOWN:
    if (param >= 0 && param < 0xff)
      keyStates[param] = DOWN; 
    break;
  case WM_KEYUP:
    if (param >= 0 && param < 0xff)
      keyStates[param] = UP; 
    break;
  }
}

void FoxInput_Update(void)
{
  POINT point;
  RECT rcClient;
  HWND alphaWindowHandle;

  memset(buttonTriggeredStates, UP, sizeof(buttonTriggeredStates));
  memcpy(keyTriggeredStates, keyStates, sizeof(keyStates));

  alphaWindowHandle = AESysGetWindowHandle();

  if (alphaWindowHandle != NULL
    && GetCursorPos(&point)
    && ScreenToClient(alphaWindowHandle, &point)
    && GetClientRect(alphaWindowHandle, &rcClient))
  {

    if (point.x >= rcClient.left && point.x <= rcClient.right
      && point.y >= rcClient.top && point.y <= rcClient.bottom)
    {
      int mx = (int) point.x;

      int my = (int) ((rcClient.bottom - rcClient.top) - point.y - 1);
      mouseX = mx;
      mouseY = my;
      mouseWithinWindow = YES;
    }
    else
    {

      mouseWithinWindow = NO;
    }
  }
}



int FoxInput_MouseDown(MouseButton button)
{
  return buttonStates[button] == DOWN;
}

int FoxInput_MouseTriggered(MouseButton button)
{
  return buttonTriggeredStates[button] == DOWN;
}

int FoxInput_MouseUp(MouseButton button)
{
  return buttonStates[button] == UP;
}

void FoxInput_GetMousePosition(int *mx, int *my)
{
  if (mx != NULL)
    *mx = mouseX;
  if (my != NULL)
    *my = mouseY;
}

int FoxInput_MouseWithinWindow(void)
{
  return mouseWithinWindow;
}

int FoxInput_KeyDown(unsigned char key)
{
  return keyStates[key] == DOWN;
}

int FoxInput_KeyTriggered(unsigned char key)
{
  return (keyStates[key] == DOWN && keyTriggeredStates[key] == UP);
}

int FoxInput_KeyUp(unsigned char key)
{
  return keyStates[key] == UP;
}

