#include "apue.h"
#include "myerr.h"

int 
main( int argc, char * argv[] )
{
	int c;
	while( (c = getc( stdin ) ) != EOF )
	{
		if( putc( c, stdout ) == EOF )
			err_sys("put error");
		if( ferror( stdin ) )
			err_sys("input error");
	}
	return 0;
}
