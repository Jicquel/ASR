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
    perror("exe <key file> <substitution file> <permutation file> <plain file> <cipher file>");
    exit(1);
  }

  int fdKey=open(argv[1],O_RDONLY);
  if(fdKey==-1)
  {
    perror("open Key");
    exit(1);
  }

  int fdSubst=open(argv[2],O_RDONLY);
  if(fdSubst==-1)
  {
    perror("open subst");
    exit(1);
  }

  int fdPerm=open(argv[3],O_RDONLY);
  if(fdPerm==-1)
  {
    perror("open perm");
    exit(1);
  }

  int fdPlain=open(argv[4],O_RDONLY);
  if(fdPlain==-1)
  {
    perror("open plain");
    exit(1);
  }

  int fdCipher=open(argv[5],O_CREAT|O_WRONLY);
  if(fdCipher==-1)
  {
    perror("open cipher");
    exit(1);
  }


  //LECTURE CLE 
  unsigned short key = 0;

  if(read(fdKey, (void*) &key, sizeof(unsigned short)) == -1)
  {
    perror("read key");
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

  unsigned char permInverse[8];
  int i;
  for(i=0; i < 8 ; i++)
    permInverse[perm[i]] = i;


  //LECTURE SUBSTITUTIONS
  unsigned char substitutions[16];

  memset(substitutions,0,16*sizeof(unsigned char));
  if(read(fdSubst, (void*)substitutions, 16*sizeof(unsigned char)) == -1)
  {
    perror("read subs");
    exit(1);
  }

  unsigned char substitutionsInverse[16];
  for(i=0; i < 16 ; i++)
    substitutionsInverse[substitutions[i]] = i;


  unsigned char block = 0;
  //CRYPTAGE
  while(read(fdPlain, (void*) &block, sizeof(unsigned char)) > 0)
  {
  printf("avant cryptage  : %u\n", block);
  block = encrypt_block(block,key,substitutions,perm);

  if(write(fdCipher,(void*) &block, sizeof(block)) == -1)
  {
    perror("write cipher");
    exit(1);
  }
  printf("apres cryptage  : %u\n", block);
  block = decrypt_block(block,key,substitutionsInverse,permInverse);

  printf("apres decryptage  : %u\n", block);
  block = 0;
}

  return EXIT_SUCCESS;
}

