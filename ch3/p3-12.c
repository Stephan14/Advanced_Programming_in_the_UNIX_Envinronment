#include "apue.h"
#include "myerr.h"

#include <stdio.h>

void set_fl( int fd, int flags )
{
	int val;
	
	if( ( val = fcntl( fd, F_GETFL, 0 ) ) < 0 )
		err_sys("fcntl F_GETFL");
	
	val |= flags;
	
	if( fcntl( fd, F_SETFL, val ) < 0 )
		err_sys("fcntl F_SETFL error");
}

int 
main( int argc, char *argv[] )
{

	return 0;
}

