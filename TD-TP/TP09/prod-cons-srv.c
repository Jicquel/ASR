#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NB_PROD_MAX 15

int nbProduit = 0;

pthread_mutex_t vitrine;
pthread_cond_t noProduit = PTHREAD_COND_INITIALIZER, fullProduit = PTHREAD_COND_INITIALIZER;

void* c_fonction(void* arg){
  int nIdS = *(int*)arg;
  int nbProduitEnleve;


  while(1){
    recv(nIdS,(void*)&nbProduitEnleve,4,0);
    pthread_mutex_lock(&vitrine);
    while(nbProduit<nbProduitEnleve)
      pthread_cond_wait(&noProduit,&vitrine);

    nbProduit-=nbProduitEnleve;
    pthread_mutex_unlock(&vitrine);
    pthread_cond_signal(&fullProduit);
    send(nIdS,(void*) &nbProduit,4,0);
  }
}

void* consommateur(void* arg){

  pthread_t new_requete;

  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t lenghtSockaddr = sizeof(struct sockaddr_un);;

  int yes=1;

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un socket_Cons;
  memset(&socket_Cons,0,sizeof(struct sockaddr_un));
  socket_Cons.sun_family=AF_UNIX;
  strncpy(socket_Cons.sun_path,"/tmp/servCons",sizeof(struct sockaddr_un)-2);
  unlink(socket_Cons.sun_path);

  if(bind(idS,(struct sockaddr*)&socket_Cons,sizeof(socket_Cons))==-1)
  {
    perror("bind");
    exit(1);
  }

  if(listen(idS,20)==-1)
  {
    perror("listen");
    exit(1);
  }

  while(1)
  {
    nIdS=accept(idS,(struct sockaddr*)&socket_Cons,&lenghtSockaddr);
    if(nIdS==-1)
    {
      perror("accept");
      exit(1);
    }
    pthread_create(&new_requete, NULL,c_fonction,(void*)&nIdS);
  }
}


void* p_fonction(void* arg){
  int nIdS = *(int*)arg;

  int nbProduitRajoute=0;
  while(1){
    recv(nIdS,(void*)&nbProduitRajoute,4,0);
    pthread_mutex_lock(&vitrine);

    while(nbProduit+nbProduitRajoute >= NB_PROD_MAX)
      pthread_cond_wait(&fullProduit,&vitrine);

    nbProduit+=nbProduitRajoute;
    printf("nombre produits : %d\n",nbProduit);
    send(nIdS,(void*)&nbProduit,4,0);
    pthread_mutex_unlock(&vitrine);
    pthread_cond_signal(&noProduit);

  }
}


void* producteur(void* arg){

  pthread_t new_requete;

  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t toto = sizeof(struct sockaddr_un);;

  int yes=1;

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_un socket_Prod;
  memset(&socket_Prod,0,sizeof(struct sockaddr_un));
  socket_Prod.sun_family=AF_UNIX;
  strncpy(socket_Prod.sun_path,"/tmp/servProd",sizeof(struct sockaddr_un)-2);
  unlink(socket_Prod.sun_path);

  if(bind(idS,(struct sockaddr*)&socket_Prod,sizeof(socket_Prod))==-1)
  {
    perror("bind");
    exit(1);
  }

  if(listen(idS,20)==-1)
  {
    perror("listen");
    exit(1);
  }

  while(1)
  {
    nIdS=accept(idS,(struct sockaddr*)&socket_Prod,&toto);
    if(nIdS==-1)
    {
      perror("accept");
      exit(1);
    }
    pthread_create(&new_requete, NULL,p_fonction,(void*)&nIdS);
  }
}

int main(int argc, char** argv)
{

  pthread_t prod;
  pthread_t cons;
  int i;


  pthread_create(&prod,NULL,producteur,NULL);

  pthread_create(&cons,NULL,consommateur,NULL);
  printf("Le serveur est en route \n");

  while(1){
    sleep(10);
  }

}
