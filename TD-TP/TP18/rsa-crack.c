#include "toolBox.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{

  if(argc != 3)
  {
    perror("exe <e> <n>");
    exit(1);
  }

  unsigned int e = atoi(argv[1]);
  unsigned int n = atoi(argv[2]);

  int i;

  unsigned int p = 0,q=0;;

  /****
   * n+1
   * n+3
   * n+7
   * n+9 sont susceptibles d'être premiers
   */
  for(p = 0; p < 65536 ; p+=10)
  {
    
    if(n%(p+1)==0 && p!=0){
      p+=1;
      break;
    }
    if(n%(p+3)==0){
      p+=3;
      break;
    }
    if(n%(p+7)==0){
      p+=7;
      break;
    }
    if(n%(p+9)==0){
      p+=9;
      break;
    }
  }
  if(p == 65536)
  {
    printf("Key not found\n");
    return EXIT_SUCCESS;
  }
  q=(n/p);
  printf("p : %hu\nq : %hu\n",p,q);

  ClefPrivee clefPrivee = obtenirClefPrivee(e,p,q);
  printf("CLEF PRIVEE\n-------------\nn : %u\nd : %u\n\n",clefPrivee.n,clefPrivee.d);

  return EXIT_SUCCESS;
}
