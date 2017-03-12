/**************************************************************************/
/* PROGRAM: lab2.c */
/* DESCRIPTION: This program generates a chain of processes */
/* using fork(). The number of processes n is a command line argument. */
/* Each process sleeps for a random time less than 10 seconds then prints out */
/* process ID, parent ID, and child ID */
/**************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
int main(int argc, char *argv[])
{
	int i, m, n, sleeptime, seed;
	int pid_t, childpid;

	if (argc != 3) 
	{
		printf(" Usage: %s processes sleep \n", argv[0]);
		exit(1);
	}
	
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	if(n < 0 || n > 10 && m < 0 || m > 20)
	{
		printf(" Error, processes input is out of bounds, 0 < n < 10");
		printf("\n Error, sleep input is out of bounds, 0 < m < 20 \n");
		exit(1);
	}
	else if(n < 0 || n > 10)
	{
		printf("Error, processes input is out of bounds, 0 < n < 10 \n");
		exit(1);
	}
	else if(m < 0 || m > 20)
	{
		printf("Error, sleep input is out of bounds, 0 < m < 20 \n");
		exit(1);
	}


	childpid = 0;	
	for (i = 0; i < n; i++)
	if (childpid = fork()) break;

	if (childpid == -1) 
	{
		perror ("\n The fork failed\n");
		exit(1);
	}
	printf("\n %d: process ID:%6ld parent ID:%6ld childID:%6ld", i , (long)getpid(), (long)getppid(), (long)childpid ); 
	
	seed = (int)(getpid() + childpid);
	srand(seed);
	
	/* since each process has a different childpid, using the childpid
	as the seed number will restart the random function.
	Therefore, each process will have a different sleeptime
	*/

	sleeptime = rand()%m;
	printf (" sleep = %d\n", sleeptime);
	sleep(sleeptime);
	exit(0);
}
