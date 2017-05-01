#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>


void main(){
	//creat("lock2",0);
	if(access("lock2", F_OK) != -1 && access("lock1", F_OK) != 0){
		creat("lock1", 0);
		unlink("lock2");
	}
}

