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

static void printArray(int * dataArray);
static void sortArray();

int main(void)
{
	int dataArray[10] = { 1000, 1, 10, 100, 500, 30, 22, 8, 50 };

	SetConsoleTitle((LPCSTR)"GAM150Debug");

	printArray(dataArray);

	sortArray();

	printArray(dataArray);

	printf("Press 'enter' to quit.");
	getchar();
}

/******************************************************************************/
/*!
\brief
Prints an array of ints to the output window.
\param dataArray
Pointer to the data array to be sorted.
*/
/******************************************************************************/

static void printArray(int * dataArray)
{
	int i;

	for(i = 0; i < 10; ++i)
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

static void sortArray(int * dataArray)
{
	int i, j;

	for(i = 0; i < 10; ++i)
	{
		for(j = 0; j < 10 - i; ++j)
		{
			if (dataArray[j] > dataArray[i + 1])
			{
				int temp;
				temp = dataArray[j];
				dataArray[j] = dataArray[i + 1];
				dataArray[j + 1] = temp;
			}
		}
	}
}