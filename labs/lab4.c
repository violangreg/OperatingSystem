/* 
   PROGRAM: lab4.c 
*/ 
# include <stdio.h> 
# include <unistd.h> 

void main() 
{ 
	static char *mesg[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};      
	int display(char *), i; 
 
	for (i = 0; i < 10; ++i) 
	 	display(mesg[i]); 
	 	 	
	//sleep(2);
	printf("%d \n", getpid());
}  

int display(char *m) 
{ 
	char err_msg[25]; 

	switch (fork())
	{           
		case 0:
			printf("Parent: %d, PID: %d,", getppid(), getpid());
			execlp("/bin/echo", "echo", m, (char *) NULL);
			sprintf (err_msg, "%s Exec failure", m);
			perror(err_msg);
			return(1);
		case -1:
			perror ("Fork failure");
			return(2);           
		default: 
			return(0); 
    	} 
}

