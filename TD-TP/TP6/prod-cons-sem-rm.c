#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>


#define L 15
int main(void){

  key_t key = ftok("/tmp",'A');
  int semid = semget(key,3,0);
  shmctl(semid,IPC_RMID,NULL);



  return EXIT_SUCCESS;
}
