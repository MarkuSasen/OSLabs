#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
//#include <signal.h>

int main(){
	
	int fd[2];
	pid_t pid;

	if(pipe(fd) == -1)
	{
		perror("Pipe error\n"); 
		return 1;
	}

	
	pid = fork();

	switch(pid)
	{
		case -1:
			perror("fork error\n"); return 1;
		case 0:
		{
			char buf[1024]; int writ = 0, len = 0;
			memset(buf,'*',1023); buf[1023] = '\0';

			close(fd[0]);

			writ = write(fd[1], buf, 1024);
			
			printf("written %d chars with error %d\n",writ, errno);
//			kill(getpid(),9);
			close(fd[1]);
			exit(0);
		}
		default:
		{
			int red = 0, len = 0;
			char buf[1024];
			int status;
			close(fd[1]);
			waitpid(pid,&status,0);
			if(WIFEXITED(status))
			{
				while((len = read(fd[0],buf,1024)) > 0)
					red += len;
				
				buf[10] = '\0';	
				printf("reaadden \"%s\" string %d chars with error %d\n",buf,red,errno);
			} else 
				perror("Child terminated with errors");

			close(fd[0]);
			exit(0);
		}
	}

	
	return 0;
}
