#include "exponen.h"

unsigned int exponen(unsigned int g, unsigned int exposant, unsigned int p)
{
  unsigned int res = 1;
  unsigned int tmp = g;

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
  return res;
}
