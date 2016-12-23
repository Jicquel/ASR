#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXBUF 100

int main(int argc, char** argv){


  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t toto = sizeof(struct sockaddr_un);;
  int yes=1;
  char c[100];
  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un local,remote;
  memset(&local,0,sizeof(struct sockaddr_un));
  local.sun_family=AF_UNIX;
  strncpy(local.sun_path,argv[1],sizeof(struct sockaddr_un)-2);
  unlink(local.sun_path);


  if(bind(idS,(struct sockaddr*)&local,sizeof(local))==-1){
    perror("bind");
    exit(1);
  }

  if(listen(idS,3)==-1){
    perror("listen");
    exit(1);
  }

  getchar();


  while(1){

    nIdS=accept(idS,(struct sockaddr*)&local,&toto);
    if(nIdS==-1){
      perror("accept");
      exit(1);
    }
    x=fork();
    if(x==0){
      close(idS);
      while(1){
      nbBytes=recv(nIdS,c,MAXBUF,0);
      printf("nbByte : %d",nbBytes);
      send(nIdS,c,nbBytes,0);
      }
    }

  }

  return EXIT_SUCCESS;
}
