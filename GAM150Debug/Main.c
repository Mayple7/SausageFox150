/******************************************************************************/
/*!
 \file   main.c
 \author Doug Schilling
 \par    Course: GAM150
 \par    All content � 2013 DigiPen (USA) Corporation, all rights reserved.
 \brief
	Purpose: Sort and print an array of 10 predetermined values.

	Objectives:
		Identify and fix any defects in the program.
		Remove the magic number ("10") specifying the array length.
		Modify the functions to support arrays of any length.
		Retain the original purpose of the program.

	Note:
		Consider this code to be part of a much larger project.  As a result,
		you should not change any project settings as a means to solve any
		defects that are identified.
 */
/******************************************************************************/

#include <stdio.h>
#include <Windows.h>

#define ARRAYLENGTH 10

static void printArray(int * dataArray, int length);
static void sortArray(int * dataArray, int length);

int main(void)
{
	int dataArray[ARRAYLENGTH] = { 1000, 1, 10, 100, 500, 30, 22, 8, 50 };

	SetConsoleTitle((LPCSTR)"GAM150Debug");

	printArray(dataArray, ARRAYLENGTH);

	sortArray(dataArray, ARRAYLENGTH);

	printArray(dataArray, ARRAYLENGTH);

	printf("Press 'enter' to quit.\n");
	while(getchar() != '\n')
	{
		printf("Press 'enter' to quit.\n");
	}
	return 0;
}

/******************************************************************************/
/*!
\brief
Prints an array of ints to the output window.
\param dataArray
Pointer to the data array to be sorted.
*/
/******************************************************************************/

static void printArray(int * dataArray, int length)
{
	int i;

	for(i = 0; i < length; ++i)
	{
		printf("%6d ", dataArray[i]);
	}
	printf("\n");
}

/******************************************************************************/
/*!
\brief
Sorts an array of ints using bubble sort.
\param dataArray
Pointer to the data array to be sorted.
\note
"the bubble sort seems to have nothing to recommend it, except a catchy name
and the fact that it leads to some interesting theoretical problems"
Donald Knuth, The Art of Computer Programming
*/
/******************************************************************************/

static void sortArray(int * dataArray, int length)
{
	int i, j, temp;

	for(i = 0; i < length; ++i)
	{
		for(j = i+1; j < length; ++j)
		{
			if (dataArray[i] > dataArray[j])
			{
				temp = dataArray[i];
				dataArray[i] = dataArray[j];
				dataArray[j] = temp;
			}
		}
	}
}