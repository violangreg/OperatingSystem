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
	int sleeptime, seed;
	
	pid_t childpid; // declaring children variables & children template

	// Checks input arguments
	if (argc != 4) 
	{
		printf("Usage: %s sleep int int \n", argv[0]);
		exit(1);
	}
	
	sleeptime = atoi(argv[1]); //
	
	// Input checker
	if(sleeptime < 0 || sleeptime > 50)
	{
		printf("Error, sleeptime is out of bounds (0-50)\n");
		exit(1);
	}

	// Forking and creating a children tree up to 'nth' level
	for(int i = 0; i < 4; i++)
	{
		char temp_i = (char) i + '0';
		char *new_argv[] = {"./project2b", &temp_i, argv[1], argv[2], argv[3], NULL};
		
		if((childpid = fork()) == 0)
		{
			int ret = execve("project2b", new_argv, NULL);
			perror("Exec failure");
			exit(1);
		}
	} 

	// Check if the fork failed
	if(childpid < 0)
	{
		perror("The fork failed \n");
		exit(1);
	}

	// Display output
	printf("I am the process parent, the maximum sleep time is %d and the two numbers are %d and %d \n", sleeptime, atoi(argv[2]), atoi(argv[3])); 
	
	/* since each process has a different childpid, using the childpid
	as the seed number will restart the random function.
	Therefore, each process will have a different sleeptime
	*/
	seed = (int)(getpid() + childpid);
	srand(seed);

	int s = rand()%sleeptime;
	
	// sleep before exiting
	sleep(s);
	exit(0);
}

// End of main

