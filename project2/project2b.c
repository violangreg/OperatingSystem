/**************************************************************************/
/* Name: Greg Paolo Violan and Luke Nguyen
/* PROGRAM: project2.c */
/* DESCRIPTION: Parent makes children and children does arithmetic functions*/
/**************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>


int main(int argc, char *argv[])
{
	int i, sleeptime, n1, n2, seed;
	int sum, diff, mult, div;
	
	i = atoi(argv[1]);
	sleeptime = atoi(argv[2]);
	n1 = atoi(argv[3]);
	n2 = atoi(argv[4]);

	switch(i)
	{
		case 0:
			sum = n1 + n2;
			printf("I am child 0 with PID %d, the sum is %d \n", getpid(), sum);
			break;
		
		case 1:
			diff = n1 - n2;
			printf("I am child 1 with PID %d, the difference is %d \n", getpid(), diff);
			break;
		case 2:
			mult = n1 * n2;
			printf("I am child 2 with PID %d, the multiplicative is %d \n", getpid(), mult);
			break;
		case 3:
			if(n2 == 0)
			{
				printf("Error: second int is 0");
				exit(1);
			}
			
			div = n1 / n2;
			printf("I am child 3 with PID %d, the divided is %d \n", getpid(), div);
			break;
		default: printf("case: default \n");
			break;
	}
	
	/* since each process has a different childpid, using the childpid
	as the seed number will restart the random function.
	Therefore, each process will have a different sleeptime
	*/
	seed = (int)(getpid());
	srand(seed);

	int s = rand()%sleeptime;
	
	// sleep before exiting
	sleep(s);
	exit(0);
}

// End of main

