#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>


int main()
{
	pid_t pid;
	int stat;	

	pid = fork();
	
	switch(pid){

		case -1 : exit(1);

		case 0 : printf("I am %d and %d is my parent\n",getpid(),getppid()); break;

		default : {
			 printf("I am the first Procces! %d\n",getpid());
			waitpid(pid,&stat,0);	
			pid = fork();
			if(pid > 0)
			{	printf("waitting for %d\n",pid);
				pid = waitpid(pid, &stat, 0);
			
				printf("Please, continiue running. I am %d and I m tired awaiting the %d\n",pid, getpid());
			}

			else if(pid == -1)
				printf("err %s\n", strerror(errno));
			if(pid != 0)
				printf("%d terminated\n",pid);
				break;
			}
	}


	return EXIT_SUCCESS;
}
