#include <stdio.h>
#include <stdlib.h>

int main(void){
  mkfifo("./fifo",0666);

  return EXIT_SUCCESS;
}
