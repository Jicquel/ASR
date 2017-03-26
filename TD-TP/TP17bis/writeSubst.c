#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{

  if(argc!=2)
  {
    perror("exe <substitutions file>");
    exit(1);
  }


  int fdSubst = open(argv[1],O_CREAT|O_WRONLY,0664);
  if(fdSubst==-1)
  {
    perror("open subst");
    exit(1);
  }

 unsigned char subst[16];
 subst[0]=3;
 subst[1]=8;
 subst[2]=14;
 subst[3]=1;
 subst[4]=12;
 subst[5]=5;
 subst[6]=10;
 subst[7]=0;
 subst[8]=2;
 subst[9]=7;
 subst[10]=9;
 subst[11]=11;
 subst[12]=4;
 subst[13]=6;
 subst[14]=15;
 subst[15]=13;

  if(write(fdSubst,(void*) &subst, 16*sizeof(unsigned char)) == -1)
  {
    perror("write subst");
    exit(1);
  }
  



  return EXIT_SUCCESS;
}
