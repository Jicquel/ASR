#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXBUF 500
#define MAXFILESIZE 1500


int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t sizelocal = sizeof(struct sockaddr_in);;
  int yes=1;
  char c[MAXBUF];
  int port;
  unsigned int nbreNumber,masque,seedValue,fd;
  unsigned int bufferNombres[MAXFILESIZE];

  if(argc != 2)
  {
    perror("executable <port>");
    exit(1);
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
  {
    perror("setsockopt");
    exit(1);
  }

  port = atoi(argv[1]);
  struct sockaddr_in local;
  memset(&local,0,sizeof(struct sockaddr_in));

  local.sin_port = htons(port);
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;

  if(bind(idS,(struct sockaddr*)&local,sizeof(local))==-1)
  {
    perror("bind");
    exit(1);
  }

  if(listen(idS,5)==-1)
  {
    perror("listen");
    exit(1);
  }


  while(1)
  {

    nIdS=accept(idS,(struct sockaddr*)&local,&sizelocal);

    if(nIdS==-1)
    {
      perror("accept");
      exit(1);
    }

    x=fork();
    if(x==0)
    {

      /***OBTENTION ARGUMENTS***/
      sprintf(c,"%s","This is a random number generator\nHow many numbers do you need?");
      send(nIdS,c,strlen(c),0);
      if(recv(nIdS,(void*)&nbreNumber,sizeof(int),0) <= 0)
      {
        perror("connexion terminée");
        exit(1);
      }

      memset(c,0,MAXBUF);
      sprintf(c,"%s","masque?");
      send(nIdS,c,strlen(c),0);
      if(recv(nIdS,(void*)&masque,sizeof(int),0)<=0)
      {
        perror("connexion terminée");
        exit(1);
      }

      memset(c,0,MAXBUF);
      sprintf(c,"%s","seed value?");
      send(nIdS,c,strlen(c),0);
      if(recv(nIdS,(void*)&masque,sizeof(int),0)<=0)
      {
        perror("connexion terminée");
        exit(1);
      }

      /***TRAITEMENT DONNEES A ENVOYER***/
      


      /***ENVOI DES DONNEES***/
      fd = open("./tmp",O_RDONLY); 
      memset(bufferNombres,0,MAXFILESIZE);
      if(read(fd,bufferNombres,nbreNumber*sizeof(unsigned int))==-1)
      {
        perror("read");
        exit(1);
      }

      send(nIdS,bufferNombres,nbreNumber*sizeof(unsigned int),0);
      close(nIdS);
      if(remove("./tmp")==-1)
      {
        perror("remove");
        exit(1);
      }


    }
  }

  return EXIT_SUCCESS;
}
