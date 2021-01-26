#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include "symbols.h"
#include "parser.h"
#include "numbers.h"

int main ( int argc, char *argv[] )
// guix environment --container --ad-hoc gcc-toolchain
// compile with: $ gcc -lm main.c -o nums
{
  int bases[2]  = { 10 , 2 } ;
  char *set_in  = NULL ;
  char *set_out = NULL ;
  //
  arguments     args;
  t_arguments  pargs;
  pargs      = &args;
  //
  // PARSE INPUT
  parse(argc,argv,pargs);
  //
  // ASSIGN PARSED INPUT
  if ( pargs->inputbase != NULL )
  {
    set_in   = pargs -> inputbase;
    bases[0] = carrlen ( set_in );
  } else { // IN THIS CASE THE INPUT BASE SET IS ASSIGNED LATER
    bases[0] = pargs -> ibase;
  }
  if ( pargs->outputbase != NULL )
  {
    set_out  = pargs -> outputbase;
    bases[1] = carrlen ( set_out );
  } else { // IN THIS CASE THE OUTPUT BASE SET IS ASSIGNED LATER
    bases[1] = pargs -> obase;
  }
  int bVerbose = pargs -> verbose;

  t_number number_i , number_o , number_r , number_e;

  number_i = create_number() ;
  number_o = create_number() ;
  //
  number_i -> size   = carrlen( pargs->args[0] );
  assign_base2number ( number_i , bases[0] , set_in );
  copy_carr_to_value ( pargs->args[0] , number_i );
  //
  assign_base2number ( number_o , bases[1] , set_out );
  //
  if ( bVerbose )
    show_number ( number_i );
  //
  conversion ( number_i , number_o , bVerbose );
  //
  fprintf(stdout,"%s\n",number_o->value);
  //
  if ( bVerbose )
    show_number ( number_o );
  //
  if (0)
  {
    fprintf(stdout,"ADDING\n");
    t_number numb = instantiate_number("12","0123456789");
    number_e = add_numbers( number_i , numb , bVerbose );
    show_number ( number_e );
    show_number ( numb );
    free_number ( numb );
    free_number ( number_e );
  }  
  free_number ( number_i );
  free_number ( number_o );

  if(bVerbose)
  {
    print_base_character_set ( 32 ) ;
    print_base_character_set ( 10 ) ;
    print_base_character_set (  2 ) ;
  }

  return ( 0 ) ;
}
