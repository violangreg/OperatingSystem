/**************************************************************************/
/* PROGRAM: Locks - Project 3, 4/6/17
/* NAME: Greg Paolo Violan, 011706641
/* NAME: Luke Nyugen,
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
	// declaring varaibles
	char *fname;							// name of file passed from parent
	char *lock = "lock";						// name of lock file
	
	int fd;								// return value of creat 'lock' (used to close)
	int sleeptime;							// sleep time period
	int num_tries;							// number of tries (of obtaining a lock) before terminating child
	int count = 0;							// incremental var to check number of tries
	int ret_value;							// return value for exiting child (used for parent's wait status)
	int k;								// child's #
	
	// declare and get the child's pid, ret_value
	pid_t pid, childpid;
	pid = getpid();
	ret_value = (int)(pid % 256);
	srand((unsigned) pid); 
	
	// assign parent's argvs to their proper variables
	k = atoi(argv[1]); 
	fname = argv[2]; 							// notice this is not atoi'd (since we want the char not int)
	sleeptime = atoi(argv[3]);
	num_tries = atoi(argv[4]);
	
	// try to create the lock
	while ((fd = creat(lock, 0)) == -1 && errno == EACCES)
	{
		if (++count < num_tries)
		{
			sleep(rand()%sleeptime);
		}
		else 
		{ 
			printf ("Child %d: unable to obtain lock file \n", k); 
			kill(pid, k);
			exit(-1);
		}
	}
	
	// check if it's the right file
	if(strcmp(fname, "text.dat") != 0)
	{
		printf("%s is not the right file (text.dat). \n", fname);
		exit(-1);
	}
	
	printf("Child %d: successfully created 'lock' \n", k);
	
	// display the file contents (using another child w/ exec)
	if((childpid = fork()) == 0)
	{
		execl("/bin/cat", "cat", fname, (char *) 0);
		perror("Exec failure \n");
		exit(1);
	}

	// check if the fork failed
	if(childpid < 0)
	{
		perror("The fork failed \n");
		exit(1);
	}
	
	// release the lock
	unlink(lock);
	
	printf("Child %d: releasing 'lock', process LSB: %d \n", k, ret_value);
	
	close (fd);
	exit(ret_value);
}

// end of file
