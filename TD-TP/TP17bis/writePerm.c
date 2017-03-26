#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{

  if(argc!=2)
  {
    perror("exe <perm file>");
    exit(1);
  }


  int fdPerm = open(argv[1],O_CREAT|O_WRONLY);
  if(fdPerm==-1)
  {
    perror("open perm");
    exit(1);
  }

  unsigned char perm[8];
  perm[0]=5;
  perm[1]=2;
  perm[2]=0;
  perm[3]=4;
  perm[4]=6;
  perm[5]=1;
  perm[6]=7;
  perm[7]=3;

  if(write(fdPerm,(void*) &perm, 8*sizeof(unsigned char)) == -1)
  {
    perror("write perm");
    exit(1);
  }




  return EXIT_SUCCESS;
}
