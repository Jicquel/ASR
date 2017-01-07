#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>

#define NB_CLIENT_MAX 2

int nbClient = 0;
int timeDepasse=0;
pthread_mutex_t lock;

int res = 0;
int idS;

void handler(int signo)
{
  if(close(idS)!=0)
  {
    perror("close");
    exit(1);
  }
  timeDepasse=1;
}

void* checkTimeFunction (void* arg){
  alarm(20);
}

void* client(void* arg){

  int idS = *(int*) arg;
  int nbBytes=0,reponse;
  int envoi = 5;

  send(idS,(void*) &envoi,4,0);
  nbBytes = recv(idS,(void*)&reponse,sizeof(reponse),0);
  pthread_mutex_lock(&lock);
  res+=reponse;
  printf("Resultat : %d\n", res);
  pthread_mutex_unlock(&lock);
  printf("bouh\n");
}

int main(int argc, char** argv){


  idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizelocal = sizeof(struct sockaddr_in);;
  int port,i,yes=1;
  int nIdS[NB_CLIENT_MAX];
  pthread_t clients[NB_CLIENT_MAX];
  pthread_t checkTime;

  /**SIGACTION**/
  struct sigaction act;
  /*************/

  if(argc != 2)
  {
    perror("executable <port>");
    exit(1);
  }

  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  /**SIGACTION**/
  act.sa_handler = handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  if(sigaction(SIGALRM, &act, NULL)==-1)
  {
    perror("sigaction");
    exit(1);
  }
  /*************/

  port = atoi(argv[1]);
  struct sockaddr_in local;
  memset(&local,0,sizeof(struct sockaddr_un));

  local.sin_port = htons(port);
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;

  if(bind(idS,(struct sockaddr*)&local,sizeof(local))==-1){
    perror("bind");
    exit(1);
  }

  if(listen(idS,NB_CLIENT_MAX)==-1){
    perror("listen");
    exit(1);
  }

  pthread_create(&checkTime,NULL,checkTimeFunction,NULL);

  while(nbClient <NB_CLIENT_MAX && timeDepasse != 1)
  {
    nIdS[nbClient] = accept(idS,(struct sockaddr*) &local, &sizelocal);

    if(nIdS[nbClient] != -1)
    {
      nbClient++;
    }
  }

  for(i = 0 ; i < nbClient ; i++)
  {
    pthread_create(&clients[i], NULL,client,(void*)&nIdS[i]);
  }

  for(i = 0 ; i < nbClient ; i++)
  {
    pthread_join(clients[i],NULL);
  }

  printf("Execution finie : %d\n",res);
  return EXIT_SUCCESS;
}
