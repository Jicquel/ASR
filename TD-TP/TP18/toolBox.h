#ifndef _TOOL_BOX_H
#define _TOOL_BOX_H

typedef struct solution_ {
  long int d;
  long int x;
  long int y;
}Solution;

unsigned int exponen(unsigned int g, unsigned int exposant, unsigned int modulus);

unsigned long int pgcd(unsigned long x, unsigned long y);

unsigned long int phi(unsigned long n);

Solution bezout(int a, int b);



/**********************/
/******** RSA *********/
/**********************/


typedef struct clefPublique_ {
  unsigned int e;
  unsigned int n;
}ClefPublique;

typedef struct clefPrivee_ {
  unsigned int d;
  unsigned int n;
}ClefPrivee;

ClefPublique obtenirClefPublique( unsigned short p, unsigned short q);

ClefPrivee obtenirClefPrivee(unsigned int e, unsigned int p, unsigned int q);

void obtenirCoupleClefs(ClefPublique* clefPublique, ClefPrivee* clefPrivee, unsigned int p, unsigned int q);

#endif
