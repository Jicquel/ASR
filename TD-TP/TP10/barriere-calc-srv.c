#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#define NB_CLIENT_MAX 5
#define TAILLE_TAB 65536

int nbClient = 0,nbFini=0;
int timeDepasse=0;


pthread_mutex_t lock,lockFini;

pthread_cond_t peutCommencer=PTHREAD_COND_INITIALIZER;
pthread_cond_t fini=PTHREAD_COND_INITIALIZER;

/***DONNEES***/
int tab[TAILLE_TAB];
int chunckSize=0,nombreDonneesRecues=0;
int *tabResultat;


int idS;//necessaire pour handler


struct thread_arg{int idS; int numWorker;};

int max(int tab[],int size)
{
  int i;
  int max=0;

  for(i=0; i < size;i++)
  {
    if(max<tab[i])
      max=tab[i];
  }
  return max;
}

void handler(int signo)
{
  if(close(idS)!=0)
  {
    perror("close");
    exit(1);
  }
  timeDepasse=1;
}

void* checkTimeFunction (void* arg){
  alarm(20);
}

void* client(void* arg){

  struct thread_arg args= *(struct thread_arg*)arg;
  int idS = args.idS;
  int numWorker = args.numWorker;
  int nbBytes=0,max=0;
  int left;
  int right;

  char reponse;

<<<<<<< HEAD
  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int nIdS,x,nbBytes;
  socklen_t lengthSock = sizeof(struct sockaddr_un);
=======
  printf("%d'th account created\n",numWorker);
  printf("%d'th account id:%d\n",numWorker,idS);
>>>>>>> 0e3feeba66d595828f2c7b93883e124a1074f601

  while(nbClient <NB_CLIENT_MAX && timeDepasse != 1)
    pthread_cond_wait(&peutCommencer,&lock);

  pthread_mutex_unlock(&lock);
  left=chunckSize*numWorker;
  right=left+chunckSize-1;

  send(idS,(void*)&left,4,0);
  send(idS,(void*)&right,4,0);

<<<<<<< HEAD
  if(listen(idS,20)==-1){
    perror("listen");
    exit(1);
  }

  while(1){
    nIdS=accept(idS,(struct sockaddr*)&socket_Prod,&lengthSock);
    if(nIdS==-1){
      perror("accept");
      exit(1);
    }
    pthread_create(&new_requete, NULL,p_fonction,(void*)&nIdS);
  }
=======
  send(idS,(void*)tab,sizeof(int)*TAILLE_TAB,0);
  recv(idS,(void*)&max,sizeof(max),0);
  
  tabResultat[numWorker]=max;
  nbFini++;
  pthread_cond_signal(&fini);
>>>>>>> 0e3feeba66d595828f2c7b93883e124a1074f601
}

int main(int argc, char** argv){

<<<<<<< HEAD
  int idS=socket(AF_UNIX,SOCK_STREAM,0);
  int nIdS,nbBytes;
=======
>>>>>>> 0e3feeba66d595828f2c7b93883e124a1074f601

  idS=socket(AF_INET,SOCK_STREAM,0);
  socklen_t sizelocal = sizeof(struct sockaddr_in);;
  int port,i,yes=1;
  int nIdS[NB_CLIENT_MAX];

  pthread_t clients[NB_CLIENT_MAX];
  pthread_t checkTime;

  /**SIGACTION**/
  struct sigaction act;



  if(argc != 3)
  {
    perror("executable <port> <file to read>");
    exit(1);
  }

  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }

  if (pthread_mutex_init(&lockFini, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }

  if(setsockopt(idS,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
    perror("setsockopt");
    exit(1);
  }

<<<<<<< HEAD
  struct sockaddr_un socket_Client;
  socklen_t lengthSock = sizeof(socket_Client);
  memset(&socket_Client,0,sizeof(struct sockaddr_un));
  socket_Client.sun_family=AF_UNIX;
  strncpy(socket_Client.sun_path,"/tmp/servClient",sizeof(struct sockaddr_un)-2);
  unlink(socket_Client.sun_path);

  if(bind(idS,(struct sockaddr*)&socket_Client,sizeof(socket_Client))==-1){
=======
  /**SIGACTION**/
  act.sa_handler = handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  if(sigaction(SIGALRM, &act, NULL)==-1)
  {
    perror("sigaction");
    exit(1);
  }

  
  /***CONFIGURATION SOCKET***/
  port = atoi(argv[1]);
  struct sockaddr_in local;
  memset(&local,0,sizeof(struct sockaddr_un));

  local.sin_port = htons(port);
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;


  /***MISE EN ECOUTE SOCKET***/
  if(bind(idS,(struct sockaddr*)&local,sizeof(local))==-1){
>>>>>>> 0e3feeba66d595828f2c7b93883e124a1074f601
    perror("bind");
    exit(1);
  }

  if(listen(idS,NB_CLIENT_MAX)==-1){
    perror("listen");
    exit(1);
  }
 

<<<<<<< HEAD
  while(nIdS = accept(idS,(struct sockaddr*)&socket_Client,&lengthSock) != -1)
  { 
    pthread_create(&new_requete, NULL,p_fonction,(void*)&nIdS);
=======


/***INITIALISATION TABLEAU A TRIER***/
  memset(tab,0,TAILLE_TAB);
  int fd = open(argv[2],O_RDONLY|0664);
  if(fd==-1)
  {
    perror("open");
    exit(1);
>>>>>>> 0e3feeba66d595828f2c7b93883e124a1074f601
  }
  while(read(fd,(void*)&tab[nombreDonneesRecues],sizeof(int))!=0)
    nombreDonneesRecues+=1;



  pthread_create(&checkTime,NULL,checkTimeFunction,NULL);

  while(nbClient <NB_CLIENT_MAX && timeDepasse != 1)
  {
    nIdS[nbClient] = accept(idS,(struct sockaddr*) &local, &sizelocal);

    if(nIdS[nbClient] != -1)
    {
      struct thread_arg args={nIdS[nbClient],nbClient};
      pthread_create(&clients[nbClient], NULL,client,(void*)&args);
      nbClient++;
    }
  }

  chunckSize=TAILLE_TAB/nbClient;

  printf("%d workers hired, no more vacancies or deadline reached\n",nbClient);
  printf("SIZE=%d WKS=%d  chunck size=%d",nombreDonneesRecues,nbClient,chunckSize);
  

  /***CREATION TABLEAU FINAL***/
  tabResultat = malloc(nbClient * sizeof(int));


  pthread_cond_broadcast(&peutCommencer);

  while(nbFini!=nbClient)
    pthread_cond_wait(&fini,&lockFini);

  printf("...all done .... MAX=%d\n",max(tabResultat,nbClient));

  

  return EXIT_SUCCESS;
}
