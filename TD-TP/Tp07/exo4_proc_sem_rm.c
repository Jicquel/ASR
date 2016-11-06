#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>


int main(void){

  key_t key = ftok("/tmp",'A');
  int shmid = shmget(key,4,0666),semid = semget(key,2,0666);
  semctl(semid,IPC_RMID,NULL);
  shmctl(shmid,IPC_RMID,NULL);



  return EXIT_SUCCESS;
}
