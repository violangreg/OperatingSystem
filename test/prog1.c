#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("\nHoola\n");
	
	if (argc > 1) 
	{
		execlp("/bin/cat", "cat", argv[1], (char *) 0);
		perror("Exec Failure");
		exit(1);
	}
	
	printf("\nNeed more arguments\n"); 
	return(1);
}
