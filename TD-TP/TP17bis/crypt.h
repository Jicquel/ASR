#ifndef _CRYPT_H
#define _CRYPT_H

#define NB_TOUR 2
#define BLOCK_SIZE 8
#define RANGE_VALUE_SUBST 16 //2**4

unsigned char encrypt_block(unsigned char block, unsigned short key, unsigned char substitutions[], unsigned char permutations[]);
unsigned char decrypt_block(unsigned char block, unsigned short key, unsigned char substitutions[], unsigned char permutations[]);

#endif
