#include "crypt.h"
#include <stdio.h>

int main(int argc,char** argv)
{
  unsigned short key = 65251;

  unsigned char block = 205;

  unsigned char perm[8];
  perm[0]=7;
  perm[1]=1;
  perm[2]=0;
  perm[3]=4;
  perm[4]=2;
  perm[5]=6;
  perm[6]=5;
  perm[7]=3;

  unsigned char permInverse[8];
  int i;
  for(i=0; i < 8 ; i++)
  permInverse[perm[i]] = i;

  unsigned char substitutions[16];
  substitutions[0]=3;
  substitutions[1]=8;
  substitutions[2]=14;
  substitutions[3]=1;
  substitutions[4]=12;
  substitutions[5]=5;
  substitutions[6]=10;
  substitutions[7]=0;
  substitutions[8]=2;
  substitutions[9]=7;
  substitutions[10]=9;
  substitutions[11]=11;
  substitutions[12]=4;
  substitutions[13]=6;
  substitutions[14]=15;
  substitutions[15]=13;

  unsigned char substitutionsInverse[16];
  for(i=0; i < 16 ; i++)
  substitutionsInverse[substitutions[i]] = i;

  printf("avant cryptage  : %u\n", block);
  block = encrypt_block(block,key,substitutions,perm);

  printf("apres cryptage  : %u\n", block);
  block = decrypt_block(block,key,substitutionsInverse,permInverse);

  printf("apres decryptage  : %u\n", block);

  return 1;
}


