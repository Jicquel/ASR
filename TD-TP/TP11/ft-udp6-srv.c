#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAXSIZE 65536

int calculTempsMicro(struct timeval debut, struct timeval fin)
{
  int nbreMicroSecond = 0;
  if(debut.tv_usec > fin.tv_usec)
  {
    nbreMicroSecond += debut.tv_usec - fin.tv_usec;
    nbreMicroSecond += (fin.tv_sec-debut.tv_sec-1)*1000000;//1sec = 10^6 us
  }
  else
  {
    nbreMicroSecond += fin.tv_usec - debut.tv_usec;
    nbreMicroSecond += (fin.tv_sec-debut.tv_sec)*1000000;//1sec = 10^6 us
  }
  return nbreMicroSecond;
}

int main(int argc, char** argv)
{

  if(argc!=3)
  {
    perror("./exe <port> <file written>");
    exit(1);
  }

  int idS=socket(AF_INET6,SOCK_DGRAM,0);
  int yes,nbBytes=0;
  char buff[MAXSIZE];
  int16_t port = (int16_t) atoi(argv[1]);
  struct sockaddr_in6 local;
  /***GESTION DU TEMPS***/

  struct timeval debut, fin;

  socklen_t sizeLocal = sizeof(local);

  int fd = open(argv[2], O_CREAT | O_WRONLY, 0644);

  if(fd == -1)
  {
    perror("open");
    exit(1);
  }

  memset(buff,0,MAXSIZE);
  socklen_t lenghtSockaddr = sizeof(struct sockaddr);

  memset(&local,0,sizeof(struct sockaddr_in6));
  local.sin6_port = htons(port);
  local.sin6_flowinfo = 0;
  local.sin6_family = AF_INET6;
  local.sin6_addr = in6addr_any;

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
    gettimeofday(&debut,NULL);
    nbBytes = recvfrom(idS,buff,MAXSIZE,0,(struct sockaddr*) &local,&sizeLocal);
    if(nbBytes == -1)
    {
      perror("recvfrom");
      exit(1);
    }
    write(fd,buff,nbBytes);
    printf("reception %d bytes\n",nbBytes);
    gettimeofday(&fin,NULL);

    printf("Temps d'envoi : %d\n",calculTempsMicro(debut,fin));
  }
}
