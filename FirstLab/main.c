#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>


int zero = 0;

void pid_exiting()
{
	printf("\nONEXIT: <%d> died \n", getpid());
};

void child_pid_exiting()
{
	printf("\nONEXIT: <%d> died, his parent [%d]\n", getpid(), getppid());
};


void pidat_exit()
{
	printf("\nATEXIT: <%d> died\n",getpid());
};


void pidat_exitchild()
{
	printf("\nATEXIT: <%d> died, parent [%d] -> trying to divide by a zero...\n",getpid(),getppid());
	int no = 5/zero;
	printf("divided!\n\n");
};


int main()
{
	pid_t pid;
	int stat;	

	on_exit(pid_exiting,NULL);
	atexit(pidat_exit);
	pid = fork();
	
	switch(pid)
	{

		case -1 : exit(1);

		case 0 : printf("I am %d and %d is my parent\n",getpid(),getppid());
			on_exit(child_pid_exiting,NULL);
			return 0;
		default : 
			printf("I am the first Procces! %d. When my first child [%d] will die, I ll continiue my work\n ",getpid(), pid);
			waitpid(pid,&stat,0);			
			printf("Oh god, he died! Guess, I might create new one...\n");

			pid = fork();

			if(pid > 0)
			{	
				printf("waitting for %d\n",pid);
				pid = waitpid(pid, &stat, 0);
			
				printf("Finally, my second child died. I am [%d] and I was pleased to see [%d] agony\n",getpid(), pid);
			} 

			else if(pid == -1)
				printf("err %s\n", strerror(errno));			
			else {
				atexit(pidat_exitchild);
				return 0;
				}
	}


	printf("\n\n\n dividing by a zero, then, creating some new forks\n");



//	int a = 5/zero;

	pid = fork();
	if(!pid) atexit(pidat_exitchild);
	

	switch(pid)
	{
		case -1 : exit(1);
		case 0 : printf("<%d>, parent is [%d], dividing by a zero...\n",getpid(),getppid());
		atexit(pidat_exit);
		on_exit(child_pid_exiting,NULL);
		int yes = -1 / zero;
		break;
		
		default : 
			waitpid(pid,&stat,0);
			printf("%d is a parent of %d\n", getpid(), pid);
			break;

	}




	return 0;
}
