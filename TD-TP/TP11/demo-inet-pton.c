#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){

  unsigned char buf4[sizeof(struct in_addr)];
  unsigned char buf6[sizeof(struct in6_addr)];

  int fd;

  if(argc!=3){
    printf("./ip2n <address 4 or 6> <file to save>");
    exit(1);
  }

  fd=open(argv[2],O_WRONLY|O_CREAT,0644);
  if(strchr(argv[1],'.')!=NULL)
  {
    if(inet_pton(AF_INET,argv[1],(void*)buf4)<=0)
    {
      perror("inet_pton");
      exit(1);
    }

    printf("ipv4 address\n");
    write(fd,buf4,sizeof(struct in_addr));
    close(fd);
  }
  else if(strchr(argv[1],':')!=NULL)
  {
    if(inet_pton(AF_INET6,argv[1],(void*)buf6)<=0)
    {
      perror("inet_pton");
      exit(1);
    }

    printf("ipv6 address\n");
    write(fd,buf6,sizeof(struct in6_addr));
    close(fd);
  }
  else
  {
    printf("adresse non valide\n");
    exit(1);
  }

  return EXIT_SUCCESS;
}
