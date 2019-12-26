#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include <time.h>
#include <unistd.h>
#include <string.h>

#include <semaphore.h>

#include "def.h"
#define SHMNAME "shared_memory"
#define SHMID 1234
static int id = -1;

static void closeall()
{
        if(id != -1)
        {
                struct shmid_ds shmds;
                
                shmctl(id, IPC_RMID, &shmds);
                printf("atexit\n");
        }

}

const char* formstring(char *, size_t);

////////////// SERVER
int main(int argc, char* argv[]){

//        if(!atexit(closeall))
//                printf("atexit created\n"); 

        //Создали ключ
        key_t key = ftok(SHMF, SHMID);

        if (key == -1)
        {
                printf("%d ", errno);
                perror("!ftok");
                return 1;
        }
        //Создаем Разделяемую память
        id = shmget(key, 1024, IPC_CREAT | IPC_EXCL | 0660);

if(-1 == id)
        switch(errno)
        {
                case EINVAL:
                        {
                                perror("EINVAL");
                                return 1;
                        }
                case EEXIST:
                        {
                                perror("EEXIST");
                                return 1;
                        }
                case ENOSPC:
                        {
                                perror("ENOSPC");
                                return 1;
                        }
                case ENOENT:
                        {
                                perror("ENOENT");
                                return 1;
                        }      
                case EACCES:
                        {
                                perror("EACCES");
                                return 1;
                        }
                case ENOMEM:
                        {
                                perror("ENOMEM");
                                return 1;
                        }
                default:
                        {
                                printf("SHM succesfuly created\n");
                        }
        }
        

        //Подстыковываемся к существующей памяти
        void* shmaddr = shmat(id, NULL, SHM_RND);

        if(-1 == (int) shmaddr)
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
                
                struct shmid_ds shmds;
                shmctl(id, IPC_RMID, &shmds);
                exit(errno);
        }

        ////////////////////////////
                int i = 0;
                struct shmid_ds shmcurrentinfo;

                shmctl(id, IPC_STAT, &shmcurrentinfo);

//                printf("SHM INFO:\n | [MyPid ~ Last shmat PID] <%d ~ %d> | " \
                                " [Size of segment] <%ld> \n" \
                                "[key #] <%d> | [count of attaches] <%d> |\n" ,
  //                              getpid(), shmcurrentinfo.shm_lpid,
    //                            shmcurrentinfo.shm_segsz,  shmcurrentinfo.shm_perm.__key,
      //                          shmcurrentinfo.shm_nattch);
        
                size_t shsize = shmcurrentinfo.shm_segsz;
                

                if(shsize < 30)
                {
                        shmdt(shmaddr);
        
                        struct shmid_ds shmds;
                        shmctl(id, IPC_RMID, &shmds);
                }
                memset((char*)shmaddr, '\0', shsize);

                char buffer[30];
                int f; 
                if(argc >= 2)
                     f = 0;
                else f = 1;

                
                sem_t *sema
                        = sem_open("/saaaas", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IWOTH | S_IROTH, 1);

                if(sema == SEM_FAILED)
                {
                        perror("SEM_FAILED ");
                        struct shmid_ds shmds;
                        shmdt(shmaddr);
                        shmctl(id, IPC_RMID, &shmds);
                        return EXIT_FAILURE;
                }
                

                        int sval;

                        while(f || (i < atoi(argv[1])))
                        {               
                                //printf("%s\n", formstring(buffer, 30));
                               // fflush(stdout);i
                                sem_getvalue(sema, &sval);
                                
                                printf("Waiting for semaphore, current val:%d ...\n", sval);
                                sem_wait(sema);
                        //        sleep(1);
                                        formstring(buffer,30);
                                        memcpy((char*)shmaddr, buffer, 30);
                                sem_post(sema);
                                sleep(1); 
                                // printf("%s", (char*)shmaddr);

                                formstring(buffer, 30);
                                i++;
                        }
        
        struct shmid_ds shmds;
        ///////////////////////////
        shmdt(shmaddr);
        shmctl(id, IPC_RMID, &shmds);

        sem_close(sema);
        sem_unlink("/semensemenich");
        
        return EXIT_SUCCESS;
}



const char* formstring(char* buf, size_t size){
 
        time_t now = time(NULL);

        struct tm *tt;
            tt = localtime(&now);


        strftime(buf, size, "%T", tt);
        sprintf(buf, "%s : %d\0", buf, getpid()); 
        return buf;
}
