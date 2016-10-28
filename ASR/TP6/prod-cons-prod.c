#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>


#define L 15
int main(void){

  key_t key = ftok("/tmp",'A');
  int semid = semget(key,3,0666);
  struct sembuf prod_in[2]={{0,-1,0},{1,-1,0}},prod_out[2]={{1,1,0},{2,1,0}};



  if(semid==-1){
    perror("semget");
    exit(-1);
  }
  while(1){
    if(semop(semid,prod_in,2)<0){
      perror("semget -- producer");
      exit(-1);
    }
    if(semop(semid,prod_out,2)<0){
      perror("semget -- producer");
      exit(-1);
    } 
    printf("producer (%d) -- put a candy -- %d\n",getpid(),semctl(semid,2,GETVAL));
    sleep(rand()%3);
  }

  return EXIT_SUCCESS;
}
