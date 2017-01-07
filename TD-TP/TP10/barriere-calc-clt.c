#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>

#define TAILLE_TAB 65536


int max(int tab[],int left,int right)
{
  int i;
  int max=0;
  for(i=left; i < right;i++)
  {
    if(max<tab[i])
      max=tab[i];
  }
  return max;
}

int main(int argc, char** argv){
  
  int idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizeRemote = sizeof(struct sockaddr_in);;
  int yes=1,chunckSize=0;
  int tab[TAILLE_TAB];
  int right,left;
  int maximum=0;

  if(argc != 2)
  {
    perror("executable <adresse>");
    exit(1);
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_in remote;
  memset(&remote,0,sizeof(struct sockaddr_in));
  remote.sin_port = htons((int16_t) 7777);
  remote.sin_family=AF_INET;
  inet_pton(AF_INET,argv[1],&(remote.sin_addr.s_addr));

  if(connect(idS,(struct sockaddr*)&remote,sizeof(remote))==-1)
  {
    perror("connect");
    exit(1);
  }


  recv(idS,(void*) &left,4,0);
  recv(idS,(void*) &right,4,0);
  printf("left : %d\nright : %d\n",left,right);

  recv(idS,(void*)tab,sizeof(int)*TAILLE_TAB,0);

  maximum = max(tab,left,right);
  printf("maximum : %d\n",maximum);
  send(idS,(void*)&maximum,4,0);
  
  return EXIT_SUCCESS;
}

