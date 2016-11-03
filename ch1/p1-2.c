#include "apue.h"
#include "myerr.h"

#define BUFSIZE 4096

int 
main( int argc, char *argv[] )
{
	char buf[BUFSIZE];
	int n;
	
	while( ( n =  read( STDIN_FILENO, buf, BUFSIZE ) ) > 0 )
		//read的返回值n为write所使用
		if( write( STDOUT_FILENO, buf, n ) != n )
			err_sys("write error");
	
	if( n < 0 )
		err_sys("read error");
 
	return 0;
}
