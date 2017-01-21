#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFFER_NBRE_SIZE 100
#define MAXSIZE 500

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizeRemote = sizeof(struct sockaddr_in);
  int yes=1,nbBytes;
  char buffAddr[MAXSIZE],buffPort[MAXSIZE];
  unsigned int nbreNumber,masque,seedValue;
  unsigned int nbBuffer[BUFFER_NBRE_SIZE];

  if(argc != 3)
  {
    perror("executable <file addr> <file port>");
    exit(1);
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_in remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin_family=AF_INET;
  int fdAddr = open(argv[1],O_RDONLY);

  int OK=1;
  while(OK==1)
  {
    int i=0;
    memset(buffAddr,0,MAXSIZE);

    /***LECTURE ADRESSE DANS FICHIER***/
    do
    {
      if(read(fdAddr,&buffAddr[i],sizeof(char)<=0))
      {
        OK=0;
        break;
      }
      i+=1;
    } while(buffAddr[i-1]!='\n');


    /***LECTURE PORT***



    /***TENTATIVES CONNEXIONS***/
    remote.sin_port = htons((int16_t) atoi(buffPort));
    inet_pton(AF_INET,buffAddr,&(remote.sin_addr.s_addr));

    if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1)
    {
      perror("connect");
    }

    return EXIT_SUCCESS;
  }
