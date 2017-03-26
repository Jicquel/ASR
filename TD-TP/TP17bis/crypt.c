#include "crypt.h"
#include <stdio.h>
#include <string.h>

unsigned char encrypt_block(unsigned char  block, unsigned short key, unsigned char substitutions[RANGE_VALUE_SUBST], unsigned char permutations[BLOCK_SIZE])
{
  unsigned char k[2];

  memcpy((void*)k,(void*)&key,16);

  unsigned char blockTmp = 0;
  unsigned char mask =0;

  //SUBSTITUTIONS
  blockTmp = 0;
  mask = 0xf;
  blockTmp |= substitutions[(block & mask)];
  mask = mask << 4;
  blockTmp |= (substitutions[(block & mask)>>4] << 4);
  block = blockTmp;

  //PERMUTATIONS
  int j;
  blockTmp = 0;
  mask = 0x1;
  for(j=0;j<BLOCK_SIZE;j++)
  {
    blockTmp |= (block & (mask << permutations[j]));
  }
  block = blockTmp;
  block = block ^ k[1];

  //SUBSTITUTIONS
  blockTmp = 0;
  mask = 0xf;
  blockTmp |= substitutions[(block & mask)];
  mask = mask << 4;
  blockTmp |= (substitutions[(block & mask)>>4] << 4);
  block = blockTmp;

  //PERMUTATIONS
  blockTmp = 0;
  mask = 0x1;
  for(j=0;j<8;j++)
  {
    blockTmp |= (block & (mask << permutations[j]));
  }
  block = blockTmp;

  return block;
}

unsigned char decrypt_block(unsigned char  block, unsigned short key, unsigned char substitutions[RANGE_VALUE_SUBST], unsigned char permutations[BLOCK_SIZE])
{
  unsigned char k[2];

  memcpy((void*)k,(void*)&key,16);

  unsigned char blockTmp = 0;
  unsigned char mask =0;

  //PERMUTATIONS
  int j;
  blockTmp = 0;
  mask = 0x1;
  for(j=0;j<BLOCK_SIZE;j++)
  {
    blockTmp |= (block & (mask << permutations[j]));
  }
  block = blockTmp;

  //SUBSTITUTIONS
  blockTmp = 0;
  mask = 0xf;
  blockTmp |= substitutions[(block & mask)];
  mask = mask << 4;
  blockTmp |= (substitutions[(block & mask)>>4] << 4);
  block = blockTmp;

  block = block ^ k[1];

  //PERMUTATIONS
  blockTmp = 0;
  mask = 0x1;
  for(j=0;j<8;j++)
  {
    blockTmp |= (block & (mask << permutations[j]));
  }
  block = blockTmp;

  //SUBSTITUTIONS
  blockTmp = 0;
  mask = 0xf;
  blockTmp |= substitutions[(block & mask)];
  mask = mask << 4;
  blockTmp |= (substitutions[(block & mask)>>4] << 4);
  block = blockTmp;

  block = block ^ k[0];

  return block;

}
