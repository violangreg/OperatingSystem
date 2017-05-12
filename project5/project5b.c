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
void set_sembuf_struct(struct sembuf *s, int num, int op, int flg)
{
	s-> sem_num = (short) num;
	s-> sem_op = op;
	s-> sem_flg = flg;
	return;
};
union semun {
	int val; 
	struct semid_ds *buf;
	ushort *array;
};
void main(int argc, char *argv[]){ 
	int i=0;
	int status; //c
	int pCount=0;
	int semop_ret; //c
	char buffer[MAX_CANON]; //c
	pid_t pid,w;	//c
	int nChild; //c
	char *op; // c
	int k; // c
	int sem_id; // c					//variable to store semaphore id 
	int sem_value;				//variable to store value of semaphore
	key_t ipc_key; 				//store a key generated from ftok()
	struct semid_ds sem_buf; 	//create a storage location for
	union semun arg;
	/*checking condition for argument*/
	if (argc!=4){
		printf("\n Usage: Need 4 arguments to work");
		exit(1);
	}
	nChild=atoi(argv[1]); // c
	op=argv[2];
	k=atoi(argv[3]);
	if (nChild<1){
		printf("Number of child can't be less than 1");
		exit(1);
	}
	if ((strcmp(op,"s")!=0) && (strcmp(op,"n")!=0)){
		printf("Wrong option(either 's' or 'n')\n");
		exit(1);
	}
	if (!(k>0)){
		printf("Delay has to be non-zero positive integer");
		exit(1);
	}
	ipc_key = ftok(".", 's');
	/* Create semaphore */
	if ((sem_id = semget(ipc_key,1,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH|IPC_CREAT | IPC_EXCL)) == -1) {
			perror ("Semget: ");
			exit(1);
	}
	printf ("Semaphore identifier %d\n", sem_id);
	/*setting up for semaphore*/
	struct sembuf semwait[1];
	struct sembuf semsignal[1];
	/* Initialize semaphore element to 1 */
	set_sembuf_struct(semwait, 0, -1, 0);
	set_sembuf_struct(semsignal, 0, 1, 0);
	if (semop(sem_id, semsignal, 1) == -1) {
		printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
		if (semctl(sem_id, 0, IPC_RMID) == -1){
			printf ("%ld: could not delete semaphore - %s\n", (long)getpid(), strerror(errno));
			exit(1);
		}
	}	
	/*creating child section*/
	for (i=0;i<nChild;i++){
		if (i<nChild-1){
			if((pid=fork())!=0){
				sprintf(buffer,"i:%d process ID: %d parent ID: %d child ID: %d\n",(i+1),getpid(),getppid(),pid);
				break;
			}
		}else{
			sprintf(buffer,"i:%d process ID: %d parent ID: %d child ID: %d\n",i,getpid(),getppid(),pid);
		}
	}
	/*checking to see either create critical section or not*/
	if ((strcmp(op,"s")==0)){
		/*Enrty of critical section*/
		while (( (semop_ret = semop(sem_id, semwait, 1) ) == -1) && (errno ==EINTR));
		if (semop_ret == -1){
			printf ("%ld: semaphore decrement failed - %s\n", (long)getpid(), strerror(errno));
		}else{
			/*writing the output*/
			for (int count=0;buffer[count]!='\0';count++){
				putchar(buffer[count]);
				fflush(stdout);
			}
		}	
		/*Exiting critical section*/
		while (((semop_ret = semop(sem_id, semsignal, 1)) == -1) && (errno == EINTR));
		if (semop_ret == -1){
			printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), strerror(errno));
		}
	}else{
		/*writing the output*/
		for (int count=0;buffer[count]!='\0';count++){
			putchar(buffer[count]);
			fflush(stdout);
			usleep(k);
		}
	}
	while ((w = wait(&status)) && w != - 1);
	printf("%d",i);
	if ((strcmp(op,"s")==0) && i==nChild){
		if (semctl(sem_id, 0, IPC_RMID, 0) == -1) {
			perror ("semctl: IPC_RMID");
			exit(5);
		}
	}
	/*end of the program*/
	exit(2);
}
