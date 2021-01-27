#include <stdio.h>
#include <stdlib.h>

void fatal(char *s)
{
  fprintf(stdout,"FATAL: %s\n",s);
  exit(1);
}

int carrlen(char *s)
{
  char *p = s;
  while(*p!='\0')
    p++;
  return(p-s);
}
