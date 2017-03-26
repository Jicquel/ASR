#include "toolBox.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>


int main(int argc, char **argv)
{

  unsigned short p = 32531, q = 25469;

  ClefPrivee clefPrivee;
  ClefPublique clefPublique;

  obtenirCoupleClefs(&clefPublique, &clefPrivee, p,q);

  printf("CLEF PUBLIQUE\n-------------\nn : %u\ne : %u\n\n",clefPublique.n,clefPublique.e);
  printf("CLEF PRIVEE\n-------------\nn : %u\nd : %u\n\n",clefPrivee.n,clefPrivee.d);
  


  return EXIT_SUCCESS;
}
