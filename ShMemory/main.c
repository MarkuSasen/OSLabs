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

        if(!atexit(closeall))
                printf("atexit created\n"); 

        //Создали ключ
        key_t key = ftok(SHMNAME, SHMID);

        if (key == -1)
        {
                printf("%d ", errno);
                perror("!ftok");
                return 1;
        }
        //Создаем Разделяемую память
        id = shmget(key, 1024, IPC_CREAT | 0660);

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
                
                struct shmid_ds shmds;
                shmctl(id, IPC_RMID, &shmds);
                exit(errno);
        }

        ////////////////////////////
                int i = 0;
                struct shmid_ds shmcurrentinfo;

                shmctl(id, IPC_STAT, &shmcurrentinfo);

                printf("SHM INFO:\n | [MyPid ~ Last shmat PID] <%d ~ %d> | " \
                                " [Size of segment] <%ld> \n" \
                                "[key #] <%d> | [count of attaches] <%d> |\n" ,
                                getpid(), shmcurrentinfo.shm_lpid,
                                shmcurrentinfo.shm_segsz,  shmcurrentinfo.shm_perm._key,
                                shmcurrentinfo.shm_nattch);
        
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
                        while(f || (i < atoi(argv[1])))
                        {               
                                //printf("%s\n", formstring(buffer, 30));
                               // fflush(stdout);
                                
                                if(*((char*)shmaddr) == '\0')
                                {
                                        formstring(buffer,30);

                                        memcpy((char*)shmaddr, buffer, 30);
                                }
                                // printf("%s", (char*)shmaddr);

                                printf("Waiting for memory to be clear\n");
                                sleep(3);
                                ++i;
                        }
        
        
        ///////////////////////////
        shmdt(shmaddr);
        
       
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
