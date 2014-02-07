/*****************************************************************************/
/*!
\file				MemoryManager.c
\author				Kaden Nugent (kaden.n)
\date				Jan 8, 2014

\brief				Manages memory allocation

\par				Functions:
\li					CallocMyAlloc
\li					MallocMyAlloc
  
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
	printf("CALLOC:  Bytes: %i  Total Allocs: %i\n", count * size, ++TotalMemoryAllocs);

	//Give them what they really came for
	return calloc(count, count * size);
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
	printf("MALLOC:  Bytes: %i  Total Allocs: %i\n", count * size, ++TotalMemoryAllocs);

	//Give them what they really came for
	return malloc(count * size);
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
	printf("FREEING DATA:  Total Frees: %i\n", ++TotalMemoryFrees);

	free(memory);
}
