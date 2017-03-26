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
  int valeurEnvoyee=4;

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un socket_Cons;
  memset(&socket_Cons,0,sizeof(struct sockaddr_un));
  socket_Cons.sun_family=AF_UNIX;
  strncpy(socket_Cons.sun_path,"/tmp/servCons",sizeof(struct sockaddr_un)-2);

  if(connect(idS,(struct sockaddr*)&socket_Cons,sizeof(socket_Cons))==-1){
    perror("connect");
    exit(1);
  }

  printf("0 pour quitter\n");

  printf("Combien de produits souhaitez-vous retirer? : ");
  scanf("%d",&valeurEnvoyee);

  while( valeurEnvoyee != 0)
  {
    send(idS,(void*) &valeurEnvoyee,4,0);
    printf("envoi\n");
    valeurEnvoyee=0;
    recv(idS,(void*) &reply,4,0);
    printf("nombre total de produits : %d\n",reply);
    reply=0;
    printf("Combien de produits souhaitez-vous retirer? : ");
    scanf("%d",&valeurEnvoyee);
    printf("\n");
  }
  return EXIT_SUCCESS;
}
