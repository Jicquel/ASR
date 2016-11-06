#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

#define NBRE_PERSONNE 2

int main(void){

  key_t key = ftok("/tmp",'A');
  int semid;

  union semun{int val; struct semid_ds *buf;unsigned short *array; struct seminfo *__buf;} arg;//indispensable pour semctl

  semid= semget(key,2,0666|IPC_CREAT);
  if(semid==-1){
    perror("semid");
    exit(0);
  }

  arg.val=NBRE_PERSONNE;
  if(semctl(semid,0,SETVAL,arg)==-1){//nbre de personnes attendues pour in
    perror("semctl");
    exit(-1);
  }

  arg.val=0;
  if(semctl(semid,1,SETVAL,arg)==-1){//nbre de personne attendus pour out
    perror("semctl");
    exit(-1);
  }

  return EXIT_SUCCESS;
}
