#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_NBRE_SIZE 100
#define MAXSIZE 500

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizeRemote = sizeof(struct sockaddr_in);;
  int yes=1,nbBytes;
  char buff[MAXSIZE];
  memset(buff,0,MAXSIZE);
  unsigned int nbreNumber,masque,seedValue;
  unsigned int nbBuffer[BUFFER_NBRE_SIZE];

  if(argc != 3)
  {
    perror("executable <server addr> <port>");
    exit(1);
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_in remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin_port = htons((int16_t) atoi(argv[2]));
  remote.sin_family=AF_INET;
  inet_pton(AF_INET,argv[1],&(remote.sin_addr.s_addr));

  if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1)
  {
    perror("connect");
    exit(1);
  }


  memset(buff,0,MAXSIZE);
  if(recv(idS,(void*) buff,MAXSIZE,0)<=0)
  {
    perror("recv");
    exit(1);
  }
  printf("%s",buff);
  scanf("%d",&nbreNumber);
  send(idS,(void*)&nbreNumber,sizeof(unsigned int),0);


  memset(buff,0,MAXSIZE);
  if(recv(idS,(void*) buff,MAXSIZE,0)<=0)
  {
    perror("recv");
    exit(1);
  }
  printf("%s",buff);
  scanf("%x",&masque);
  send(idS,(void*)&masque,sizeof(unsigned int),0);


  memset(buff,0,MAXSIZE);
  if(recv(idS,(void*) buff,MAXSIZE,0)<=0)
  {
    perror("recv");
    exit(1);
  }
  printf("%s",buff);
  scanf("%d",&seedValue);
  send(idS,(void*)&seedValue,sizeof(unsigned int),0);

  nbBytes = recv(idS,(void*) nbBuffer, nbreNumber*sizeof(unsigned int),0);
  if(nbBytes==-1)
  {
    perror("recv");
    exit(1);
  }

  printf("%d bytes received\n\n",nbBytes);

  int i;
  for(i=0;i<nbreNumber;i++)
  {
    printf("%d   ",nbBuffer[i]);
  }

  close(idS);
  printf("\nall done...");
  
  return EXIT_SUCCESS;
}
