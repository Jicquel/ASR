#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "toolBox.h"


unsigned int exponen(unsigned int g, unsigned int exposant, unsigned int p)
{
  unsigned long long int res = 1;
  unsigned long long int tmp = (unsigned long long) g;

  int i;
  for(i=1;i<32;i++)
  {
    if(exposant%2 == 1)
    {
      res = (res*tmp) % p;
    }
    tmp= (tmp*tmp) % p;
    exposant = exposant/2;
  }
  return (unsigned int) res;
}

unsigned long int pgcd(unsigned long x, unsigned long y)
{
  if(x==0)
    return y;
  if (y==0)
    return x;
  if(x>=y) 
    return pgcd(y,x%y);
  if(y>=x)
    return pgcd(x,y%x);
}


unsigned long int phi(unsigned long n)
{
  unsigned long compt=0;
  unsigned long int i;
  for(i = n-1; i!=0;i--)
  {
    if(pgcd(i,n)==1)
    {
      compt++;
    }
  }
  return compt;
}

Solution bezout(int a, int b)
{
  Solution s;
  Solution tmp;

  if (b == 0) {
    s.d = a;
    s.x = 1;
    s.y = 0;

    return s;
  }

  else {
    tmp = bezout(b,a%b);
    s.d = tmp.d;
    s.x = tmp.y;
    s.y = tmp.x-(a/b)*tmp.y;

    return s;
  }
}

/**********************/
/******** RSA *********/
/**********************/

ClefPublique obtenirClefPublique( unsigned short p, unsigned short q)
{
  unsigned int phiDeN = (p-1)*(q-1);
  ClefPublique clef;
  unsigned int e;
  srand(time(NULL));
  do
  {
    e = (unsigned) rand()%phiDeN; 

  }while(pgcd(e,phiDeN) != 1);

  clef.e = e;
  clef.n = p*q;

  return clef;
}

ClefPrivee obtenirClefPrivee(unsigned int e, unsigned short p, unsigned short q)
{

  ClefPrivee clef;
  unsigned int phiDeN = (unsigned)(p-1)*(q-1);
  unsigned int n = p*q;

  Solution s = bezout(e, phiDeN);
  while(s.x <0)
    s.x+=phiDeN;
  clef.d = ((s.x)%phiDeN);

  clef.n = n;

  return clef;
}

void obtenirCoupleClefs(ClefPublique* clefPublique, ClefPrivee* clefPrivee, unsigned short p, unsigned short q)
{
  *clefPublique = obtenirClefPublique(p,q);
  *clefPrivee = obtenirClefPrivee(clefPublique->e,p,q);
}
