#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXSIZE 256

int main(int argc, char** argv)
{

  if(argc!=3)
  {
    perror("./exe <port> <file written>");
    exit(1);
  }

  int idS=socket(AF_INET,SOCK_DGRAM,0);
  int yes,nbBytes=0;
  char buff[MAXSIZE];
  int16_t port = (int16_t) atoi(argv[1]);
  struct sockaddr_in local;
  socklen_t sizeLocal = sizeof(local);

  int fd = open(argv[2], O_CREAT | O_WRONLY, 0644);

  if(fd == -1)
  {
    perror("open");
    exit(1);
  }

  memset(buff,0,MAXSIZE);
  socklen_t lenghtSockaddr = sizeof(struct sockaddr);;

  memset(&local,0,sizeof(struct sockaddr_in));
  local.sin_port = htons(port);
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
    write(fd,buff,nbBytes);
    printf("reception %d bytes\n",nbBytes);
  }
}
