#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void main(int argc, char *argv[]) 
{ 
	pid_t pid; 
	int ret_value, time; 
 	
 	time = atoi(argv[2]);
 	
	pid = getpid();
	ret_value = (int)(pid % 256);
	
	srand((unsigned) pid);  
	sleep(rand() % 5);    
	
	if (atoi(*(argv + 1)) % 2) 
	{
		printf("Child %d is terminating with signal 009 \n", pid);  
		kill(pid, 9);
	}
	else
	{  
		printf("Child %d is terminating with exit(%04X) \n", pid, ret_value); 
		exit(ret_value);
	}

} 

