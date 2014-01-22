#ifndef FOX_GSM
#define FOX_GSM

typedef struct GSM_Pointers
{
	void (*pLoad)(void);
	void (*pInit)(void);
	void (*pUpdate)(void);
	void (*pDraw)(void);
	void (*pFree)(void);
	void (*pUnload)(void);
}GSM_Pointers;

typedef struct GameState
{
	int Previous;
	int Current;
	int Next;
}GameState;

struct GSM_Pointers GSMPointers;

void GSMInitialize(int InitState);
int GetPreviousState(void);
int GetCurrentState(void);
int GetNextState(void);
void SetPreviousState(int PreviousState);
void SetCurrentState(int CurrentState);
void SetNextState(int NextState);
void GSMUpdate(int CurState);

#endif