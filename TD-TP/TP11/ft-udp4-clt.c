#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXSIZE 65536 //2^16

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_DGRAM,0);
  int yes=1, nbByte;
  char buff[MAXSIZE];
  memset(buff,0,MAXSIZE);

  if(argc != 4)
  {
    perror("executable <file name> <taille d'une tranche> <addr dest>");
    exit(1);
  }

  int fd = open(argv[1],O_RDONLY,0644);
  int tranche = atoi(argv[2]);
  printf("%d", tranche);

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }

  
  struct sockaddr_in remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin_port = htons((int16_t) 6666);
  remote.sin_family=AF_INET;
  inet_pton(AF_INET,argv[3],&(remote.sin_addr.s_addr));

  if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1)
  {
    perror("connect");
    exit(1);
  }

  nbByte = read(fd,(void*) buff,(size_t) tranche);

  while(nbByte != 0)
  {
    sendto(idS,buff,nbByte,0,(struct sockaddr*) &remote, sizeof(remote));
    printf("envoi de %d byte\n",nbByte);
    memset(buff,0,MAXSIZE);
  nbByte = read(fd,(void*) buff,(size_t) tranche);
  }
  printf("Fini\n");
  return EXIT_SUCCESS;
}

