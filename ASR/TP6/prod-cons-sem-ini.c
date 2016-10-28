#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>


#define L 15
int main(void){

  key_t key = ftok("/tmp",'A');
  int semid = semget(key,3,0666|IPC_CREAT);
  union semun{int val; struct semid_ds *buf;unsigned short *array; struct seminfo *__buf;} arg;//indispensable pour semctl

  
  if(semid==-1){
    printf("%d",semid);
    perror("semget");
    exit(-1);
  }

  arg.val=L;
  if(semctl(semid,0,SETVAL,arg)==-1){//Producteur
    perror("semctl");
    exit(-1);
  }

  arg.val=1;
  if(semctl(semid,1,SETVAL,arg)==-1){//Vitrine
    perror("semctl");
    exit(-1);
  }

  arg.val=0;
  if(semctl(semid,2,SETVAL,arg)==-1){//Consommateur
    perror("semctl");
    exit(-1);
  }


   return EXIT_SUCCESS;
}
