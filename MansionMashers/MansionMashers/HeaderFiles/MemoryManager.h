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
//Plain old freeing
void __cdecl FreeMyAlloc(void *memory);

#endif