#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXBUF 100

int main(int argc, char** argv){
  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int x,nbBytes;
  int yes=1;
  char c[100];

  if(argc != 2)
  {
    perror("./a.out <coordonnées serveur>");
    exit(1);
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un remote;
  memset(&remote,0,sizeof(struct sockaddr_un));
  remote.sun_family=AF_UNIX;
  strncpy(remote.sun_path,argv[1],sizeof(struct sockaddr_un)-2);

  if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1){
    perror("connect");
    exit(1);
  }

  printf("QUIT pour quitter\n");
  printf("Entrer votre choix : ");
  scanf("%s",c);
  while( strcmp(c,"QUIT") != 0)
  {
    send(idS,c,strlen(c),0);
    printf("envoi\n");
    memset(&c,0,MAXBUF);
    nbBytes=recv(idS,c,MAXBUF,0);
    printf("reçu %s\n",c);
    memset(c,0,MAXBUF);
    printf("QUIT pour quitter\n");
    printf("Entrer votre choix : ");
    scanf("%s",c);
    printf("\n");

  }
  return EXIT_SUCCESS;
}
