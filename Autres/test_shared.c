#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){

key_t key = ftok(".",'r');
int *x;
int shmid = shmget(key,4*sizeof(int),0666|IPC_CREAT);

if (shmid<0){
  perror("shmid");
  exit(0);
}

x= (int*) shmat(shmid,NULL,0);
if((int)x==-1){
perror("shmat");
exit(-1);
}

printf("%p\n",x);
(*x)=2;
printf("%d\n",*x);

return EXIT_SUCCESS;
}
