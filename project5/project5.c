// Violan, Greg Paolo | 011706641
// Nyugen, Luke
// CECS 326, Sec 01
// May 4, 2017
// Project5: Semaphore and buffer

// included headers
#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

// method for setting up the data structure of the semaphore
void set_sembuf_struct(struct sembuf *s, int num, int op, int flg)
{
	s-> sem_num = (short) num;
	s-> sem_op = op;
	s-> sem_flg = flg;
	return;
}

void main(int argc, char *argv[])
{
	// SETTING UP
	char *opt;	// opt used for operation 'n' no semaphore protection and 's' semaphore protection.
	char buffer[MAX_CANON]; // buffer used for printing
	int semid, k, N, semop_ret, status; // semid is the semaphore id, k is the sleep value, N is the number of processes, semop_ret is the return value of semop used for condition, status is for the children current status
	int i = 0; // i is for incremental variable
	key_t ipc_key; // unique sem identifier
	pid_t childpid, w; // childpid is the return pid_t from the child, w is the pid_t wait for children
	
	// argument checker
	if(argc != 4)
	{
		printf("Usage: file #processes s/n sleep \n");
		exit(1);
	}

	// check if number of processes is inputted properly
	N = atoi(argv[1]);
	if(N <= 0)
	{
		printf("argv[1]: number of processes needs to be greater than 0 \n");
		exit(0);
	}
	
	// take in opt and check if its either 'n' no semaphore protection or 's' semaphore protection
	opt = argv[2];
	if(strcmp(opt, "n") != 0 && strcmp(opt, "s") != 0)
	{
		printf("argv[2]: opt is incorrect: 'n' for no semaphore protection, 's' for semaphore protection \n");
		exit(0);
	}
	
	// check sleep value
	k = atoi(argv[3]);
	if(k < 0){
		printf("argv[3]: sleep needs to be greater than or equal to 0 \n");
		exit(0); 
	}
	
	// create unique sem identifier
	ipc_key = ftok(".", 's'); //path, id
	/* Create semaphore */
	if ((semid = semget(ipc_key, 1, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | IPC_CREAT | IPC_EXCL)) == -1) // creates a semaphore with the generated key 
	{										// from ftok, number of semphamore in the set is NS, creates a new semaphore but returns an error when the 
		perror ("semget");						//semaphore already exists.
		exit(1);
	}
	
	// critical section /*setting up semaphore*/
	struct sembuf semwait[1];
	struct sembuf semsignal[1];
	
	/* Initialize semaphore element to 1 */
	set_sembuf_struct(semwait, 0, -1, 0);
	set_sembuf_struct(semsignal, 0, 1, 0);
	
	if (semop(semid, semsignal, 1) == -1) 
	{
		printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
		
		// remove existing semaphore
		if (semctl(semid, 0, IPC_RMID) == -1)
		{
			printf ("%ld: could not delete semaphore - %s\n", (long)getpid(), strerror(errno));
			exit(1);
		}
	}
	
	//creating children
	for (i = 1; i < N; i++)
	{
		if((childpid = fork()) != 0)
		{
			break;
		}
	}
	sprintf(buffer,"i:%d process ID: %d parent ID: %d child ID: %d \n", i, getpid(), getppid(), childpid);
	
	// semaphore protection
	if(strcmp(opt, "s") == 0)
	{
		// entry of cs
		while (( (semop_ret = semop(semid, semwait, 1) ) == -1) && (errno == EINTR)); // check if waiting
		if (semop_ret == -1)
		{
			printf ("%ld: semaphore decrement failed - %s\n", (long)getpid(), strerror(errno));
		}
		else
		{
			// writing the output
			for(int i = 0; buffer[i] != '\0'; i++)
			{
				putchar(buffer[i]);
				fflush(stdout);
			}
		}
		// exit
		while (((semop_ret = semop(semid, semsignal, 1)) == -1) && (errno == EINTR)); // increment
		if (semop_ret == -1)
			printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
			
	}
	else // no semaphore protection
	{
		// writing the output
		for(int i = 0; buffer[i] != '\0'; i++)
		{
			putchar(buffer[i]);
			fflush(stdout);
			usleep(k);
		}
	}
	
	// wait for children
	while ((w = wait(&status)) && w != -1);
	
	// delete semaphore at th end
	if (i == N)
	{
		if (semctl(semid, 0, IPC_RMID, 0) == -1) 
		{
			perror ("semctl: IPC_RMID");
			exit(5);
		}
	}
	
	// exit
	exit(2);
}
// end of file
