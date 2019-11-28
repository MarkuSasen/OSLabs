#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include <time.h>
#include <signal.h>

int main(){
	
	int fd;
	pid_t pid;
	char *fifo = "/tmp/fif_1";
	if(mkfifo(fifo, 0666) == -1)
	{
		perror("Fifo error\n"); 
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
			char buf[30]; int len = 0;
			timespec_get(&ts, TIME_UTC);
			tt = localtime(&ts.tv_sec);
			
			strftime(buf, sizeof(buf), "%T", tt);
			sprintf(buf, "%s.%ld", buf, ts.tv_nsec / 1000000);
	

			fd = open(fifo, O_WRONLY);			
			len = write(fd,buf,30);
			
			memset(buf,'\0',30);
				
			close(fd);
			if(len > 0)
			{
				printf("Children await\n");
				kill(getpid(),SIGSTOP); 
			}

			fd = open(fifo,O_RDONLY);		
			len = read(fd, buf, 30);
			if(len > 0)	
				printf("PARENT TIME: %s\n", buf);
			else perror("No response from parent\n");
	
			close(fd);

			unlink(fifo);
			exit(0);
		}
		default:
		{
			int len = 0;
			char buf[30];
			
			fd = open(fifo,O_RDONLY);

			len = read(fd,buf,30);

		if(len > 0)					
			printf("CHILD TIME: %s\n", buf);
		else perror("No response from children\n");

			memset(buf,'\0',30);
				sleep(1);

	
			close(fd);
		
				timespec_get(&ts, TIME_UTC);

				tt = localtime(&ts.tv_sec);

				
			strftime(buf, sizeof(buf), "%T", tt);
			sprintf(buf, "%s.%ld", buf, ts.tv_nsec / 1000000);
	
			sleep(1);	
			kill(pid, SIGCONT); 

			fd = open(fifo, O_WRONLY);			
			
				len = write(fd,buf,30);
				printf("Writing... \n");
			

	
				close(fd);

			printf("Closing p\n");
			unlink(fifo);
			exit(0);
		}
	}

	
	return 0;
}
