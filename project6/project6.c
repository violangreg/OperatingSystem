/* Unnamed/named pipe + case sensitivity change pipe.c */

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

void main(int argc, char *argv[])
{
	mode_t fifo_mode = S_IRUSR | S_IWUSR; //int mkfifo (const char *path, mode_t mode); success: 0, failure: -1, sets errno. mkfifo creates the FIFO file referenced by path; 									mode: File mode bits: S_IRUSR read permission, owner. S_IWUSR write permission, owner 
	int f_des[2]; 
	static char message[BUFSIZE], new_message[BUFSIZE]; 
	char buffer[MAX_CANON]; 
	char *c;
	int i, k, n, child, fd, status; 
	char buf[BUFSIZE];
	unsigned strsize;
	pid_t childpid;
	i = 0;
	
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
						
						// change case sensitivity
						  while (message[i])
						  {
						    c = message[i];
						    
					    	    if(islower(c)) 
						    {
							 c = toupper(c);
						    }
						    else 
						    {
							 c = tolower(c);
						    }
						    
						    new_message[i] = c;
						    i++;
						  }
						  
						printf ("parent receives the message *%s* from child %d\n", new_message, childpid); 
						fflush(stdout); 
					}
					else 
					{ 
					 	perror ("Read"); 
					 	exit(4);
					} 
			}// end of switch statement
		}// end of nested if
		else{ // child receives, parent writes
			switch (fork()) 
			{
				case -1: 
					perror ("Fork"); 
					exit(3);
				case 0: 
					/* In the child */
					close(f_des[1]); // close current file?
					
					
					// reading message
					if (read(f_des[0], message, BUFSIZ) != -1) 
					{ 
						printf ("child %d is about to read the message from FIFO\n", getpid());
						
						// change case sen.
						  while (message[i])
						  {
						    c = message[i];
						    
					    	    if(islower(c)) 
						    {
							 c = toupper(c);
						    }
						    else 
						    {
							 c = tolower(c);
						    }
						    
						    new_message[i] = c;
						    i++;
						  }
						
						printf ("child %d receives the message *%s* from parent\n", getpid(), new_message);
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
		if(strcmp(argv[3], "c") == 0)
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
				if ((fd = open(argv[2], O_WRONLY)) == -1) // WRITE ONLY
				{
					perror("Child cannot open FIFO");
					exit(1);
				}
		
				/* In the child */
				strsize = strlen(argv[4]) + 1;
		
				if (write(fd, argv[4], strsize) != strsize) 
				{
					printf("Child write to FIFO failed\n");
					exit(1);
				}
				
				printf ("child %d is about to send the message [%s] to %s\n", (long)getpid(), argv[4], argv[2]);
				printf("message sent\n");
			}
			else
			{
				/* parent does a read */
				// parent opens FIFO
				if ((fd = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1)
				{
					perror("Parent cannot open FIFO");
					exit(1);
				}
				
				printf ("parent is about to read the message from %s\n", argv[2]);
		
				// waits for the child to finish writing
				while ((wait(&status) == -1) && (errno == EINTR));
				
				if (read(fd, buf, BUFSIZE) <= 0)
				{
					perror("Parent read from FIFO failed\n");
					exit(1);
				}
				
				// change case sen.
				  while (buf[i])
				  {
				    c = buf[i];
				    
			    	    if(islower(c)) 
				    {
					 c = toupper(c);
				    }
				    else 
				    {
					 c = tolower(c);
				    }
				    
				    new_message[i] = c;
				    i++;
				  }
				  
				printf ("parent receives the message *%s* from child %d\n", new_message, (long)childpid);
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
				if ((fd = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1)
				{
					perror("Child cannot open FIFO");
					exit(1);
				}
				
				 // child read
				printf ("child %d is about to read the message from %s\n", getpid(), argv[2]);
				 
				if (read(fd, buf, BUFSIZE) <= 0)
				{
					perror("Child read from FIFO failed\n");
					exit(1);
				}
				
				// change case sensitivity
				  while (buf[i])
				  {
				    c = buf[i];
				    
			    	    if(islower(c)) 
				    {
					 c = toupper(c);
				    }
				    else 
				    {
					 c = tolower(c);
				    }
				    
				    new_message[i] = c;
				    i++;
				  }
				printf ("child receives the message *%s* from parent\n", new_message, (long)childpid);
			}
			else	
			{
				/* parent does a write */
				// parent opens FIFO
				if ((fd = open(argv[2], O_WRONLY)) == -1)
				{
					perror("Parent cannot open FIFO");
					exit(1);
				}
				//printf ("Parent is about to write\n", (long)getpid());
				strsize = strlen(argv[4]) + 1;
				
				// parent write
				if (write(fd, argv[4], strsize) != strsize) 
				{
					printf("Parent write to FIFO failed\n");
					exit(1);
				}
				
				printf ("parent is about to send the message [%s] to %s\n", argv[4], argv[2]);
				printf("message sent\n");
			}
		}
		
		
	 }
	 
	exit(0);
} 


