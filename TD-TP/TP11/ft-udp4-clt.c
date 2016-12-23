#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>


int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_DGRAM,0);
  int yes=1;

  if(argc != 2)
  {
    perror("executable <addr dest>");
    exit(1);
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_in remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin_port = htons((int16_t) 6666);
  remote.sin_family=AF_INET;
  inet_pton(AF_INET,argv[1],&(remote.sin_addr.s_addr));

  if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1){
    perror("connect");
    exit(1);
  }

  while(1)
  {
    sendto(idS,"salut",6,0,(struct sockaddr*) &remote, sizeof(remote));
    printf("envoi\n");
    sleep(1);
  }
  return EXIT_SUCCESS;
}

