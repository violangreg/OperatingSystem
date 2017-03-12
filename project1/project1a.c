pid_t _fork()
{
	pid_t pid;
	if ((pid = fork()) == -1) 
	{
		printf("\n The fork failed %s \n", (int)getpid());
		exit(1);
	}
	return pid;
}

void CreateTree(int level)
{
	pid_t right_child, left_child;	
	
	if(level == 0) return;	
	left_child = fork();
	
	if(left_child != 0)
	{
		right_child = fork();
		if(right_child == 0) 
		{
			CreateTree(level - 1);
		}	
	}
	else
	{
		
		CreateTree(level - 1);		
	}
	printf("\n level:%d processID:%6ld parentID:%6ld childID_1:%6ld childID_2:%6ld \n", level , (long)getpid(), (long)getppid(), (long)left_child, (long)right_child);


}

void ProcessTree(int level)
{
	if(level <= 0) return;
	switch(fork())
	{
		case 0:
			ProcessTree(level - 1);
			break;
		case -1:
			printf("\n fork failed %s\n", (long)getpid());
			break;
		default:
			break;
	}
}
