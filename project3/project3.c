/**************************************************************************/
/* PROGRAM: Locks - Project 3, 4/6/17
/* NAME: Greg Paolo Violan, 011706641
/* CLASS: CECS326 Sec 01
/* DESCRIPTION: This program creates 3 children, each children tries to 
/* obtain a lock and cat text.dat then release the lock for other children to obtain.
/* Once released, the child terminates.
/**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

void main(int argc, char *argv[])
{				
	char *lock = "lock";		// name of lock file
	int status; 			// used for checking the status of the child
	
	// declaring pids
	pid_t pid, childpid, w;
	pid = getpid();
	srand((unsigned) pid);
	
	// checks input arguments
	if (argc != 4) 
	{
		printf("Usage: %s filename sleeptime num_tries \n", argv[0]);
		exit(1);
	}
	
	if(atoi(argv[2]) < 0 || atoi(argv[3]) <= 0)
	{
		printf("sleeptime cannot be < 0 or num_tries cannot be <= 0 \n", argv[0]);
		exit(1);
	}
	
	// if lock exist, delete it before starting
	if(access(lock, F_OK) != -1)
	{
		unlink(lock);
	}

	// create 3 children
	for(int i = 0; i < 3; i++)
	{
		char temp_i = (char) i + '0';	// child k variable
		char *new_argv[] = {"./project3child", &temp_i, argv[1], argv[2], argv[3], NULL};
	
		// forking and exec'ing, passing in argv array with proper arguments  
		if((childpid = fork()) == 0)
		{
			// find project3child file and passing in new_argv as its command line starting with ./project3child as argv[0] ... and so forth
			int ret = execve("project3child", new_argv, NULL);
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
	
	// wait for children
	while ((w = wait(&status)) && w != -1)
	{ 
		if (w != -1)
			printf ("Wait on PID: %d returns status of: %04X \n", w, status); 
	}
	
	sleep(1);
	exit(0);
}

// end of file
