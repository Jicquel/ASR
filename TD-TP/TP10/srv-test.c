#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NB_CLIENT_MAX 10


   int nIdS;
void* open_account(void* arg){
  char buffer[200];
  while(recv(nIdS,buffer,200,0)){printf("%s\n",buffer);}

}

int main(int argc, char** argv){

  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int x,nbBytes;
  pthread_t new_requete;

  int yes=1;

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un socket_Cons;
  socklen_t tailleSocket=sizeof(struct sockaddr_un);
  memset(&socket_Cons,0,sizeof(struct sockaddr_un));
  socket_Cons.sun_family=AF_UNIX;
  strncpy(socket_Cons.sun_path,"/tmp/serv",sizeof(struct sockaddr_un)-2);
  unlink(socket_Cons.sun_path);

  if(bind(idS,(struct sockaddr*)&socket_Cons,sizeof(socket_Cons))==-1){
    perror("bind");
    exit(1);
  }

  if(listen(idS,NB_CLIENT_MAX)==-1){
    perror("listen");
    exit(1);
  }

  while(1){
    nIdS=accept(idS,(struct sockaddr*)&socket_Cons,&tailleSocket);
    if(nIdS==-1){
      perror("accept");
      exit(1);
    }
    close(idS);
    pthread_create(&new_requete, NULL,open_account,(void*)&nIdS);
    break;

  }


  while(1){
    sleep(1);
  }

}
