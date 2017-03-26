#include "toolBox.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BYTE_BLOCK_SIZE 3


int main(int argc, char **argv)
{

  if(argc != 5)
  {
    perror("exe <plain file> <cipher file> <d> <n>");
    exit(1);
  }

  int fdPlain=open(argv[1],O_CREAT|O_WRONLY,0664);
  if(fdPlain==-1)
  {
    perror("open plain");
    exit(1);
  }

  int fdCipher=open(argv[2],O_RDONLY);
  if(fdCipher==-1)
  {
    perror("open cipher");
    exit(1);
  }

  unsigned int d = atoi(argv[3]);
  unsigned int n = atoi(argv[4]);

  unsigned int blockWithPadding=0;

  while(read(fdCipher, (void*)&blockWithPadding,sizeof(unsigned int)) > 0)
  {
    blockWithPadding = exponen(blockWithPadding,d,n);
    if(write(fdPlain,(void*) &blockWithPadding, BYTE_BLOCK_SIZE) == -1)
    {
      perror("write cipher");
      exit(1);
    }
  }

  return EXIT_SUCCESS;
}
