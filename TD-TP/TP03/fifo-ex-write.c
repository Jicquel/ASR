#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
  int fd=open("./fifo",O_WRONLY);
  
  while(1){
    write(fd,"s\n",2);
  }
  return EXIT_SUCCESS;
}
