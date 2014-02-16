#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

typedef enum MouseButton
{
  MOUSE_BUTTON_LEFT,
  MOUSE_BUTTON_MIDDLE,
  MOUSE_BUTTON_RIGHT,

  MOUSE_BUTTON_COUNT
} MouseButton;

extern int FoxInput_MouseDown(MouseButton button);

extern int FoxInput_MouseTriggered(MouseButton button);

extern int FoxInput_MouseUp(MouseButton button);

extern void FoxInput_GetMousePosition(int *mouseX, int *mouseY);

void FoxInput_GetWorldPosition(int *mx, int *my);

extern int FoxInput_MouseWithinWindow(void);

extern int FoxInput_KeyDown(unsigned char key);

extern int FoxInput_KeyTriggered(unsigned char key);

extern int FoxInput_KeyUp(unsigned char key);

extern void FoxInput_PassEvent(unsigned int messageType, int param);

extern void FoxInput_Update(void);

#endif