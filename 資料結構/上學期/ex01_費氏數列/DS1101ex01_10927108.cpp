// 10927108 劉珈彤

#include <iostream>
#include <math.h>
#include <time.h>
using namespace std ;

void factor( int ans, int smallfactor, long long time ) {
  if ( ans%smallfactor != 0 ) return factor( ans, smallfactor - 1, time + 1 ) ;
  else printf( "%llu * %llu (Inner loop: %d times)\n", smallfactor, ans/smallfactor, time ) ;
} // factor()

void sequence( int count, int walk, int ans, int firstnum, int secondnum, int smallfactor, long long time ) {
  if ( walk <= count ) {
  	ans = firstnum + secondnum ;
    printf( "[%d] %llu = ", walk, ans ) ; 
	secondnum = firstnum ;
	firstnum = ans ;
	
	smallfactor = sqrt( ans ) ;
	factor( ans, smallfactor, time ) ;
	
	sequence( count, walk + 1, ans, firstnum, secondnum, smallfactor, time ) ;
  } // if()

} // sequence()

void combination( int walk, int count, int digit, char str[10], long long &time ) {
  for( int i = walk ; i <= 10-count && count > 0 ; i++ ) {
  	str[digit-count] = i + '0' ;
    combination( i + 1, count - 1, digit, str, time ) ;
  } // for()
  
  if( count == 0 ) {
    printf( "[ %d] %s\n", time, str ) ;
    time++ ;
  } // if()

} // combination()

int main() {
  printf( "** Fibonacci Series Generator **\n" ) ;
  printf( "* 0. Quit                      *\n" ) ;
  printf( "* 1. Iterative generation      *\n" ) ;
  printf( "* 2. Recursive generation      *\n" ) ;
  printf( "* 3. Combination generator     *\n" ) ;
  printf( "********************************\n" ) ;
  printf( "Input a command(0, 1, 2, 3): " ) ;
  long long ans = 0, firstnum = 1, secondnum = 0 ;
  long long time = 1, smallfactor = 0, bigfactor = 0 ;
  int count = 0, type = 0, walk = 0 ;
  int digit = 0 ;
  char garbage[100] ; 
  char str[10] = { '\0' } ;
  if ( scanf( "%d", &type ) == 0 ) return 1 ;
    
  
  while ( type != 0 ) {
    while ( type > 3 || type < 0 ) { 
      printf( "Command does not exist!\n\n" ) ;
    
      printf( "** Fibonacci Series Generator **\n" ) ;
      printf( "* 0. Quit                      *\n" ) ;
      printf( "* 1. Iterative generation      *\n" ) ;
      printf( "* 2. Recursive generation      *\n" ) ;
      printf( "* 3. Combination generator     *\n" ) ;
      printf( "********************************\n" ) ;
      printf( "Input a command(0, 1, 2, 3): " ) ;

      if ( scanf( "%d", &type ) == 0 ) return 1 ;
    } // while()
  
    printf( "\nInput a number: " ) ;
    scanf( "%d", &count ) ;

    while ( count > 999999 || ( count < 0 || count > 11 ) ) {
      if ( ( type == 1 || type == 2 ) && count > 999999 )
        printf( "### The number must be in [1,999999] ###\n" ) ;
      else if ( type == 3 && ( count < 0 || count > 11 ) )
        printf( "### The number must be in [1,10] ###\n" ) ;
      printf( "\nInput a number: " ) ;
      scanf( "%d", &count ) ;
    } // while()
      
    if ( type == 1 ) {
      if ( count > 0 && count <= 92 ) {
  	    while ( walk < count ) {
  	      time = 1 ;
  	      ans = firstnum + secondnum ;
      
          smallfactor = sqrt ( ans ) ;
          while ( ans%smallfactor != 0 ) {
            smallfactor-- ;
            time++ ;
          } // while()
    
          bigfactor = ans/smallfactor ;
  	
  	      secondnum = firstnum ;
  	      firstnum = ans ;
  	      walk++ ;
  	      printf ( "[%d]%llu = %llu * %llu", walk, ans, smallfactor, bigfactor ) ;
  	      printf ( "( Inner loop : %d times )\n", time ) ;
   	    } // while()
	
	      printf ( "<Outer loop:  %d times>\n\n", count - 1 ) ;
      } // if()

    } // if()
    else if ( type == 2 ) {
      if ( count > 0 && count <= 92 ) {
        sequence( count, walk+1, ans, firstnum, secondnum, smallfactor, time ) ;
        printf ( "<Outer recursion:  %d times>\n\n", count ) ;
      } // if()
		
	    printf ( "<Outer recursion:  %d times>\n\n", count ) ;
	  } // else if()
    else if ( type == 3 ) {
      digit = count ;
      while ( walk <= 10-count ) {
      	str[0] = walk + '0' ;
        combination( walk + 1, count - 1, digit, str, time ) ;
        walk++ ;
	    } // while()
	  
      printf( "Recursion: %d ms\n", clock() ) ;
	  } // else if()

    ans = 0 ;
    firstnum = 1 ;
    secondnum = 0 ;
    time = 1 ;
    smallfactor = 0 ;
    bigfactor = 0 ;
    walk = 0 ;
    digit = 0 ;
    for( int i = 0 ; i < 10 ; i++ )
      str[i] = '\0' ;

    printf( "Input a command(0, 1, 2, 3): " ) ;
    if ( scanf( "%d", &type ) == 0 ) return 0 ;
    
  } // while()
  
  return 1 ;
} 
