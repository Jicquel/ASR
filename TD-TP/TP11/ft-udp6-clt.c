#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>


#define MAXSIZE 65536 //2^16

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

int main(int argc, char** argv){
  int idS=socket(AF_INET6,SOCK_DGRAM,0);
  int yes=1, nbByte;
  char buff[MAXSIZE];
  memset(buff,0,MAXSIZE);

  /***GESTION DU TEMPS***/
  struct timeval debut, fin;

  if(argc != 4)
  {
    perror("executable <file name> <taille d'une tranche> <addr dest>");
    exit(1);
  }

  int fd = open(argv[1],O_RDONLY,0644);
  int tranche = atoi(argv[2]);

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }


  struct sockaddr_in6 remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin6_port = htons((int16_t) 6666);
  remote.sin6_flowinfo = 0;
  remote.sin6_family = AF_INET6;
  remote.sin6_addr = in6addr_any;
  inet_pton(AF_INET,argv[3],&(remote.sin6_addr));

  if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1)
  {
    perror("connect");
    exit(1);
  }

  nbByte = read(fd,(void*) buff,(size_t) tranche);

  while(nbByte != 0)
  {
    gettimeofday(&debut,NULL);
    sendto(idS,buff,nbByte,0,(struct sockaddr*) &remote, sizeof(remote));
    printf("envoi de %d byte\n",nbByte);
    memset(buff,0,MAXSIZE);
    nbByte = read(fd,(void*) buff,(size_t) tranche);
    gettimeofday(&fin,NULL);

    printf("Temps d'envoi : %d\n",calculTempsMicro(debut,fin));
  }

  printf("Fini\n");
  return EXIT_SUCCESS;
}

