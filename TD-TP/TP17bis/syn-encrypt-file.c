#include "crypt.h"
#include <stdio.h>

int main(int argc,char** argv)
{

  unsigned short key = 45201;

  unsigned char block = 45;

  printf("avant cryptage  : %c\n", block);
  block = encrypt_block(block,key);

  printf("apres cryptage  : %c\n", block);
  block = decrypt_block(block,key);

  printf("apres decryptage  : %c\n", block);
}


