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
    perror("exe <plain file> <cipher file> <e> <n>");
    exit(1);
  }

  int fdPlain=open(argv[1],O_RDONLY);
  if(fdPlain==-1)
  {
    perror("open plain");
    exit(1);
  }

  int fdCipher=open(argv[2],O_CREAT|O_WRONLY,0664);
  if(fdCipher==-1)
  {
    perror("open cipher");
    exit(1);
  }

  unsigned int e = atoi(argv[3]);
  unsigned int n = atoi(argv[4]);

  unsigned char block[BYTE_BLOCK_SIZE];

  unsigned int blockWithPadding=0;
  int i;
  char cryptageFini = 0;
  while(!cryptageFini)
  {
    memset(block,0,BYTE_BLOCK_SIZE);
    blockWithPadding = 0;

    for(i=0;i<BYTE_BLOCK_SIZE;i++)
    {
      if(read(fdPlain, (void*)block+i, sizeof(unsigned char))<=0)
      {
        cryptageFini=1;
        break;
      }
    }

    memcpy(&blockWithPadding,block,BYTE_BLOCK_SIZE);

    blockWithPadding = exponen(blockWithPadding,e,n);
    if(write(fdCipher,(void*) &blockWithPadding, sizeof(unsigned int)) == -1)
    {
      perror("write cipher");
      exit(1);
    }

  }

  return EXIT_SUCCESS;
}
