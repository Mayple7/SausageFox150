/*****************************************************************************/
/*!
\file				SearchIntArray.c
\author				Dan Muller (d.muller)
\date				Feb 26, 2024

\brief				Searches through an int array

\par				Functions:
\li					searchhitArray
  
\par 
<b> Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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
			break;
		}
	}
	
	return -openSlot;
}
