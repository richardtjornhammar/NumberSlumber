int carrlen(char *s)
{
  char *p = s;
  while(*p!='\0')
    p++;
  return(p-s);
}
