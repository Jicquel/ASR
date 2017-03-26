#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{

  if(argc!=3)
  {
    perror("exe <key value < 65536> <key file>");
    exit(1);
  }


  int fdKey = open(argv[2],O_CREAT|O_WRONLY|0664);
  if(fdKey==-1)
  {
    perror("open key");
    exit(1);
  }

  unsigned short key = (unsigned short) atoi(argv[1]); 
  
  if(write(fdKey,(void*) &key, sizeof(key)) == -1)
  {
    perror("write key");
    exit(1);
  }
  



  return EXIT_SUCCESS;
}
