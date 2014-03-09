/*****************************************************************************/
/*!
\file				MemoryManager.c
\author				Kaden Nugent (kaden.n)
\date				Jan 8, 2014

\brief				Manages memory allocation

\par				Functions:
\li					CallocMyAlloc
\li					MallocMyAlloc
\li					FreeMyAlloc
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Main

/*************************************************************************/
/*!
	\brief
	Keeps track of what memory is used. [CALLOC]
*/
/*************************************************************************/
void *__cdecl CallocMyAlloc(int count, int size)
{
	//Print it out for all those viewers at home
	++TotalMemoryAllocs;
	//printf("CALLOC:  Bytes: %i  Total Allocs: %i\n", count * size, TotalMemoryAllocs);

	//Give them what they really came for
	return calloc(count, size);
}

/*************************************************************************/
/*!
	\brief
	Keeps track of what memory is used. [MALLOC]
*/
/*************************************************************************/
void *__cdecl MallocMyAlloc(int count, int size)
{
	//Print it out for all those viewers at home
	++TotalMemoryAllocs;
	//printf("MALLOC:  Bytes: %i  Total Allocs: %i\n", count * size, TotalMemoryAllocs);

	//Give them what they really came for
	return malloc(count * size);
}

/*************************************************************************/
/*!
	\brief
	Sets memory to all the one value that you desire. [MEMSET]
	I'm actually not sure why I made this one, I just like to make
	functions that already exsist I guess.
*/
/*************************************************************************/
void *__cdecl MemsetMyAlloc(int count, int size, int value, void *list)
{
	//Print out something
	//printf("Congratz for using this function.\n");

	//Set the memory given to the desired value
	return memset(list, value, count * size);
}

/*************************************************************************/
/*!
	\brief
	Keeps track of what memory is used. [FREE]
*/
/*************************************************************************/
void __cdecl FreeMyAlloc(void *memory)
{
	//Print it out for all those viewers at home
	++TotalMemoryFrees;
	//printf("FREEING DATA:  Total Frees: %i\n", TotalMemoryFrees);

	//Let my people go!
	free(memory);
}
