/*****************************************************************************/
/*!
\file				MemoryManager.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_MEM_MANAGER
#define FOX_MEM_MANAGER

//Total times alloced
int TotalMemoryAllocs;
//Total times freed
int TotalMemoryFrees;

//Plain old callocing
void *__cdecl CallocMyAlloc(int count, int size);
//Plain old mallocing
void *__cdecl MallocMyAlloc(int count, int size);
//Plain old memsetting
void *__cdecl MemsetMyAlloc(int count, int size, int value, void *list);
//Plain old freeing
void __cdecl FreeMyAlloc(void *memory);

#endif