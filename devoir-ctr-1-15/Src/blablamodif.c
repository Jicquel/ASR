#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


void chldhand(int s){
    printf("Not ready!");
    signal(SIGALRM,SIG_DFL);
}
int main(int argc, char *argv[]) {
  struct sigaction sa;
  sa.sa_handler=chldhand;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags=SA_RESTART;
  if(sigaction(SIGALRM,&sa,NULL)==-1){
    perror("sigaction");
    exit(1);
  }

  while (1) {
    printf("Bla-bla-bla?\n"); 
    printf("Bla-bla-bla!\n");
    printf("Bla-bla-bla...\n");
    sleep(2);
  }
}

