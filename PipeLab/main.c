#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

int main(){
	
	int fd[2];
	pid_t pid;

	if(pipe(fd) == -1)
	{
		perror("Pipe error\n"); 
		return 1;
	}

	
	pid = fork();


		struct timespec ts;

		struct tm *tt;

	switch(pid)
	{
		case -1:
			perror("fork error\n"); return 1;
		case 0:
		{
			char buf[30]; char milis[10]; int len = 0;
			timespec_get(&ts, TIME_UTC);
			tt = localtime(&ts.tv_sec);
			
			strftime(buf, sizeof(buf), "%T", tt);
			sprintf(buf, "%s.%ld", buf, ts.tv_nsec / 1000000);
	
				
			
			len = write(fd[1],buf,30);
			
			memset(buf,'\0',30);
				
			
			if(len > 0)
			{
				printf("Children await\n");
				kill(getpid(),SIGSTOP); 
			}

			
			len = read(fd[0], buf, 30);
			if(len > 0)	
				printf("PARENT TIME: %s\n", buf);
			else perror("No response from parent\n");
	
			close(fd[1]);
			close(fd[0]);

			exit(0);
		}
		default:
		{
			int len = 0;
			char buf[30];
			
				
			len = read(fd[0],buf,30);

		if(len > 0)					
			printf("CHILD TIME: %s\n", buf);
		else perror("No response from children\n");

			memset(buf,'\0',30);
				sleep(1);
	
				timespec_get(&ts, TIME_UTC);

				tt = localtime(&ts.tv_sec);

				
			strftime(buf, sizeof(buf), "%T", tt);
			sprintf(buf, "%s.%ld", buf, ts.tv_nsec / 1000000);
				
				len = write(fd[1],buf,30);
				printf("Writing... \n");
			
			sleep(1);	
			kill(pid, SIGCONT); 
	

			close(fd[0]);
			close(fd[1]);
			printf("Closing p\n");
			exit(0);
		}
	}

	
	return 0;
}
