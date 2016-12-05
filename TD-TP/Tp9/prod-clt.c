#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int main(void){
  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  socklen_t toto = sizeof(struct sockaddr_un);;
  int yes=1;
  int reply=0;
  int valeurEnvoye=2;
  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un socket_Prod;
  memset(&socket_Prod,0,sizeof(struct sockaddr_un));
  socket_Prod.sun_family=AF_UNIX;
  strncpy(socket_Prod.sun_path,"/tmp/servProd",sizeof(struct sockaddr_un)-2);

  if(connect(idS,(struct sockaddr*)&socket_Prod,sizeof(socket_Prod))==-1){
    perror("connect");
    exit(1);
  }

  while(1){
    send(idS,(void*)&valeurEnvoye,4,0);
    printf("envoi\n");
    recv(idS,(void*)&reply,4,0);
    printf("envoi bien effectué");
    sleep(2);
    }
  return EXIT_SUCCESS;
}

