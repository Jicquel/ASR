#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){

  key_t key = ftok("/tmp",'A');
  int *x=NULL;
  int shmid,semid;

  union semun{int val; struct semid_ds *buf;unsigned short *array; struct seminfo *__buf;} arg;//indispensable pour semctl

  semid= semget(key,2,0666|IPC_CREAT);
  if(semid==-1){
    perror("semid");
    exit(0);
  }
  shmid = shmget(key,4,0666|IPC_CREAT);

  x=(int*)shmat(shmid,NULL,0);
  if((int)x==-1){
    perror("shmat");
    exit(0);
  }

  (*x)=0;

  arg.val=0;
  if(semctl(semid,0,SETVAL,arg)==-1){//valeur de x pour reset
    perror("semctl");
    exit(-1);
  }

  arg.val=1;
  if(semctl(semid,1,SETVAL,arg)==-1){//x occupé ou pas
    perror("semctl");
    exit(-1);
  }

  return EXIT_SUCCESS;
}
