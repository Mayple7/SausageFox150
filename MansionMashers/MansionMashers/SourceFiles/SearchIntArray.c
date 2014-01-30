
#include <stdio.h>
#include "../HeaderFiles/SearchIntArray.h"

int searchHitArray(int *Array, int length, int target)
{
	int i;
	int openSlot = length;

	for(i = 1; i < length; i++)
	{
		if(Array[i] / 10 == target)
		{
			return i;
		}
		else if((Array[i] == 0 || Array[i] == -1) && openSlot == length)
		{
			openSlot = i;
		}
	}
	
	return -openSlot;
}
