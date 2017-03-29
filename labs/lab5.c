#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void main(int argc, char *argv[]) 
{ 
	pid_t pid, w;
	int k, status, time, n;
	
	
	if(argc != 3)
	{
		printf("Usage: %s, sleeptime, process \n", argv[0]);
		exit(1);
	}
	
	time = atoi(argv[1]);
	n = atoi(argv[2]);
	
	char value[n];
	
	if(time < 0 || time > 50)
	{
		printf("0 < Sleeptime < 50 \n");
		exit(1);
	}
	
	if(n < 0 || n > 20)
	{
		printf("0 < Process < 20 \n");
		exit(1);
	}
	
	for(k = 0; k < n; ++k)
	{
		if((pid = fork()) == 0)
		{
			sprintf(value, "%d", k);
			execl("lab5child", "lab5child", value, argv[1], (char *) 0);
		}        
		else printf ("Forked child %d \n", pid); 
   	}

	/* Wait for children  */ 
	while ((w = wait(&status)) && w != -1)
	{ 
		if (w != -1)
			printf ("Wait on PID: %d returns status of: %04X \n", w, status); 
	}    
	
	exit(0); 
} 

