#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct abstr {
  short *i ;
  int size ;
  int n_elements ;
  int n_allocd ;
} abstract ;

typedef abstract *t_abstract;

void num2bits( long NUM , t_abstract pabstraction )
{
  for ( int k=0 ; k<pabstraction -> n_elements ; k++ )
  {
    long mask = long(1) << long(k);
    long masked_n = NUM & mask;
    short bitty = masked_n >> k;
    pabstraction -> i[k] = bitty;
  }
}

enum 	base10{ _0,_1,_2,_3,_4,_5,_6,_7,_8,_9 };
typedef enum base10 digit;
typedef digit *number;

int main ( int argc, char *argv[] )
// compile with: $ gcc -lm main.cc -o bitty
{
  if ( argc<2 )
    return ( 0 ) ;

  double fractional , integer;

  // this part is only here to test the idea of enumerated numbers
  number num = (digit*) malloc(3);
  num[0] = _1 ; num[1] = _1 ; num[2] = _0; // the enum representation of the number 110
  free( num ) ;
  // end test

  abstract        abstraction;
  t_abstract     pabstraction;
  pabstraction = &abstraction;

  fractional = modf ( log(atof(argv[1]))/log(2)+1. , &integer ) ;

  pabstraction -> n_elements 	= int ( integer ) ;
  pabstraction -> size 		= (int) sizeof(short)*int(integer) ;
  pabstraction -> i 		= (short*) malloc( pabstraction -> size );

  long NUM = long ( atof(argv[1]) ) ;
  num2bits ( NUM , pabstraction ) ;

  long sum = 0 ;
  long two = 1 ;
  for ( int k=0 ; k<pabstraction -> n_elements ; k++ )
  {
    sum    += two * pabstraction -> i[k] ;
    two    *= 2 ;
  }
  for ( int k = pabstraction -> n_elements-1; k>=0; k-- )
    printf ( "%d" , pabstraction -> i[k] ) ;

  printf ( "\t %ld " , sum ) ;
  printf ( "\t %f " , integer ) ;
  printf ( "\t %ld " , NUM ) ;

  free ( pabstraction->i ) ;
  printf ( "\n" ) ;

  return ( 0 ) ;
}
