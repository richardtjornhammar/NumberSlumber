#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <argp.h>

#define SYMBOL_NULL_INDEX 48

int carrlen(char *s)
{
  char *p = s;
  while(*p!='\0')
    p++;
  return(p-s);
}

const char *argp_program_version =
"NumberSlumber 1.0";

const char *argp_program_bug_address =
"<richard.tjornhammar@gmail.com>";

typedef struct argus {
  char *args[1];                        // ARGUMENT
  int   verbose,ibase,obase;            // VERBOSITY AND GENERIC BASE SETTING
  char *inputbase,*outputbase;          // THE BASIS SETS
} arguments;
typedef arguments *t_arguments;
//
// OPTIONS.  Field 1 in ARGP.
// Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
//
static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"inputbase", 'i', "0123456789", 0,
   "input base character set"},
  {"ibase" ,  'n' , "10" , 0,
   "convert from base n using generic set"},  
  {"outputbase", 'o', "01", 0,
   "output base character set"},
  {"obase" ,  'm',  "2" , 0,
   "convert to base m using generic set"},
  {0}
};
//
// PARSER. Field 2 in ARGP.
// Order of parameters: KEY, ARG, STATE.
//
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  t_arguments pargs = state->input;

  switch (key)
    {
    case 'v':
      pargs->verbose = 1;
      break;
    case 'i':
      pargs -> inputbase = arg;
      break;
    case 'o':
      pargs -> outputbase = arg;
      break;
    case 'n':
      pargs->ibase = atoi(arg);
      break;
    case 'm':
      pargs->obase = atoi(arg);
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
          argp_usage(state);
      }
      pargs->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1)
        {
          argp_usage (state);
        }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
//
// ARGS_DOC. Field 3 in ARGP.
// A description of the non-option command-line arguments
// that are acceptable.
//
static char args_doc[] = "NUMBER";
static char doc[] = "\nNumberSlumber 2021 Richard Tjörnhammar LICENSE : GNUv3\n\n\tThis program is free software: you can redistribute it and/or modify\n\tit under the terms of the GNU General Public License as published by\n\tthe Free Software Foundation, either version 3 of the License, or\n\t(at your option) any later version.\n\n\tThis program is distributed in the hope that it will be useful,\n\tbut WITHOUT ANY WARRANTY; without even the implied warranty of\n\tMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\tGNU General Public License for more details.\n\tYou should have received a copy of the GNU General Public License\n\talong with this program.  If not, see <http://www.gnu.org/licenses/>\n" ;
static struct argp argp = {options, parse_opt, args_doc, doc};

int parse (int argc, char **argv, t_arguments pargs )
{
  // DEFAULTS
  pargs->outputbase = NULL;
  pargs->inputbase  = NULL;
  pargs->verbose =  0;
  pargs->ibase   = 10;
  pargs->obase   =  2;
  // PARSER
  argp_parse (&argp, argc, argv, 0, 0, pargs);
  return(0);
}

typedef struct nmr {
  // THE NUMBER REPRESENTATION
  char   *set; // THE CHARACTER SET
  int    base; // THE BASE
  char *value; // THE INPUT
  int    size; // THE NUM SIZE
  int was_allocd[3]; // IF ASSIGNED THEN DONT FREE
} a_number ;
typedef a_number *t_number;

void mirror_number_value(t_number number)
{
  int N = carrlen( number->value );
  int n = N/2;
  for(int i=0;i<n;i++)
  {
    char t = number->value[i];
    number->value[i] = number->value[N-1-i];
    number->value[N-1-i] = t;
  }
}

void append_value_to_number( char l , t_number number )
{
  int da = 20 ;
  if ( number -> value == NULL )
  {
      number -> value = (char*) calloc( da , sizeof(char) );
      number -> was_allocd[2] = da;
      number -> value[0] = '\0';
  }
  int curr_pos = carrlen ( number->value );
  if ( curr_pos+2 >= number-> was_allocd[2] )
  {
    number -> value = realloc( number -> value , number-> was_allocd[2] + da );
    number -> was_allocd[2] += da;
    if ( number -> value == NULL )
    {
      fprintf(stderr,"FAILED TO ALLOCATE MEMORY : DIES");
      exit(1);
    }
  }
  //
  // IF WE ARE HERE THEN EVERYTHING IS PEACHY
  number->value[curr_pos]   = l;
  number->value[curr_pos+1] = '\0';
}

void assign_base2number( t_number number , int base , char *basisset )
{
  int scz = 1 ; // sizeof char is 1
  number -> base = base;
  number -> set = (char*)calloc( number->base,scz );
  number -> was_allocd[1] = number->base ;
  if(basisset==NULL || carrlen(basisset)!=base)
    for(int i=0;i<base;i++)
      number->set[i] = i+SYMBOL_NULL_INDEX;
  else if( carrlen(basisset)==base )
    for(int i=0;i<base;i++)
      number->set[i] = basisset[i];
}

t_number create_number(void)
{
  t_number number;
  number = (a_number*) malloc(sizeof(a_number));
  number -> was_allocd[0] = 1;
  number -> was_allocd[1] = 0; // THE VALUE COULD BE SET AND NOT ALLOCATED !
  number -> was_allocd[2] = 0;
  // number -> value = NULL;
  return ( number );
}

void free_number(t_number number)
{
  if ( number -> was_allocd[2]>0 )
    free ( number->value );
  if ( number -> was_allocd[1]>0 )
    free ( number->set );
  if ( number -> was_allocd[0]>0 )
    free ( number );
}

void show_number(t_number number)
{
  fprintf ( stdout , "%s | %d %d [%s] \n" ,
	    number -> value ,
	    number -> size  ,
	    number -> base  ,
	    number -> set   ) ;
}

void print_base_character_set( int base )
{
    fprintf(stdout,"base%10d : ",base);
    for(int i=0; i<base;i++)
      fprintf(stdout,"%c",i+SYMBOL_NULL_INDEX);
    fprintf(stdout,"\n");
}
//
// THE FOLLOWING THREE FUNCTIONS ARE ONLY
// NEEDED BY THE NEANDERTHAL ALGORITHM
int get_order_of_character_in_base( char symbol , int base )
{
    for ( int i=0 ; i<base ; i++ )
      if ( symbol == i+SYMBOL_NULL_INDEX )
	return(i);
    return(-1);
}
int  gos( char s, int b )
{
  return ( get_order_of_character_in_base( s , b ) );
}

char sog( int i, int base )
{
  return ( i+SYMBOL_NULL_INDEX );
}
//
// THESE ARE MADE INTO FUNCTIONS TO SHOW
// THE RELATIONSHIP TO THE INITIAL IMPLEMENTATION
int  gos_char( char s, t_number number )
{
  for ( int i=0 ; i<number->base ; i++ )
    if ( s==number->set[i] )
      return ( i );
}
char sog_int( int i, t_number number )
{
  return ( number->set[i] );
}

void algo( char *argv , int bases[2], int bVerbose )
// NEANDERTHAL VERSION
{
  if ( bVerbose )
  {
    fprintf ( stdout , "GOT: %s\n " , argv ) ;
    fprintf ( stdout , "TRANSFORM: %d TO %d \n" , bases[0] , bases[1] ) ;
  }

  if ( 1 )
  {
    int J = 0;
    int finished = 0;
    if ( bVerbose )
    {
      fprintf ( stdout , "%s > " , argv );
    }
    while ( finished == 0 )
    {
      int done = 0 ;
      int i    = 0 ;
      char l   = '0';
      int lead = 1 ;
      while ( 1 )
      {
	char c = argv[i];
	if ( lead == 1 && c == SYMBOL_NULL_INDEX )
	{
	  i += 1;
	  continue;
	}
	if ( lead == 1 && c != SYMBOL_NULL_INDEX && c != '\0' )
	  lead = 0;
	if ( c == '\0' )
	{
	  if ( lead==1 )
	  {
	    finished = 1;
	    done = 1;
	  } else {
	    fprintf ( stdout,"%c",l );
	  }
	  break;
	}
	int num = gos( c , bases[0] ) + gos(l,bases[1])*bases[0] ;
	int mul = ( num/bases[1] ) ;
	l = sog( num - bases[1]*mul , bases[1] ) ;
	fprintf(stdout,"[%d]\n",l);
	argv[i] = mul + SYMBOL_NULL_INDEX ;
	fprintf(stdout,"[%d %d]\n",mul,SYMBOL_NULL_INDEX);
	fprintf(stdout,"(%s)",argv);
	if ( bVerbose )
	{
	  fprintf ( stdout,"[ %d ]",mul ) ;
	}
	i += 1 ;
	if ( mul == 0 )
	{
	  done = 1 ;
	}
      }
      J += 1;
    }
    fprintf ( stdout , "\n" );
  }
}

int conversion( t_number num_i, t_number num_o, int bVerbose )
{
  char *argv , bases[2]; 
  if ( bVerbose )
  {
    fprintf ( stdout , "GOT NUMBERS:\n " ) ;
    show_number ( num_i ) ;
    fprintf ( stdout , "WILL TRANSFORM FROM BASE %d TO BASE %d \n" , num_i->base , num_o->base ) ;
  }
  
  if ( 1 )
  {
    int J = 0;
    int finished = 0;
    if ( bVerbose )
    {
      fprintf ( stdout , "%s > " , argv );
    }
    while ( finished == 0 )
    {
      int done = 0 ;
      int i    = 0 ;
      char l   = '0';
      int lead = 1 ;
      while ( 1 )
      {
	char c = num_i->value[i];
	if ( lead == 1 && c == num_i->set[0] )
	{
	  i += 1;
	  continue;
	}
	if ( lead == 1 && c != num_i->set[0] && c != '\0' )
	  lead = 0;
	if ( c == '\0' )
	{
	  if ( lead==1 )
	  {
	    finished = 1;
	    done = 1;
	  } else {
	    if ( bVerbose )
	      fprintf ( stdout," {%c} ",l );	    
	    append_value_to_number( l , num_o );
	  }
	  break;
	}
	int num = gos_char( c , num_i ) + gos_char( l , num_o )*num_i->base ;
	int mul = ( num/num_o->base ) ;
	l = sog_int( num - num_o->base*mul , num_o );
	num_i->value[i] = mul + num_i->set[0]; // SHIFTS CHAR. MEMORY EFFICIENT BOOK KEEPING
	if ( bVerbose )
	{
	  fprintf ( stdout," %d ",mul ) ;
	}
	i += 1 ;
	if ( mul == 0 )
	{
	  done = 1 ;
	}
      }
      J += 1;
    }
    mirror_number_value(num_o);
  }
}

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

  t_number number_i , number_o ;
  
  number_i = create_number() ;
  number_o = create_number() ;

  number_i -> value  = pargs->args[0];
  number_i -> size   = carrlen(pargs->args[0]);

  assign_base2number ( number_i , bases[0] , set_in  );
  assign_base2number ( number_o , bases[1] , set_out );

  if ( bVerbose )
    show_number ( number_i );

  conversion ( number_i , number_o , bVerbose );

  fprintf(stdout,"%s\n",number_o->value);

  if ( bVerbose )
    show_number ( number_o );
  
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
