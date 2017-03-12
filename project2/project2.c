/**************************************************************************/
/* Name: Greg Paolo Violan and Luke Nguyen	DATE: 3/12/17
/* PROGRAM: project2.c */
/* DESCRIPTION: Parent makes children and children does arithmetic functions*/
/**************************************************************************/
// importing proper includes
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

int main(int argc, char *argv[])
{
	// declaring sleeptime = how many seconds to sleep (wait) the program
	// declaring seed = for randomizing sleeptime
	// delcaring chilpid = for getting the value of the child's pid
	int sleeptime, seed;
	pid_t childpid;

	// checks input arguments
	if (argc != 4) 
	{
		printf("Usage: %s sleep int int \n", argv[0]);
		exit(1);
	}
	
	// assigning argument 1 from command line to sleeptime
	sleeptime = atoi(argv[1]); //
	
	// input checker for sleeptime
	if(sleeptime < 0 || sleeptime > 50)
	{
		printf("Error, sleeptime is out of bounds (0-50)\n");
		exit(1);
	}

	// forking and creating 4 childrens
	for(int i = 0; i < 4; i++)
	{
		/* using i as the key to figure out which case statement to go into on the child program 
		converting i to char since execve can only take char array
		*/
		char temp_i = (char) i + '0';
		char *new_argv[] = {"./project2b", &temp_i, argv[1], argv[2], argv[3], NULL};
		
		// forking and exec'ing, passing in argv array with proper arguments  
		if((childpid = fork()) == 0)
		{
			// find project2b file and passing in new_argv as its command line starting with ./project2b as argv[0] ... and so forth
			int ret = execve("project2b", new_argv, NULL);
			perror("Exec failure");
			exit(1);
		}
	} 

	// check if the fork failed
	if(childpid < 0)
	{
		perror("The fork failed \n");
		exit(1);
	}

	// display parent output
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
// end of main
