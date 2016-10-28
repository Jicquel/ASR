#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define L 15

pthread_mutex_t m;
pthread_cond_t noplaces=PTHREAD_COND_INITIALIZER, nobonbons=PTHREAD_COND_INITIALIZER;

int places=L;
int bonbons=0;

void * prod(void* arg){
  while(1){
    while(places<1)
      pthread_cond_wait(&noplaces,&m);
    places--;
    bonbons++;

    printf("producer (%d) -- put a candy -- %d\n",getpid(),bonbons);
    sleep(rand()%3);

    pthread_cond_signal(&nobonbons);
  }
}


void * cons(void* arg){
  while(1){
    while(bonbons<1)
      pthread_cond_wait(&nobonbons,&m);

    bonbons--;
    places++;

    printf("  cons (%d) -- get a candy -- %d\n",getpid(),bonbons);
    sleep(rand()%3);

    pthread_cond_signal(&noplaces);
  }
}


int main(void){

  pthread_t conso[2];
  pthread_t produ[2];
  int i;

  for(i=0;i<2;i++){
    pthread_create(&produ[i],NULL,prod,NULL);
    pthread_create(&conso[i],NULL,cons,NULL);
  }
  while(1){
    sleep(2);
  }
}
