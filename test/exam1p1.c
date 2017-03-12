/**************************************************************************/
/* Name: Greg Paolo Violan
/* PROGRAM: exam 1 problem 1 */
/* DESCRIPTION: Write a program to create a child process. Use comments as appropriate to explain the coding.
The include files are given below. The parent process displays the message “I am the parent
process. My process ID is parentID and my child ID is mychildID” where parentID is the process
ID of the parent program and mychildID is the process ID of the child process. Then the parent
process sleeps for a random time between 0 to 15 seconds and then exits. The child process
displays the message “I am the child process. My process ID is childID” where childID is the
process ID of the child process. Then, the child process exits. */
/**************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

int main()
{
	pid_t childpid, n1, n2, n3, n4;
	childpid = 0;
	/*
	if((childpid = fork()) == 0)
	{
		childpid = (long)getpid();
		printf("I am the child process. My process ID is %d \n", (long)getpid());
	}
	if(childpid > 0 )
	{
		if(childpid == (long)getpid()){}
		else
			printf("I am the parent process. My process ID is %d and my child ID is %d \n", (long)getpid(), childpid);
	}
*/
	if((n2 = fork()) || (n1 = fork()) || (n3 = fork())){}
	//if((n1 = fork()) > 0 ){}
	printf("id: %d p: %d\n", (long)getpid(), (long)getppid());
	// Check if the fork failed
	if(childpid == -1)
	{
		perror("The fork failed \n");
		exit(1);
	}

	// Display output
	
	    
	// sleep for one sec so we can retrieve & display the proper output before exiting
	sleep(1);
	exit(0);
}

// End of main

