#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


void tea_decrypt(unsigned int v[2], unsigned int k[4])
{
  unsigned int delta = 0x9e3779b9;
  int i;
  for(i=32;i>0;i--)
  {
    v[1]-=(k[2]+(v[0]<<4))^(v[0]*(delta*i))^(k[3]+(v[0]>>5));
    v[0]-=(k[0]+(v[1]<<4))^(v[1]*(delta*i))^(k[1]+(v[1]>>5));
  }
}

int main(int argc, char** argv)
{

  unsigned int key[4];
  unsigned int v[2];

  if(argc != 4)
  {
    perror("<key file> <plain file> <cypher file>");
    exit(1);
  }
  
  int fdKey=open(argv[1],O_RDONLY);
  if(fdKey==-1)
  {
    perror("open");
    exit(1);
  }

  int fdPlain=open(argv[2],O_CREAT|O_WRONLY);
  if(fdPlain==-1)
  {
    perror("open");
    exit(1);
  }

  int fdCypher=open(argv[3],O_RDONLY);
  if(fdPlain==-1)
  {
    perror("open");
    exit(1);
  }
    
  //LECTURE CLE 
  read(fdKey, (void*) key, 4*(sizeof(unsigned int)));
 
  while(read(fdCypher,(void*) v, 2 * sizeof(unsigned int)) > 0 )
  {
	tea_decrypt(v,key);
    if(write(fdPlain, (void*) v, 2*sizeof(unsigned int)) == -1)
    {
    perror("write fdCyper");
    exit(1);
    }
  }

return EXIT_SUCCESS;
}
