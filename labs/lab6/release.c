#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

void main(int argc, char *argv[])
{
	char *fname; 
	int fd, sleeptime, n_try, count = 0; 
	
	pid_t pid;
	pid = getpid();
	srand((unsigned) pid); 
	
	fname = argv[1]; 
	sleeptime = atoi(argv[2]); 
	n_try = atoi(argv[3]);
	
	while (unlink(fname) != 0)
	{
		if (++count < n_try) 
			sleep(sleeptime);
		else 
		{
			printf ("\n Cannot release file\n"); 
			exit(-1);
		}
	}
	printf ("\n File is released\n");
}
