#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>
#include <string.h>

#include <semaphore.h>

#include "def.h"

#define SHMNAME "shared_memory"
#define SHMID 1234



int main(int argc, char*argv[])
{

        key_t key = ftok(SHMF, SHMID);

        if(key == -1)
        {
                printf("%d ", errno);
                perror("!ftok");
                return 1;
        }

        int id = shmget(key,1024,0);  
        void* shmaddr = shmat(id, NULL, SHM_RND);

        if(-1 == shmaddr)
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
                return 1;
        }


        
        char buffer[30];

        sem_t *sema = sem_open("/semich", 0);

        if(sema == SEM_FAILED)
        {
                        perror("CLIENT: SEM_FAILED ");
                        return EXIT_FAILURE;
        }

        while(1)
        {
              
                       sem_wait(sema);
                       
                       memccpy(buffer, (char*) shmaddr, '\0', 30);
                       memset((char*) shmaddr, '\0', 30);
        
                       sem_post(sema);

                       printf("%s\n mypid : %d\n", buffer, getpid());
                       sleep(1);
        }



        shmdt(shmaddr);
        
        sem_close(sema); 
        return EXIT_SUCCESS;
}
