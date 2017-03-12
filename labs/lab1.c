/****************************************************************/
/* PROGRAM NAME: lab1.c */ 
/* DESCRIPTION:  This lab assignment introduces C syntax and use of command line arguments*/ 
/* ASSIGNMENT: Write a C program named lab1 to store N random numbers in an array named randA[ ] and determine the largest, smallest, and average values of this array.  Display the array elements that store these values.  The random numbers range from -100 to +100.  N is a command line argument.  Use the debugger to go through the program.   Use all the debugger commands. */
/* CREATED BY: Greg Paolo Violan; ID: 011706641
/****************************************************************/

#include<stdio.h>
#include <time.h>
#include <stdlib.h>
main(int argc, char *argv[])
{
	// declaring varialbes, 
	// n for # of random generated numbers
	// lrg for the largest number of all the elements in the array
	// sml for the smallest number of all the elements in the array
	// avg for the average number of all the elements in the array
	int n, lrgst, smlst, avg = 0;
	
	
  	if (argc !=2) 
	{
		printf("Usage: First argument is the filename %s, second argument is 'n' numbers of randomly generated numbers (must be > 0).\n", argv[0]);
		exit(1);
	}
		

	n = atoi(argv[1]);				// convert argument string to an int and save it to n
	
	if(n <= 0)
	{
		printf("Error: argument is not > 0.\n");	// 'n' must be greater than 0.
		exit(-1);
	}			
	
	int randA[n];					// array A elements with n number of elements
	srand(time(NULL));				// generate random number of n numbers
	for(int i = 0; i < n; i++)			// print generated random numbers
	{
		randA[i] = rand()% 201 + (-100);	// store n number of random elements into the array from -100 to 100
		printf("%d\n", randA[i]);		// testing point, prints out the arrays
	}

	lrgst = LrgstElement(randA, n);			// getting the largest element in the array
	smlst = SmlstElement(randA, n);			// getting the smallest element in the array
	avg = Average(randA, n);			// getting the average of the elements in the array
	
	// displaying results
	printf("Displaying the results");
	printf("\n Largest: ");
	printf("%d", lrgst); 
	printf("\n Smallest: ");
	printf("%d", smlst); 
	printf("\n Average: ");
	printf("%d", avg);
	printf("\n");
}

// finding the largest element of the array
// param: the array, array size
int LrgstElement(int array[], int n)
{
	// assign lrgst to the smallest possible value
	int lrgst = -100;
	for(int i = 0; i < n; i++)
	{
		if(array[i] > lrgst)
		{
			lrgst = array[i];
		}
	}
	return lrgst;
}

// finding the smallest element of the array
// param: the array, array size
int SmlstElement(int array[], int n)
{	// assign smlst to the biggest possible value
	int smlst = 100;
	for(int i = 0; i < n; i++)
	{
		if(array[i] < smlst)
		{
			smlst = array[i];
		}
	}

	return smlst;
}

// finding the average of the element of the array
// param: the array, array size
int Average(int array[], int n)
{
	int avg, sum = 0;
	for(int i = 0; i < n; i++)
	{
		sum += array[i];
	}

	avg = sum/n;

	return avg;
}

// program end


