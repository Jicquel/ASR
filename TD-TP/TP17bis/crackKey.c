#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "crypt.h"
#include <unistd.h>
#include <string.h>


int main(int argc,char** argv)
{

  if(argc != 6)
  {
    perror("exe <substitution file> <permutation file> <plain file> <cipher file>");
    exit(1);
  }

  int fdSubst=open(argv[1],O_RDONLY);
  if(fdSubst==-1)
  {
    perror("open subst");
    exit(1);
  }

  int fdPerm=open(argv[2],O_RDONLY);
  if(fdPerm==-1)
  {
    perror("open perm");
    exit(1);
  }

  int fdPlain=open(argv[3],O_RDONLY);
  if(fdPlain==-1)
  {
    perror("open plain");
    exit(1);
  }

  int fdCipher=open(argv[4],O_RDONLY);
  if(fdCipher==-1)
  {
    perror("open cipher");
    exit(1);
  }




  //LECTURE PERMUTATIONS 
  unsigned char perm[8];

  memset(perm,0,8*sizeof(unsigned char));
  if(read(fdPerm, (void*)perm,8*sizeof(unsigned char)) == -1)
  {
    perror("read perm");
    exit(1);
  }

  //LECTURE SUBSTITUTIONS
  unsigned char substitutions[16];

  memset(substitutions,0,16*sizeof(unsigned char));
  if(read(fdSubst, (void*)substitutions, 16*sizeof(unsigned char)) == -1)
  {
    perror("read subs");
    exit(1);
  }


  //CRACKAGE
  unsigned short key = 0;
  unsigned char blockCrypted = 0;
  unsigned char block=0;

  if(read(fdCipher, (void*) &blockCrypted, sizeof(unsigned char)) <= 0)
  {
    perror("read cipher");
    exit(1);
  }

  for(key = 0; key < 65536 ; key++)
  {
    block = encrypt_block(block,key,substitutions,perm);
    if(block == blockCrypted)
      {
        printf("key : %u\n", key);
        break;
       }
  }

  return EXIT_SUCCESS;
}




