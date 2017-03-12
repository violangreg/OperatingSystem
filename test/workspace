/**************************************************************************/
/* Name: Greg Paolo Violan and Luke Nguyen
/* PROGRAM: project1.c */
/* DESCRIPTION: A binary process tree using fork()*/
/**************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

int main(int argc, char *argv[])
{
	int n, i, sleeptime; // declaring variables
	pid_t childpid_1, childpid_2, childpid_1N, childpid_2N; // declaring children variables & children template

	// Checks input arguments
	if (argc != 2) 
	{
		printf("Usage: %s processes sleep \n", argv[0]);
		exit(1);
	}
	
	n = atoi(argv[1]); // assigning second input argument to n

	// Input checker
	if(n < 0 || n > 5)
	{
		printf("Error, processes input is out of bounds \n");
		exit(1);
	}

	// Forking and creating a children tree up to 'nth' level
	for(i = 0; i < n; i++)
	{
		if((childpid_1 = fork()) == 0 || (childpid_2 = fork()) == 0) // creating two children if that current process is a child
			continue; // continue if it is a child (continue will skip the code in between and go to  // the next iteration) 

		childpid_1N = childpid_1; // save the left child's pid
		childpid_2N = childpid_2; // save the right child's pid
		childpid_1 = 0; // reset the pid (because right child will have the left child already when  // the parent fork() 
		childpid_2 = 0;
		break; // break if it is a parent
	} 

	// Check if the fork failed
	if(childpid_1 == -1 || childpid_2 == -1)
	{
		perror("The fork failed \n");
		exit(1);
	}

	// Display output
	if(i == 0)
		printf("LeveNo.  ProcessID  ParentID  Child1ID  Child2ID \n"); 

	printf("%7d%11ld%10ld%10ld%10ld \n", i, (long)getpid(), (long)getppid(), (long)childpid_1N, (long)childpid_2N);
	    
	// sleep for one sec so we can retrieve & display the proper output before exiting
	sleeptime = 1;
	sleep(sleeptime);
	exit(0);
}

// End of main

