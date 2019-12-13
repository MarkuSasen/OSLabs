#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>
#include <string.h>



#define SHMNAME "shared_memory"
#define SHMID 1234



int main(int argc, char*argv[])
{

        key_t key = ftok(SHMNAME, SHMID);

        if(key == -1)
        {
                printf("%d ", errno);
                perror("!ftok");
                return 1;
        }

        int id = shmget(key,1024,0);  
        void* shmaddr = shmat(id, NULL, SHM_RND);

        if(-1 == ((int*) shmaddr))
        {
                switch(errno)
                {
                        case EACCES:
                                {
                                        perror("shmat EACCES"); break;
                                }
                        case EIDRM:
                                {
                                        perror("shmat EIDRM"); break;
                                }
                        case EINVAL:
                                {
                                        perror("shmat EINVAL"); break;
                                }
                        case ENOMEM:
                                {
                                        perror("shmat ENOMEM"); break;
                                }
                }

        }


        
        char buffer[30];
        while(1)
        {
                if(*((char*) shmaddr) != '\0')
                {        
                        memccpy(buffer, (char*) shmaddr, '\0', 30);
                         
                        memset((char*) shmaddr, '\0', 30);

                        printf("%s\n mypid : %d\n", buffer, getpid());
                }

//                printf("waiting for se\n");
                
                sleep(3);
        }



        shmdt(shmaddr);
        
         
        return EXIT_SUCCESS;
}
