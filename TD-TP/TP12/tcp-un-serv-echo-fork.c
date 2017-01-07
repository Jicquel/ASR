#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#define MAXBUF 33

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t sizelocal = sizeof(struct sockaddr_in);;
  int yes=1;
  char c[MAXBUF];
  int port;

if(argc != 2)
{
  perror("executable <port>");
  exit(1);
}

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }

port = atoi(argv[1]);
  struct sockaddr_in local;
  memset(&local,0,sizeof(struct sockaddr_un));
  
  local.sin_port = htons(port);
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;

  if(bind(idS,(struct sockaddr*)&local,sizeof(local))==-1)
  {
    perror("bind");
    exit(1);
  }

  if(listen(idS,5)==-1)
  {
    perror("listen");
    exit(1);
  }


  while(1)
  {

    nIdS=accept(idS,(struct sockaddr*)&local,&sizelocal);

    if(nIdS==-1)
    {
      perror("accept");
      exit(1);
    }
    x=fork();
    if(x==0)
    {
      while(1)
      {
      nbBytes=recv(nIdS,c,MAXBUF,0);
      if(nbBytes<=0)
      {
        perror("connexion terminée");
        exit(1);
      }
      printf("nbByte : %d\n",nbBytes);
      send(nIdS,c,nbBytes,0);
      memset(c,0,MAXBUF);
      }
    }

  }

  return EXIT_SUCCESS;
}
