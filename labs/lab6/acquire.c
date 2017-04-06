#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>

void main(int argc, char *argv[])
{
	char *fname; 
	int fd, sleeptime, n_try, count=0; 
	
	pid_t pid;
	pid = getpid(); 
	srand((unsigned) pid); 
	
	fname = argv[1]; 
	sleeptime = atoi(argv[2]); 
	n_try = atoi(argv[3]);
	
	while ((fd = creat(fname, 0)) == -1 && errno == EACCES)
	{
		if (++count < n_try) 
			sleep(rand()%sleeptime);
		else 
		{ 
			printf ("\n Unable to generate.\n"); 
			exit(-1);
		}
	}
	close (fd); 
	printf ("\n File %s has been created\n", fname);
}
