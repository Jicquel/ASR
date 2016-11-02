#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){

  key_t key = ftok("/tmp",'A');
  int * x;
  int shmid, semid = semget(key,2,0666);
  struct sembuf writer_in={1,-1,0},writer_out[2]={{0,1,1},{1,1,0}};

  shmid = shmget(key,4,0666);

  x=shmat(shmid,NULL,0);

  if(semid==-1){
    perror("semget");
    exit(-1);
  }
  while(1){
    semop(semid,&writer_in,1);
    printf("reset : before x=%d\n",*x);
   *x=(*x)+1;
    printf("        after x=%d\n",*x);

    semop(semid,writer_out,2);
  }

  return EXIT_SUCCESS;
}
