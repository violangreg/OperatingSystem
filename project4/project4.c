// Violan, Greg Paolo | 011706641
// Nyugen, Luke
// CECS 326, Sec 01
// April 25, 2017
// Project4: Semaphores

// included headers
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

// data structure needed for semctrl
union semun {
	int val; 
	struct semid_ds *buf; 
	ushort *array;
};

int main(int argc, char *argv[])
{
	char *ropt;	// ropt used for command 'r' and 'n'.
	int sem_id, sem_value, i, NS; // sem_id is semaphore id, sem_value is the semaphore value, i is for incremental variable, NS is the number of the semaphore in a set
	key_t ipc_key; // unique sem identifier
	struct semid_ds sem_buf; // declare simid_ds (sem data structure) function as sem_buf
	
	// argument checker
	if(argc <= 3){
		printf("Usage: need more than three arguments \n");
		exit(1);
	}
	
	// take in ropt and check if its either 'r' for remove or 'n' for nonremove
	ropt = argv[1];
	if(strcmp(ropt, "r") != 0 && strcmp(ropt, "n") != 0)
	{
		printf("argv[1] is incorrect: 'r' for remove, 'n' for not remove \n");
		exit(0);
	}
	
	// check if number of semaphore is inputted properly
	NS = atoi(argv[2]);
	if(argc < (NS+3) || argc > (NS+3)){
		printf("Arguments not matched. Number of semaphore is %d \n", NS);
		exit(0);
	}
	
	ushort sem_array[NS]; // creates a ushort (0-61k) sem_array[3], where we will put the semaphores in
	for (int i = 0; i < NS; i++)
	{
		// check for negative semaphores
		if(atoi(argv[3+i]) > 0)
		{
			sem_array[i] = atoi(argv[3+i]);
		}
		else{
			printf("Arguments consisted of negative semaphores \n");
			exit(0);
		}
	}
	
	union semun arg; // declares semun called arg
	ipc_key = ftok(".", 'S'); //path, id
	
	/* Create semaphore */
	if ((sem_id = semget(ipc_key, NS, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | IPC_CREAT | IPC_EXCL)) == -1) // creates a semaphore with the generated key 
	{										// from ftok, number of semphamore in the set is NS, creates a new semaphore but returns an error when the 
		perror ("semget");						//semaphore already exists.
		exit(1);
	}
	
	printf ("Semaphore identifier %d\n", sem_id);	// print out the semaphore identifier
	
	/* Set arg (the union) to the address of the storage location for */
	/* returned semid_ds value */	
	arg.buf = &sem_buf;
	if (semctl(sem_id, 0, IPC_STAT, arg) == -1)     //IPC_STAT: return the current values of the semid_ds structure for the indicated semaphore
	{								//identifier. The returned information is stored in a user-generated structure referenced by the fourth
		perror ("semctl: IPC_STAT");			// argument to semctl. Must have read permission.
		exit(2);	
	}
	printf ("Create %s", ctime(&sem_buf.sem_ctime)); // returns the date and time of creation
	
	/* Set arg (the union) to the address of the initializing vector */
	arg.array = sem_array;	
	if (semctl(sem_id, 0, SETALL, arg) == -1) // SETALL: initializes all semaphores in a set to the values stored in the array referenced by arg.
	{
		perror("semctl: SETALL");
		exit(3);
	}
	
	
	// print out the semaphores in the array set
	for (i = 0; i < NS; ++i) 
	{
		if ((sem_value = semctl(sem_id, i, GETVAL, 0)) == -1) // GETVAL: return the current value of the individual semaphores referenced by the value of the semnum argument.
		{
			perror("semctl : GETVAL");
			exit(4);
		}
		printf ("Semaphore %d has value of %d\n", i, sem_value);
	}
	
	// remove the semaphore set
	if (strcmp(ropt, "r") == 0){
		if (semctl(sem_id, 0, IPC_RMID, 0) == -1) //IPC_RMID: remove the semaphore set associated with the semaphore identifier.
		{
			perror ("semctl: IPC_RMID");
			exit(5);
		}
	}
	
	//return and end	
	return 1;
}
