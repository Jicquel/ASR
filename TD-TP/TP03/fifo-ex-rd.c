#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
int fd=open("./fifo",O_RDONLY);
char c;
FILE * fp;
fp = fopen("./fifo","r");

  while(1){
    read(fd,&c,1);
    printf("%c",c);
    sleep(1);
  }

  return EXIT_SUCCESS;
}
