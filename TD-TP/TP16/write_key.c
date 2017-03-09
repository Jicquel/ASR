#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char** argv)
{
  if(argc!=2)
  {
    perror("executable <key file>");
    exit(1);
  }

  int fdKey=open(argv[1],O_WRONLY|O_CREAT);
  if(fdKey==-1)
  {
    perror("open");
    exit(1);
  }

  int i,nbByte;
  unsigned int
  for( i=0 ; i<4  ;i++)
  {
   nbByte = write(fdKey, ,sizeof(unsigned int));
   if(nbByte == -1)
   {
     perror("write");
     exit(1);
   }

  return EXIT_SUCCESS;
}
