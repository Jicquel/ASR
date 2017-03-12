#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAXSIZE 500


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

int verif_gen(unsigned int p, unsigned int g)
{
  unsigned int i,j;
  char found=0;

  for(i=0;i<p-1;i++)
  {
    for(j=i+1;j<p;j++)
    {
      if(exponen(g,i,p) == exponen(g,j,p) && !found)
        found=1;
    }
    if(!found)
      return 0;
  }
  return 1;
}

void tea_encrypt(unsigned int v[2], unsigned int k[4])
{
  unsigned int delta = 0x9e3779b9;
  int i;
  for(i=1;i<=32;i++)
  {
    v[0]+=(k[0]+(v[1]<<4))^(v[1]*(delta*i))^(k[1]+(v[1]>>5));
    v[1]+=(k[2]+(v[0]<<4))^(v[0]*(delta*i))^(k[3]+(v[0]>>5));
  }
}



int main(int argc, char** argv){

  printf("res : %hu", exponen(3,10,45));

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


  unsigned int p,g, tmp,xa,ya,xb,yb,za;
  unsigned int k[4];
  printf("nombre premier p : ");
  scanf("%d",&p);

  printf("generateur g de p tq g < p : ");
  scanf("%d",&g);

  /**envoi de p***/
  send(idS,(void*) &p,4,0);
  recv(idS,(void*) &tmp,4,0);
  printf("p envoye\n");

  /***envoi de g***/
  send(idS,(void*) &g,4,0);
  recv(idS,(void*) &tmp,4,0);
  printf("g envoye\n");

  /***reception de yb***/
  recv(idS,(void*) &yb,4,0);

  /***choix secret xa***/
  xa = (unsigned int) (rand()%(p-2))+2;

  /***calcul ya et za***/
ya = exponen(g,xa,p);
za = exponen(yb,xa,p);

/***construction key k***/
k[0] = ~za;
k[1] = k[0] & 16383;
k[2] = k[1] | 16383;
k[3] = za;



return EXIT_SUCCESS;
}