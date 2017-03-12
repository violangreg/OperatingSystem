# include <stdio.h> 
# include <stdlib.h> 
# include <sys/types.h> 
# include <unistd.h> 

void main(int argc, char *argv[]) 
{      
	static char *new_argv[] = {"echo","yourfile.txt", (char *) 0};
	execvp("/bin/echo", new_argv);
	perror("exec failure");
	exit(1);
}
