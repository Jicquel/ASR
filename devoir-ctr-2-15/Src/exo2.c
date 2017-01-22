#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFFER_ADDR_SIZE 100
#define BUFFER_PORT_SIZE 100

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizeRemote = sizeof(struct sockaddr_in);
  int yes=1,nbBytes;
  char buffAddr[BUFFER_ADDR_SIZE],buffPort[BUFFER_PORT_SIZE];
  unsigned int nbreNumber,masque,seedValue;

  if(argc != 3)
  {
    perror("executable <file addr> <file port>");
    exit(1);
  }


  struct sockaddr_in remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin_family=AF_INET;

  /***LECTURE ADRESSE DANS FICHIER***/
  FILE* fdAddr = fopen(argv[1],"r");
  if(fdAddr == NULL)
  {
    perror("fopen");
    exit(1);
  }

  while(fgets(buffAddr,BUFFER_ADDR_SIZE,(FILE*) fdAddr)!=NULL)
  {

    /***LECTURE PORT***/
    FILE* fdPort = fopen(argv[2],"r");
    if(fdPort == NULL)
    {
      perror("fopen");
      exit(1);
    }
    while(fgets(buffPort,BUFFER_PORT_SIZE,(FILE*) fdPort)!=NULL){

      /***TENTATIVES CONNEXIONS***/
      remote.sin_port = htons((int16_t) atoi(buffPort));
      inet_pton(AF_INET,buffAddr,&(remote.sin_addr.s_addr));

      printf("%s:%s\n",buffAddr,buffPort);
      if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1)
      {
        perror("connect");
      }
      else
      {
        printf("Connection nicely done, congrats mate, now i'll let you and have fun with bunch of friends\n");
        close(idS);
        if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
          perror("setsockopt");
          exit(1);
        }
      }
      memset(buffPort,0,BUFFER_PORT_SIZE);
    }
    fclose(fdPort);
    memset(buffAddr,0,BUFFER_ADDR_SIZE);
  }
  fclose(fdAddr);


  return EXIT_SUCCESS;
}
