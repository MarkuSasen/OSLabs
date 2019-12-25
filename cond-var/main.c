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
        pthread_cond_t wlock, rlock;

int readers = 0;
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




        if(pthread_cond_init(&wlock, NULL))
        {
                perror("pthread_cond_init failed\n");
                return EXIT_FAILURE;
        }
        
        if(pthread_cond_init(&rlock, NULL))
        {
                perror("pthread_cond_init failed\n");
                return EXIT_FAILURE;
        }

        if(pthread_mutex_init(&o_mutex, NULL))
        {
                perror("pthread_mu_init failed\n");
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


       pthread_mutex_destroy(&o_mutex);
       pthread_cond_destroy(&wlock);
       pthread_cond_destroy(&rlock);

       return EXIT_SUCCESS;
}
#define TS 1 
int w = 0, acc = 0;
void* writer(void *buf)
{
        int inc = 0;
                
        while(1)
        {
               pthread_mutex_lock(&o_mutex);
               while(readers > 0 || w == 1)
                       pthread_cond_wait(&wlock, &o_mutex);
               
               printf("WTID: %lld, inc: %d\n", pthread_self(), inc);
               if(snprintf((char*)buf,10,"%d",inc++) <= 0)
                       perror("Can't write chars to buf");

               w = 1;
               pthread_mutex_unlock(&o_mutex);
               pthread_cond_broadcast(&rlock);

               sleep(TS);
        }
        
}

void* reader(void *buf){
        
        while(1){
                
                pthread_mutex_lock(&o_mutex);
                        while(w != 1 || acc == 10)
                              pthread_cond_wait(&rlock, &o_mutex);
                        acc++;
                        readers++;
                     //   printf("READERS: %d", readers);
                pthread_mutex_unlock(&o_mutex);                       
        

                printf("%s \t TID: %lld\n", (char*) buf, pthread_self());
               
                pthread_mutex_lock(&o_mutex);
                        readers--;
                        if(readers == 0 && acc == 10)
                        {
                                pthread_cond_signal(&wlock); w = 0; acc = 0;
                        }
                pthread_mutex_unlock(&o_mutex);

//                sleep(TS);
        }

}

