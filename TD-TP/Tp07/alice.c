#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){

  key_t key = ftok("/tmp",'A');
  int semid = semget(key,2,0666);
  struct sembuf in={0,-1,0},begin={1,1,0},end[2]={{0,1,0},{1,-1,0}},out={1,0,0};



  if(semid==-1){
    perror("semget");
    exit(-1);
  }

    sleep(rand()%3);
    semop(semid,&in,1);
    printf("I'm alice. Who is here ?\n");
    semop(semid,&begin,1);
    printf("Hi!\n");
    semop(semid,end,2);
    printf("oh-oh-oh-...!\n");
    semop(semid,&out,1);
    printf("Bye ! ");

  return EXIT_SUCCESS;
}
