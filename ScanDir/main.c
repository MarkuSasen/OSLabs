#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pwd.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"

enum PERM {
	r,
	w,
	x
};

	
int main(){

	   struct dirent **scanned;
           struct stat filestat;
	   int n;

           n = scandir(".", &scanned, NULL, alphasort);
           
	   if(n < 0)
	   {
		perror("scandir err"); 
		exit(errno);
	   }

		
	while(n--)
	{
		if(!stat(scanned[n]->d_name, &filestat));

		else	printf("err : %d\n",errno);
		
		if(!(filestat.st_mode & (S_IFMT | S_IFDIR))){
			free(scanned[n]);
			continue;
		}
			
		char permissions[12] = {};
		memset(permissions,'-',sizeof(char)*12);
		permissions[3] = permissions[7] = permissions[11] = ' ';
//			printf("%o\n", filestat.st_mode);	
		

				if(filestat.st_mode & S_IRUSR){	
					permissions[0] = 'r';
				}if(filestat.st_mode & S_IWUSR){
					permissions[1] = 'w';
				}if(filestat.st_mode & S_IXUSR)
					permissions[2] = 'x';
				


				if(filestat.st_mode & S_IRGRP){	
					permissions[4] = 'r';
				}if(filestat.st_mode & S_IWGRP){
					permissions[5] = 'w';
				}if(filestat.st_mode & S_IXGRP)
					permissions[6] = 'x';
				
				
				if(filestat.st_mode & S_IROTH){	
					permissions[8] = 'r';
				}if(filestat.st_mode & S_IWOTH){
					permissions[9] = 'w';
				}if(filestat.st_mode & S_IXOTH)
					permissions[10] = 'x';
				

		permissions[11] = '\0';

		char *mtime =  ctime(&filestat.st_ctime);
		*strrchr(mtime,'\n') = ' ';	
		
		struct passwd *pwd;
		pwd = getpwuid(filestat.st_uid);
		char *isdir = (char*) malloc(255*sizeof(char));
		memset(isdir,'\0',sizeof(char));
		strcat((char*)isdir,((filestat.st_mode & S_IFDIR) ? ANSI_COLOR_BLUE 
		:
		 (permissions[2] == 'x' || permissions[6] == 'x' || permissions[10] == 'x') ? ANSI_COLOR_RED 
		:
		ANSI_COLOR_YELLOW));
		
		printf("%s %s %s %s" ANSI_COLOR_RESET "\n", permissions, pwd ? pwd -> pw_name : "" , mtime, strcat((char*)isdir,scanned[n]->d_name));
//		printf("%s ", scanned[n]->d_name);
	
		
		free(scanned[n]);
	}

	free(scanned);

	return 0;
}
