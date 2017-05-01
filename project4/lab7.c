# include <stdio.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>
# include <unistd.h>
# include <stdlib.h>
# include <time.h>
# define NS 3

// data structure needed for semctrl
union semun {
	int val; 
	struct semid_ds *buf; 
	ushort *array;
};

int main(void)
{
	int sem_id, sem_value, i; 
	key_t ipc_key; // unique sem identifier
	struct semid_ds sem_buf; // declare simid_ds (sem data structure) function as sem_buf
	static ushort sem_array[NS] = {3, 1, 4}; // NS = 3, creates a ushort (0-61k) sem_array[3]
	union semun arg; // declares semun called arg
	ipc_key = ftok(".", 'S'); //path, id
	
	/* Create semaphore */
	if ((sem_id = semget(ipc_key, NS, IPC_CREAT | IPC_EXCL | 0666)) == -1) // creates a semaphore with the generated key from ftok, number of semphamore in the set is 3,
	{										// creates a new semaphore but returns an error when the semaphore already exists.
		perror ("semget: IPC | 0666");
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
	printf ("Create %s", ctime(&sem_buf.sem_ctime));
	
	/* Set arg (the union) to the address of the initializing vector */
	arg.array = sem_array;
	if (semctl(sem_id, 0, SETALL, arg) == -1) // SETALL: initializes all semaphores in a set to the values stored in the array referenced by arg.
	{
		perror("semctl: SETALL");
		exit(3);
	}
	
	for (i = 0; i < NS; ++i) 
	{
		if ((sem_value = semctl(sem_id, i, GETVAL, 0)) == -1) // GETVAL: return the current value of the individual semaphores referenced by the value of the semnum argument.
		{
			perror("semctl : GETVAL");
			exit(4);
		}
		printf ("Semaphore %d has value of %d\n", i, sem_value);
	}

	/* remove semaphore 
	if (semctl(sem_id, 0, IPC_RMID, 0) == -1) //IPC_RMID: remove the semaphore set associated with the semaphore identifier.
	{
		perror ("semctl: IPC_RMID");
		exit(5);
	}*/
}
