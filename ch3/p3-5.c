#include "apue.h"
#include "myerr.h"
#include <stdio.h>

#define BUFFERSIZE 4096
int 
main( int argc, char *argv[] )
{
	char buf[ BUFFERSIZE ];
	int n;

	while( ( n = read( STDIN_FILENO,  buf, BUFFERSIZE ) ) > 0 )
		if( write( STDOUT_FILENO, buf, n) != n )
			err_sys("write error");
	
	if( n < 0 )
		err_sys("read error");

	return 0;
}
