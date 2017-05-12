/* Unnamed pipe pipe.c */
/* Usage: pipe message_to_be_written. Parent write a message to child */

//Use include files: 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#define BUFSIZE 32
// changing the message
void lowerToUpper_UpperToLower(char* message[])
{	
	int ch;
	for (int i = 0; i < BUFSIZE; i++)
	{
		ch = islower(message[i])? toupper(message[i]) : tolower(message[i]);
		putchar(ch);
	}
}


void main(int argc, char *argv[])
{
	mode_t fifo_mode = S_IRUSR | S_IWUSR; //int mkfifo (const char *path, mode_t mode); success: 0, failure: -1, sets errno. mkfifo creates the FIFO file referenced by path; 									mode: File mode bits: S_IRUSR read permission, owner. S_IWUSR write permission, owner 
	int f_des[2]; 
	static char message[BUFSIZE]; 
	char buffer[MAX_CANON]; 
	char *c;
	int i, k, n, child, fd, status; 
	char buf[BUFSIZE];
	unsigned strsize;
	pid_t childpid;
	
	if (argc != 4 && argc != 5) 
	{ 
		printf ("\n Usage: %s pipe_type (if n: pipe_name) direction message(limit of 32 chars)\n", *argv); 
		exit(1);
	}
	
	if(strcmp(argv[1], "u") == 0 && strcmp(argv[1], "n") == 0 || strcmp(argv[3], "p") == 0 && strcmp(argv[3], "c") == 0)
	{
		printf ("argv[1]: pipe_type (n/u) or argv[3]: direction (p/c)");
		exit(1);
	}
	
	if(strcmp(argv[1], "u") == 0)
	{
		/* generate unnamed pipe */ 
	 	if (pipe(f_des) == -1) // pipe
	 	{ 
	 		perror ("unnamed_pipe"); 
	 		exit(2); 
	 	}
	 	
	 	//child writes, parent receives
		if(strcmp(argv[2], "c") == 0)
		{
			switch (childpid = fork()) 
			{
				case -1: 
					perror ("Fork"); 
					exit(3);
				case 0: 
				 	/* In the child */
				 	
					// writing message
					close(f_des[0]);
					childpid = getpid();
					if (write(f_des[1], argv[3], strlen(argv[3])) != -1)
					{
						printf ("child %d is about to send the message: [%s] to FIFO\n", (long)getpid(), argv[3]);
						printf("message sent\n");
						fflush(stdout); 
					}
					else 
					{ 
						perror ("Write"); 
						exit(5); 
					}
					
				 	break;
					
					default: 
					/* In the parent */
					
					// wait for child
					while ((wait(&status) == -1) && (errno == EINTR));
					
					close(f_des[1]); // close current file?
					if (read(f_des[0], message, BUFSIZ) != -1) 
					{ 
						printf ("parent is about to read the message from FIFO\n");
						// changing message
						//lowerToUpper_UpperToLower(message);
						printf ("parent receives the message *%s* from child %d\n", message, childpid); 
						fflush(stdout); 
					}
					else 
					{ 
					 	perror ("Read"); 
					 	exit(4);
					} 
			}// end of switch statement
		}// end of nested if
		else{
			switch (fork()) 
			{
				case -1: 
					perror ("Fork"); 
					exit(3);
				case 0: 
					/* In the child */
					close(f_des[1]); // close current file?
					
					// changing message
					//lowerToUpper_UpperToLower(message);
					
					// reading message
					if (read(f_des[0], message, BUFSIZ) != -1) 
					{ 
						printf ("child %d is about to read the message from FIFO\n", getpid());
						printf ("child %d receives the message *%s* from parent\n", getpid(), message);
						fflush(stdout); 
					}
					else 
					{ 
					 	perror ("Read"); 
					 	exit(4);
					}
					break;
				default:
					/* In the parent */
					close(f_des[0]);
					
					// writing message
					if (write(f_des[1], argv[3], strlen(argv[3])) != -1)
					{
						printf ("parent is about to send the message: [%s] to FIFO\n", argv[3]);
						printf("message sent\n");
						fflush(stdout); 
					}
					else 
					{ 
						perror ("Write"); 
						exit(5); 
					}
			}// end of switch statement
		} // end of else
	 }
	 else
	 {
		/* generate a named pipe with r/w for user */
		if ((mkfifo(argv[2], fifo_mode) == -1) && (errno != EEXIST))
		{
			perror ("named_pipe");
			exit(1);
		}
		
		// parent reads, child writes
		if(strcmp(argv[3], "p") == 0)
		{
			// fork and create a child to open FIFO
			if ((childpid = fork()) == -1)
			{
				perror ("Fork");
				exit(1);
			}
			else if (childpid == 0)
			{
				// child opening FIFO
				printf ("Child %ld is about to open FIFO %s \n", (long)getpid(), argv[2]);
				
				if ((fd = open(argv[2], O_WRONLY)) == -1) // WRITE ONLY
				{
					perror("Child cannot open FIFO");
					exit(1);
				}
		
				/* In the child */
				sprintf (buf, "This was written by child %ld \n", (long)getpid());
				strsize = strlen(buf) + 1;
		
				if (write(fd, buf, strsize) != strsize) 
				{
					printf("Child write to FIFO failed\n");
					exit(1);
				}
		
				printf ("Child %ld is done\n", (long)getpid());
			}
			else
			{
				/* parent does a read */
				printf ("Parent %ld is about to open FIFO %s\n", (long) getpid(), argv[2]);
		
				// parent opens FIFO
				if ((fd = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1)
				{
					perror("Parent cannot open FIFO");
					exit(1);
				}
				printf ("Parent is about to read\n", (long)getpid());
		
				// waits for the child to finish writing
				while ((wait(&status) == -1) && (errno == EINTR));
		
				if (read(fd, buf, BUFSIZE) <= 0)
				{
					perror("Parent read from FIFO failed\n");
					exit(1);
				}
		
				printf ("Parent %ld received: %s\n", (long)getpid(), buf);
			}
		}
		else // parent writes, child reads
		{
			// fork and create a child to open FIFO
			if ((child = fork()) == -1)
			{
				perror ("Fork");
				exit(1);
			}
			else if (child == 0)
			{
				printf ("Child %ld is about to open FIFO %s \n", (long)getpid(), argv[2]);
		
				if ((fd = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1) // WRITE ONLY
				{
					perror("Child cannot open FIFO");
					exit(1);
				}
		
				/* In the child */
				sprintf (buf, "This was written by child %ld \n", (long)getpid());
				strsize = strlen(buf) + 1;
				
				 // child read
				if (read(fd, buf, BUFSIZE) <= 0)
				{
					perror("Child read from FIFO failed\n");
					exit(1);
				}
				
				printf ("Child %ld received: %s\n", (long)getpid(), buf);
			}
			else
			{
				/* parent does a write */
				printf ("Parent %ld is about to open FIFO %s\n", (long) getpid(), argv[2]);
		
				// parent opens FIFO
				if ((fd = open(argv[2], O_WRONLY)) == -1)
				{
					perror("Parent cannot open FIFO");
					exit(1);
				}
				printf ("Parent is about to write\n", (long)getpid());
			
				// parent write
				if (write(fd, buf, strsize) != strsize) 
				{
					printf("Parent write to FIFO failed\n");
					exit(1);
				}
				printf ("Parent %ld is done\n", (long)getpid());
			}
		}
		
		
	 }
	 
	exit(0);
} 


