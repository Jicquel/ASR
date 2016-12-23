#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAXSIZE 256

int main(int argc, char** argv)
{

  int idS=socket(AF_INET,SOCK_DGRAM,0);
  int yes,nbBytes=0;
  char buff[MAXSIZE];

  struct sockaddr_in local;
  socklen_t sizeLocal = sizeof(local);

  memset(buff,0,MAXSIZE);
  socklen_t lenghtSockaddr = sizeof(struct sockaddr);;

  memset(&local,0,sizeof(struct sockaddr_in));
  local.sin_port = htons((int16_t) 6666);
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }

  if(bind(idS,(struct sockaddr*)&local,sizeof(local))==-1)
  {
    perror("bind");
    exit(1);
  }

  while(nbBytes != -1){
    nbBytes = recvfrom(idS,buff,MAXSIZE,0,(struct sockaddr*) &local,&sizeLocal);
    if(nbBytes == -1)
    {
      perror("recvfrom");
      exit(1);
    }
    printf("ok\n");
    printf("%s",buff);
  }
}
