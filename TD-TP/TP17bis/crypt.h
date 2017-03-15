#ifndef CRYPT_H
#define CRYPT_H

unsigned char encrypt_block(unsigned char  block, unsigned short key);
unsigned char decrypt_block(unsigned char  block, unsigned short key);

#endif
