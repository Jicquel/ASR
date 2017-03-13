#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXBUF 500

unsigned int exponen(unsigned int g, unsigned int exposant, unsigned int p)
{
  unsigned int res = 1;
  unsigned int tmp = g;

  int i;
  for(i=1;i<32;i++)
  {
    if(exposant%2 == 1)
    {
      res = (res*tmp) % p;
    }
    tmp= (tmp*tmp) % p;
    exposant = exposant/2;
  }
  return res;
}

int main(int argc, char** argv){
  int idS=socket(AF_INET,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t sizelocal = sizeof(struct sockaddr_in);
  int yes=1;
  char c[MAXBUF];
  int port;

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

  if(listen(idS,1)==-1)
  {
    perror("listen");
    exit(1);
  }

  nIdS=accept(idS,(struct sockaddr*)&local,&sizelocal);

  if(nIdS==-1)
  {
    perror("accept");
    exit(1);
  }


  unsigned int p,g,xb;
/***reception de p***/
  recv(nIdS,(void*) &p,4,0);
  send(nIdS,(void*) &p,4,0);

  /***reception de g***/
  recv(nIdS,(void*) &g,4,0);
  send(nIdS,(void*) &g,4,0);

/***choix secret xb***/
  xb = (unsigned int) (rand()%(p-2))+2;

  /***calcul yb***/
  yb = exponen(g,xb,p);
  
  /***envoi de yb***/
  send(IdS,(void*) &yb,4,0);

  /***calcul zb***/
  zb = exponen(yb,xa,p);
  
  /***

  while(1)
  {
    nbBytes=recv(nIdS,c,MAXBUF,0);
    if(nbBytes<=0)
    {
      perror("connexion terminée");
      exit(1);
    }
    printf("nbByte : %d\n",nbBytes);
    send(nIdS,c,nbBytes,0);
    memset(c,0,MAXBUF);
  }



return EXIT_SUCCESS;
}
