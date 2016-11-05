#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>


#define RMAX 10

int main(void){

  key_t key0 = ftok("/tmp",'B');
  key_t key1 = ftok("/tmp",'C');
  int shmid,semid0,semid1;
  int* shval;

  union semun{int val; struct semid_ds *buf;unsigned short *array; struct seminfo *__buf;} arg;//indispensable pour semctl

  semid0 = semget(key0,1,0666|IPC_CREAT);
  if(semid0==-1){
    perror("semid");
    exit(0);
  }
  semid1 = semget(key1,1,0666|IPC_CREAT);
  if(semid1==-1){
    perror("semid");
    exit(0);
  }
  shmid = shmget(key0,4,0666|IPC_CREAT);

  shval=(int*)shmat(shmid,NULL,0);

  if((int)shval==-1){
    perror("shmat");
    exit(0);
  }

  (*shval)=0;

  arg.val=0;
  if(semctl(semid0,0,SETVAL,arg)==-1){//writer bloque l'entrée de reader (0 ok, 1 bloqué).
    perror("semctl");
    exit(-1);
  }

  arg.val=RMAX;
  if(semctl(semid1,0,SETVAL,arg)==-1){//nombre de readers
    perror("semctl");
    exit(-1);
  }

  return EXIT_SUCCESS;
}
