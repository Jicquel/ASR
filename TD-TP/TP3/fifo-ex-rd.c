#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
int fd=open("./fifo",O_RDONLY);
char c;
  while(1){
    read(fd,&c,1);
    printf("%c",c);
  }

  return EXIT_SUCCESS;
}
