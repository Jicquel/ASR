#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int main(void){
  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int yes=1;
  int reply=0;
  int valeurEnvoyee=0;
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

  printf("0 pour quitter\n");
  printf("Combien de produits souhaitez-vous ajouter? : ");
  scanf("%d",&valeurEnvoyee);
  while( valeurEnvoyee != 0)
  {
    send(idS,(void*) &valeurEnvoyee,4,0);
    printf("envoi\n");
    valeurEnvoyee=0;
    recv(idS,(void*) &reply,4,0);
    printf("nombre total de produits : %d\n",reply);
    reply=0;
    printf("Combien de produits souhaitez-vous ajouter? : ");
    scanf("%d",&valeurEnvoyee);
    printf("\n");
  }
  return EXIT_SUCCESS;
}

