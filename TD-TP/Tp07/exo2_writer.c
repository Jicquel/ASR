#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define RMAX 10

int main(void){
  key_t key0 = ftok("/tmp",'B');
  key_t key1 = ftok("/tmp",'C');
  int shmid,semid0,semid1;

  int *shval;
  struct sembuf block={0,1,0},unblock={0,-1,0},in={0,-RMAX,0},out={0,RMAX,0};
  shmid = shmget(key0,4,0666);

  semid0= semget(key0,1,0666);
  if(semid0==-1){
    perror("semget");
    exit(-1);
  }

  semid1= semget(key1,1,0666);
  if(semid1==-1){
    perror("semget");
    exit(-1);
  }

  shval = (int*) shmat(shmid,NULL,0);
  if((int)shval==-1){
    perror("shmat");
    exit(0);
  }

  while(1){
    sleep(1);
    semop(semid0,&block,1);
    semop(semid1,&in,1);
    (*shval)+=2;

    printf("  writer   %d: shval=%d",getpid(),*shval);

    semop(semid1,&out,1);
    semop(semid0,&unblock,1);
  }

  return EXIT_SUCCESS;
}
