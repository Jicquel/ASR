#ifndef CRYPT_H
#define CRYPT_H
#include <string.h>
#define NB_TOUR 2
unsigned char encrypt_block(unsigned char  block, unsigned short key);
unsigned char decrypt_block(unsigned char  block, unsigned short key);

#endif
