#include "verif_gen.h"


int verif_gen(unsigned int p, unsigned int g)
{
  unsigned int i,j;
  char found=0;

  for(i=0;i<p-1;i++)
  {
    for(j=i+1;j<p;j++)
    {
      if(exponen(g,i,p) == exponen(g,j,p) && !found)
        found=1;
    }
    if(!found)
      return 0;
  }
  return 1;
}
