#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>

//Prototype
void set_sembuf_struct(struct sembuf *s, int semnum, int semop, int semflg);

int main(int argc, char *argv[]){

	int n, k, i, sem, num, op, flg, semop_ret, status, sem_id;
	char buffer[10000], *ptr;
	pid_t childPid;
	pid_t parentPid = getpid();
	key_t ipc_key = ftok(".",'S');
	struct semid_ds sem_buf;
	ptr = buffer;
	
	//Input validation
	if(argc != 4){
		printf("Error. The number of arguments must be 4.\n");
		exit(1);
	}
	if(atoi(argv[1]) < 0){
		printf("Error. Argument 1 must be a integer greater than 0.\n");
		exit(1);
	}
	else{
		n = atoi(argv[1]);
	}
	if(strcmp(argv[2],"n") == 0){
		//set semaphore creation to false		
		sem = 0;
	}
	else if(strcmp(argv[2],"s") == 0){
		//set semaphore creation to true		
		sem = 1;
	}
	else{
		printf("Error. Argument 2 must be either 'n' or 's'.\n");
		exit(1); 
	}
	if(atoi(argv[3]) < 0){
		printf("Error. Argument 3 must be an integer greater than 0.\n");
		exit(1); 
	}
	else{
		k = atoi(argv[3]);
	}
	//implement wait() and signal()
	struct sembuf semwait[1];
	struct sembuf semsignal[1];
	//initialize semwait and semsignal	
	set_sembuf_struct(semwait, 0, -1, 0);
	set_sembuf_struct(semsignal, 0, 1, 0);

	if(sem){
		//create the semaphore               
		if((sem_id = semget(ipc_key, 1, IPC_CREAT | 0666| IPC_EXCL)) == -1){ 
        		perror ("semget: IPC | 0666"); 
                	exit(1); 
        	}
		//increment semaphore
		if(semop(sem_id, semsignal, 1) == -1){
			printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), 				strerror(errno));
			if (semctl(sem_id, 0, IPC_RMID) == -1){
				printf ("%ld: could not delete semaphore - %s\n", 					(long)getpid(), strerror(errno));
				exit(1);
			}
		} 
	}
	//create child processes
	for(i = 1; i < n; i++){
		childPid = fork();
		if(childPid < 0){
			printf("Fork failed.");
		}
		else if(childPid > 0){
			break;
		}
		else{}
	}
	//decrement the semaphore and enter the critical section
	if(sem){
		while((semop_ret = (semop(sem_id, semwait, 1)) == -1) && (errno ==EINTR));
		if (semop_ret == -1){
			printf ("%ld: semaphore decrement failed - %s\n", (long)getpid(), 				strerror(errno));
		}
	}
	//child process writes its child number, its PID, its parent PID, and its child PID to 		//the buffer
	sprintf(buffer,"i: %d: process ID: %6ld parent ID: %6ld child ID: %6ld\n", i,  		(long)getpid(), (long)getppid(), (long)childPid);
	//output each character in the buffer
	while(*ptr != NULL){
                fputc(*ptr, stderr);
                usleep(k);
                ptr++;
	}
	//increment semaphore and exit the critical section
	if(sem){
		while (((semop_ret = semop(sem_id, semsignal, 1)) == -1) && (errno == EINTR)); 			if (semop_ret == -1){
			printf ("%ld: semaphore increment failed - %s\n", (long)getpid(), 				strerror(errno));
		}
	}
	//Parent process wait for child process to finish execution
	waitpid(childPid, &status, 0);
	//Parent process remove semaphore
	if(sem && parentPid == getpid()){
		if (semctl(sem_id, 0, IPC_RMID, 0) == -1)
		{
			perror ("semctl: IPC_RMID");
			exit(1);
		}
	}
	exit(0);
}

void set_sembuf_struct(struct sembuf *s, int num, int op, int flg){
	s -> sem_num = (short)num;
	s -> sem_op = op;
	s -> sem_flg = flg;
	return;
}

