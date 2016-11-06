#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>


#define RMAX 10

int main(void){

  key_t key0 = ftok("/tmp",'B');//sem pour writer
  key_t key1 = ftok("/tmp",'C');//sem pour readers
  int shmid,semid0,semid1;

  int *shval;
  struct sembuf checkForWriter={0,0,0},in={0,-1,0},out={0,1,0};
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
    semop(semid0,&checkForWriter,1);
    semop(semid1,&in,1);

    printf("      reader %d: shval=%d\n",getpid(),*shval);

    semop(semid1,&out,1);
    sleep(rand()%3);
  }

  return EXIT_SUCCESS;
}
