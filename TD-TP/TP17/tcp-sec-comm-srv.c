#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXBUF 500
void tea_decrypt(unsigned int v[2], unsigned int k[4])
{
  unsigned int delta = 0x9e3779b9;
  int i;
  for(i=32;i>0;i--)
  {
    v[1]-=(k[2]+(v[0]<<4))^(v[0]*(delta*i))^(k[3]+(v[0]>>5));
    v[0]-=(k[0]+(v[1]<<4))^(v[1]*(delta*i))^(k[1]+(v[1]>>5));
  }
}

unsigned int exponen(unsigned int g, unsigned int exposant, unsigned int p)
{
  unsigned int res = 1;
  unsigned int tmp = g;

  int i;
  for(i=0;i<32;i++)
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


  printf("%hu", exponen(25,8,89));
  int idS=socket(AF_INET,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t sizelocal = sizeof(struct sockaddr_in);
  int yes=1;
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


  unsigned int p,g,xb,yb,ya,zb;
  unsigned int k[4];

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
  send(nIdS,(void*) &yb,4,0);

  /***reception de ya***/
  recv(nIdS,(void*) &ya,4,0);

  /***calcul zb***/
  zb = exponen(ya,xb,p);

  /***construction key k***/
  k[0] = ~zb;
  k[1] = k[0] & 16383;
  k[2] = k[1] | 16383;
  k[3] = zb;

  char buff[8];
  unsigned int v[2];

  while(1)
  {
    nbBytes=recv(nIdS,v,8,0);
    tea_decrypt(v,k);

    if(nbBytes<=0)
    {
      perror("connexion terminée");
      exit(1);
    }
    memcpy((void*)buff,(void*)v,8);
    write(1,buff,8);
    memset((void*)v,0,8);
    memset(buff,0,8);
  }

  return EXIT_SUCCESS;
}
