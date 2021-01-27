#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

typedef struct argus {
  char *args[1];                        // ARGUMENT
  int   verbose,ibase,obase;            // VERBOSITY AND GENERIC BASE SETTING
  char *inputbase,*outputbase;          // THE BASIS SETS
  char *add;
} arguments;
typedef arguments *t_arguments;

//static error_t parse_opt (int key, char *arg, struct argp_state *state);
int parse (int argc, char **argv, t_arguments pargs );
