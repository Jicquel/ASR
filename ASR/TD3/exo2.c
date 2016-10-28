void chldhand(int s){
  int x;
  if((x=waitpid(-1,NULL,WNOHANG))>0)sleep(3);
}

int main(int argc, char** argv){
  int pid,pid1,pid2,pid3;
  struct sigaction sa;
  sa.sa_handler=chldhand;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags=SA_RESTART;
  if(sigaction(SIGCHILD,&sa,NULL)==-1){
    perror("sigaction");
    exit(1);
  }
  pid=fork();
  if(pid<0){perror("fork0");exit(1)}
  if(pid==0){sleep(1);exit(0);}
  pid1=fork();
