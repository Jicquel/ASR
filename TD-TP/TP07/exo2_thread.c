#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NBRE_MAX 5
pthread_mutex_t m;
pthread_cond_t cond_reset=PTHREAD_COND_INITIALIZER;

int x=0;

void * writer(void* arg){
  int numWriter = *(int*) arg;

  while(1){
    pthread_mutex_lock(&m);
    printf("   incr%d: before x=%d\n",numWriter,x);
    x+=1;
    printf("            after x=%d\n",x);
    pthread_mutex_unlock(&m);
    sleep(rand()%3);
    pthread_cond_signal(&cond_reset);
  }
}


void * controleur(void* arg){
  while(1){
    pthread_mutex_lock(&m);
    while(x<NBRE_MAX)
      pthread_cond_wait(&cond_reset,&m);

    printf(" reset: before x=%d\n",x);
    x=0;
    printf("         after x=%d\n",x);

    pthread_mutex_unlock(&m);
  }
}


int main(void){

  pthread_t writ[5];
  pthread_t control[2];
  int i,numWriter;

  for(i=0;i<2;i++)
    pthread_create(&control[i],NULL,controleur,NULL);

  for(i=0;i<5;i++){
    numWriter = i+1;
    pthread_create(&writ[i],NULL,writer,(void*) &numWriter);
  }
  while(1){
    sleep(10);
  }
}
