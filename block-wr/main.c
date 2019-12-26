#include <sys/types.h>
#include <sys/wait.h>


#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <pthread.h>

#define READERS 10
#define WRITERS 1

void* writer(void *);
void* reader(void *);

pthread_mutex_t o_mutex, rmutex;
        
        pthread_t w_tid;
        pthread_t r_tids[READERS];
        pthread_rwlock_t wlock;

        
int main()
{
        char* buf = (char*) malloc(10*sizeof(char));
        
        if(buf)
                memset(buf,'\0',10);
        else
        {
                perror("buf == null");
                return EXIT_FAILURE;
        }




        if(pthread_rwlock_init(&wlock, NULL))
        {
                perror("pthread_rw_init failed\n");
                return EXIT_FAILURE;
        }


        if(pthread_create(&w_tid, NULL, writer, (void*) buf))
        {
                perror("pthread_create failed\n");
                return EXIT_FAILURE;
        }

        int i = 0;
        for(i; i < READERS; i++)
        
        if(pthread_create(&r_tids[i], NULL, reader, (void*) buf))
        {
                perror("pthread_create failed\n");
                return EXIT_FAILURE;
        }
                
        

       pthread_join(w_tid, NULL);

       free( buf );


       pthread_rwlock_destroy(&wlock);
       return EXIT_SUCCESS;
}


void* writer(void *buf)
{
        int inc = 0;
                
        while(1)
        {

               pthread_rwlock_wrlock(&wlock);
               
               printf("WRLOCK RELEASED: WTID: %lld, inc: %d\n", pthread_self(), inc);
               if(snprintf((char*)buf,10,"%d",inc++) <= 0)
                       perror("Can't write chars to buf");

               pthread_rwlock_unlock(&wlock);

               sleep(1);
        }
        
}

void* reader(void *buf){
        
        while(1){
                
                pthread_rwlock_wlock(&wlock);
                
                printf("RDLOCK RELEASED: %s \t TID: %lld\n", (char*) buf, pthread_self());
        
                pthread_rwlock_unlock(&wlock);

                sleep(1);
        }

}

