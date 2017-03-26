#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>


int main(void){

  key_t key0 = ftok("/tmp",'B');
  key_t key1 = ftok("/tmp",'C');
  int shmid = shmget(key0,4,0666),semid0 = semget(key0,1,0666),semid1=semget(key1,1,0666);
  semctl(semid0,IPC_RMID,NULL);
  semctl(semid1,IPC_RMID,NULL);
  shmctl(shmid,IPC_RMID,NULL);



  return EXIT_SUCCESS;
}
