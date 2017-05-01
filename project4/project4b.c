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
union semun {
	int val; 
	struct semid_ds *buf;
	ushort *array;
};
int main(int argc, char *argv[])
{
	int sem_id;					//variable to store semaphore id 
	int sem_value;				//variable to store value of semaphore
	int i; 						//counter
	int ns;						//number of semaphores
	key_t ipc_key; 				//store a key generated from ftok()
	char *command;				//store the command for deleting semaphore set	
	struct semid_ds sem_buf; 	//create a storage location for 
	int argCounter;
	if (argc<=3){
		printf("Usage: need more than three arguments !!!\n");
		exit(1);
	}else{
		ns=atoi(argv[2]);
		command=argv[1];
		printf("%s",command);
		if ((strcmp(command,"r")!=0) && (strcmp(command,"n")!=0)){
			printf("Wrong option(either 'r' or 'n')");
			exit(1);
		}else{
			if (ns<1){
				printf("Usage: number of semaphore has to be greater than 0");
				exit(1);
			}else{
				if ((argc-3)!=ns){
					printf("Arguments not matched. Number of semaphores is %d\n",ns);
					exit(1);
				}
			}
		}	
	}
	ushort sem_array[ns]; 	//create a sem_array to populate the users input value for semaphore
	for (i=0;i<ns;i++){
		if (atoi(argv[i+3])>=0){
			sem_array[i]=atoi(argv[i+3]);
		}else{
			printf("Semaphore value can't be negative !!!");
			exit(1);
		}
	}
	union semun arg;
	ipc_key = ftok(".", 's');
	/* Create semaphore */
	if ((sem_id = semget(ipc_key, ns, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH|IPC_CREAT | IPC_EXCL)) == -1) {
			perror ("Semget: ");
			exit(1);
	}
	printf ("Semaphore identifier %d\n", sem_id);
	/* Set arg (the union) to the address of the storage location for */
	/* returned semid_ds value */
	arg.buf = &sem_buf;
	if (semctl(sem_id, 0, IPC_STAT, arg) == -1) {
		perror ("semctl: IPC_STAT");
		exit(2);
	}
	printf ("Create %s", ctime(&sem_buf.sem_ctime));
	/* Set arg (the union) to the address of the initializing vector */
	arg.array = sem_array;
	if (semctl(sem_id, 0, SETALL, arg) == -1) {
		perror("semctl: SETALL");
		exit(3);
	}
	for (i=0; i<ns; ++i) {
		if ((sem_value = semctl(sem_id, i, GETVAL, 0)) == -1) {
			perror("semctl : GETVAL");
			exit(4);
		}
		printf ("Semaphore %d has value of %d\n",i, sem_value);
	}
	if (strcmp(command,"r")==0){
		if (semctl(sem_id, 0, IPC_RMID, 0) == -1) {
			perror ("semctl: IPC_RMID");
			exit(5);
		}
	}
	return 1;
}
