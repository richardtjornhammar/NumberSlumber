#include <stdio.h>
#include <stdlib.h>

typedef struct nmr {
  // THE NUMBER REPRESENTATION
  char   *set; // THE CHARACTER SET
  int    base; // THE BASE
  char *value; // THE INPUT
  int    size; // THE NUM SIZE
  int was_allocd[3]; // IF ASSIGNED THEN DONT FREE
} a_number ;
typedef a_number *t_number;

void     mirror_number_value ( t_number number );
void     append_value_to_number ( char l , t_number number );
void     assign_base2number ( t_number number , int base , char *basisset );
t_number create_number ( void );
t_number instantiate_number ( char * , char * );
void     free_number ( t_number number );
void     show_number ( t_number number );
void     print_base_character_set ( int base );
int      gos_char( char s, t_number number );
char     sog_int( int i, t_number number );
int      conversion( t_number num_i, t_number num_o, int bVerbose );
t_number add_numbers ( t_number num_a, t_number num_b, int bVerbose );
void     copy_carr_to_value (  char * , t_number );
void     copy_value_to_carr ( t_number , char * );
