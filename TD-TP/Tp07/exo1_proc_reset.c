#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>


#define VMAX 5

int main(void){

  key_t key = ftok("/tmp",'A');
  int shmid,semid = semget(key,2,0666);
  int *x;
  struct sembuf reset_in={1,-1,0},reset_out={-VMAX,1};
  shmid = shmget(key,4,0666);


  x=shmat(shmid,NULL,0);

  if(semid==-1){
    perror("semget");
    exit(-1);
  }

  while(1){
    semop(semid,&reset_in,1);
    printf("reset : before x=%d\n",*x);
    *x=0;
    printf("        after x=%d\n",*x);

    semop(semid,&reset_out,2);
  }

  return EXIT_SUCCESS;
}
