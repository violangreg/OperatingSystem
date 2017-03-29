/**************************************************************************/
/* Name: Greg Paolo Violan and Luke Nguyen	DATE: 3/12/17
/* PROGRAM: project2.c */
/* DESCRIPTION: This is the child class, it does the arithmetic functions*/
/**************************************************************************/
// importing proper includes
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

int main(int argc, char *argv[])
{
	// declaring all necessary variables
	// i = key, sleeptime = sleep, n1 = 1st integer (a), n2 = 2nd integer (b), seed = randomization sleep
	// sum = a + b, diff = a - b, mult = a * b, div = a / b
	int i, sleeptime, n1, n2, seed;
	int sum, diff, mult, div, rem;
	
	// assigning argv to proper variables note argv[0] is ./project2b which is calling to run this program
	i = atoi(argv[1]);
	sleeptime = atoi(argv[2]);
	n1 = atoi(argv[3]);
	n2 = atoi(argv[4]);

	// case statements for all the arithmetic functions using the key i
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
				printf("I am child 3 with PID %d, error: sec int is 0 \n", getpid());
			else
			{
				div = n1 / n2;
				rem = n1 % n2;
				if(rem > 0)
					printf("I am child 3 with PID %d, the divided is %d, the remainder is %d \n", getpid(), div, rem);
				else
					printf("I am child 3 with PID %d, the divided is %d \n", getpid(), div);
			}
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
// end of main
