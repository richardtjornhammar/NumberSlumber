#include "symbols.h"
#include "numbers.h"

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
  number->size = curr_pos+1 ;
}

void assign_base2number( t_number number , int base , char *basisset )
{
  if (number->set !=NULL )
    fatal("CANNOT ALLOCATE ALREADY DEFINED SET");
  int scz = 1 ; // sizeof char is 1
  number -> base = base;
  number -> set = (char*)calloc( number->base+1 , scz );
  number -> was_allocd[1] = number->base+1 ;
  if(basisset==NULL || carrlen(basisset)!=base)
    for(int i=0;i<base;i++)
      number->set[i] = i+SYMBOL_NULL_INDEX;
  else if( carrlen(basisset)==base )
    for(int i=0;i<base;i++)
      number->set[i] = basisset[i];
  number->set[number -> was_allocd[1]-1]='\0';
}

t_number create_number(void)
{
  t_number number;
  number = (a_number*) malloc(sizeof(a_number));
  number -> was_allocd[0] = 1;
  number -> was_allocd[1] = 0;
  // THE VALUE COULD BE SET AND NOT ALLOCATED !
  number -> was_allocd[2] = 0;
  return ( number );
}

t_number instantiate_number ( char *value , char *symbols )
{
  // NOT DONE
  t_number number ;
  int nb,nn;
  nb = carrlen ( symbols ) ;
  nn = carrlen ( value ) ;
  number = create_number() ;
  assign_base2number ( number, nb, symbols );
  for ( int i=0 ; i<nn ; i++ )
    append_value_to_number ( value[i] , number );
  return ( number ) ;
}

void free_number ( t_number number )
{
  if ( number -> was_allocd[2]>0 )
    free ( number->value );
  if ( number -> was_allocd[1]>0 )
    free ( number->set );
  if ( number -> was_allocd[0]>0 )
    free ( number );
}

void show_number ( t_number number )
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

int size_of_value ( t_number num )
{
  int n ;
  n = carrlen ( num->value );
  num -> size = n ;
  return ( n ) ;
}

void copy_value_to_carr( t_number num , char carr[] )
{
  int m = carrlen(carr);
  if ( num->size!=m )
  {
    fprintf(stdout,"FATAL ERROR N2C : COULD NOT COPY NUMBER \n");
    exit(1);
  }
  for ( int i=0 ; i<m ; i++ )
  {
    carr[i] = num->value[i];
  }
  carr[m]='\0';
}

void copy_carr_to_value(  char carr[] , t_number num )
{
  int m = carrlen(carr);
  if ( num->size!=m )
  {
    fprintf(stdout,"FATAL ERROR C2N : COULD NOT COPY NUMBER \n");
    exit(1);
  }
  if ( num->was_allocd[2] == 0 )
  {
    for ( int i=0 ; i<m ; i++ )
      append_value_to_number ( carr[i] ,  num );
  }
  if ( num->was_allocd[2]>0 )
  {
    for ( int i=0 ; i<m ; i++ )
    {
      num->value[i] = carr[i] ;
    }
    num->value[m] = '\0' ;
  }
  num->size = carrlen ( num->value );
}

int conversion ( t_number num_i , t_number num_o , int bVerbose )
{
  char *argv , bases[2];
  //
  // CREATES A STORE OF
  // THE ORIGINAL INPUT
  int  nc = size_of_value( num_i );
  char tmp_carr[ nc ];
  for ( int i=0 ; i<=nc ; i++ )
    tmp_carr[i] = ' ' ;
  tmp_carr[nc] = '\0' ;
  copy_value_to_carr( num_i , tmp_carr );
  // DONE

  if ( bVerbose )
  {
    fprintf ( stdout , "GOT NUMBERS:\n" ) ;
    show_number ( num_i ) ;
    fprintf ( stdout , "WILL TRANSFORM FROM BASE %d TO BASE %d \n" , num_i->base , num_o->base ) ;
  }
  if ( 1 )
  {
    int J = 0;
    int finished = 0;
    if ( bVerbose )
    {
      fprintf ( stdout , "%s > " , num_i->value );
    }
    while ( finished == 0 )
    {
      int done = 0 ;
      int  i   = 0 ;
      char l   = num_o->set[0];
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
	    finished = 1 ;
	    done = 1 ;
	  } else {
	    if ( bVerbose )
	      fprintf ( stdout," {%c} ",l ) ;
	    append_value_to_number( l , num_o ) ;
	  }
	  break;
	}
	int num = gos_char( c,num_i ) + gos_char( l,num_o )*num_i->base ;
	int mul = ( num/num_o->base ) ;
	l = sog_int( num - num_o->base*mul , num_o );
	num_i -> value[i] = num_i->set[mul];
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
    if ( num_o->was_allocd[2]==0 )
    {
      append_value_to_number( num_o->set[0] , num_o ) ;
    }
    mirror_number_value(num_o);
  }
  copy_carr_to_value( tmp_carr , num_i ); // copy back
}

char xor ( char b_1 , char b_2 , char base[2] )
{
  int b1 , b2 ;
  char v[ 2 ] = { base[0],base[1] };
  b1 = b_1 == v[1] ;
  b2 = b_2 == v[1] ;
  return ( v[ ( (b1 || b2) - (b1 && b2) ) ] );
}

char and ( char b_1 , char b_2 , char base[2] )
{
  int b1 , b2 ;
  char v[ 2 ] = { base[0],base[1] };
  b1 = b_1 == v[1] ;
  b2 = b_2 == v[1] ;
  return ( v[ b1 && b2 ] );
}

char or ( char b_1 , char b_2 , char base[2] )
{
  int b1 , b2 ;
  char v[ 2 ] = { base[0],base[1] };
  b1 = b_1 == v[1] ;
  b2 = b_2 == v[1] ;
  return ( v[ b1 || b2 ] );
}

t_number copy_number( t_number num_o )
{
    t_number number_r;
    number_r = create_number();
    assign_base2number ( number_r , num_o->base , num_o->set );
    for ( int i=0 ; i<num_o->size ; i++ )
        append_value_to_number ( num_o->value[i] ,number_r ) ;
    number_r->size = num_o->size;
    return ( number_r );
}

t_number add_numbers ( t_number num_a , t_number num_b , int bVerbose )
{
  // WE DEFINE A CALCULATION BASE
  int  nbase = 2 ;
  char base[ 3 ] = {'0','1','\0'};
  t_number number_u, number_d, number_o ;

  number_o = create_number();
  assign_base2number ( number_o , nbase , base );

  if ( num_a->was_allocd[2]>0 && num_b->was_allocd[2]>0 )
  {
    if ( num_a->base!=2 )
    {
      number_u = create_number() ;
      assign_base2number ( number_u , nbase , base );
      conversion ( num_a , number_u , bVerbose );
      number_u -> size = carrlen ( number_u->value );
    } else {
      number_u = num_a ;
    }
    if(bVerbose)
      show_number ( number_u ) ;

    if ( num_b->base!=2 )
    {
      number_d = create_number() ;
      assign_base2number ( number_d , nbase , base );
      conversion ( num_b , number_d , bVerbose );
      number_d -> size = carrlen ( number_d->value );
    } else {
      number_d = num_b;
    }
    if(bVerbose)
      show_number ( number_d );
  }
  int N , M , n, m;
  t_number larger_number,smaller_number;
  N = number_u->size;
  M = number_d->size;
  n = N>M?M:N ;
  m = N>M?N:M ;
  larger_number = N>=M?number_u:number_d;
  smaller_number= N>=M?number_d:number_u;
  //
  // DOING BINARY BITWISE ADDITION
  {
    // THIS IS A LOCAL SCOPE
    char vu0 = base[0] , vd0 = base[0] ;
    int   i0 = 0 , j0 =  0 , r0 = 0;
    for ( int i = 0 ; i <= m ; i++ )
    {
      i0  = (m-1-i) ;
      j0  = (n-1-i) ;
      int bi = i0<m&&i0>=0 ? larger_number->value[i0]==base[1] : 0;
      int bj = j0<n&&j0>=0 ? smaller_number->value[j0]==base[1] : 0;
      int bn = bi+bj+r0;
      vu0 = base[ bn%2 ];
      r0  = bn>=2;
      append_value_to_number( vu0 , number_o ) ;
    }
    if ( r0==1 ) // THE FINAL BIT MIGHT BE ONE BIT LARGER THAN THE TWO OTHER NUMBERS
      append_value_to_number ( base[1] ,number_o ) ;
  }
  mirror_number_value ( number_o );
  if ( bVerbose )
  {
    fprintf ( stdout , "HAD : %s\n" , number_u -> value ) ;
    fprintf ( stdout , "HAD : %s\n" , number_d -> value ) ;
    fprintf ( stdout , "RES : %s\n" , number_o -> value ) ;
  }

  if ( num_a->base>2 ) // CONVERT BACK TO THE USER INPUT BASE
  {
    t_number number_r;
    number_r = create_number();
    assign_base2number ( number_r , num_a->base , num_a->set );
    conversion ( number_o , number_r , bVerbose );
    free_number( number_o );
    return ( number_r ) ;
  }
  //
  // RETURNS A POINTER TO THE ALLOCATED NUMBER CONTAINING THE RESULT
  return ( number_o );
}

t_number multiply_numbers ( t_number num_a , t_number num_b , int bVerbose )
{
  // WE DEFINE A CALCULATION BASE
  int  nbase = 2 ;
  char base[ 3 ] = {'0','1','\0'};
  t_number number_u, number_d, number_o ;

  number_o = create_number();
  assign_base2number ( number_o , nbase , base );

  if ( num_a->was_allocd[2]>0 && num_b->was_allocd[2]>0 )
  {
    if ( num_a->base!=2 )
    {
      number_u = create_number() ;
      assign_base2number ( number_u , nbase , base );
      conversion ( num_a , number_u , bVerbose );
      number_u -> size = carrlen ( number_u->value );
    } else {
      number_u = num_a ;
    }
    if ( bVerbose )
      show_number ( number_u ) ;

    if ( num_b->base!=2 )
    {
      number_d = create_number() ;
      assign_base2number ( number_d , nbase , base );
      conversion ( num_b , number_d , bVerbose );
      number_d -> size = carrlen ( number_d->value );
    } else {
      number_d = num_b;
    }
    if(bVerbose)
      show_number ( number_d );
  }
  int N , M , n, m;
  t_number larger_number,smaller_number;
  N = number_u->size;
  M = number_d->size;
  n = N>M?M:N ;
  m = N>M?N:M ;
  larger_number = N>=M?number_u:number_d;
  smaller_number= N>=M?number_d:number_u;
  t_number number_temp , number_res ;
  {
    // THIS IS A LOCAL SCOPE
    char vu0 = base[0] , vd0 = base[0] ;
    int   i0 = 0 , j0 =  0 , r0 = 0 ;
    number_temp = copy_number( larger_number );
    int p0  = 0 ;
    int cnt = 0 ;
    for ( int i = 0 ; i < n ; i++ )
    {
      if ( smaller_number->value[n-i-1] == base[0] )
      {
          cnt++ ;
	  append_value_to_number( base[0] , number_temp ) ;
      } else {
	if ( number_res->size<1 )
	{
	    number_res = copy_number ( number_temp );
	}
	else
	{
	    t_number tmp;
	    tmp = add_numbers ( number_temp , number_res , bVerbose );
            number_res = copy_number ( number_temp );
	}
      }
    }
  }
  free(number_o);
  number_o = copy_number ( number_res );
  if ( bVerbose )
  {
    fprintf ( stdout , "HAD : %s\n" , number_u -> value ) ;
    fprintf ( stdout , "HAD : %s\n" , number_d -> value ) ;
    fprintf ( stdout , "RES : %s\n" , number_o -> value ) ;
  }

  if ( num_a->base>2 ) // CONVERT BACK TO THE USER INPUT BASE
  {
    t_number number_r;
    number_r = create_number();
    assign_base2number ( number_r , num_a->base , num_a->set );
    conversion ( number_o , number_r , bVerbose );
    free_number( number_o );
    return ( number_r ) ;
  }
  //
  // RETURNS A POINTER TO THE ALLOCATED NUMBER CONTAINING THE RESULT
  return ( number_o );
}
