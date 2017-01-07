#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAXSIZE 500

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizeRemote = sizeof(struct sockaddr_in);;
  int yes=1,nbByte;
  char buff[MAXSIZE];
  memset(buff,0,MAXSIZE);

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

  printf("QUIT pour quitter");
  printf("message à envoyer : ");
  scanf("%s",buff);

  while(strcmp(buff,"QUIT")!=0)
  {
    send(idS,buff,strlen(buff),0);
    printf("envoi\n");
    recv(idS,(void*) buff,MAXSIZE,0);
    printf("message reçu : %s\n",buff);
    printf("\nmessage à envoyer : ");

    memset(buff,0,MAXSIZE);
    scanf("%s",buff);
  }
  return EXIT_SUCCESS;
}
