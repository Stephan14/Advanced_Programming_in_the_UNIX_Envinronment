#include "apue.h"
#include "myerr.h"
#include <stdio.h>

int main( int argc, char* argv[] )
{

	struct stat buf;
	int i;
	char *ptr;

	for( i = 1; i < argc; i++ )
	{
		printf("%s : ", argv[ i ]);
		if( lstat( argv[i], &buf ) < 0 )
		{
			err_sys("lstat error\n");
			continue;
		}

		if( S_ISREG( buf.st_mode ) )
			ptr = "reguar";
		else if( S_ISDIR( buf.st_mode ) )
			ptr = "dirstory";
		else if( S_ISCHR( buf.st_mode ) )
			ptr = "charactor special";
		else if( S_ISBLK( buf.st_mode ) )
			ptr = "block special";
		else if( S_ISFIFO( buf.st_mode ) )
			ptr = "fifo";
		else if( S_ISLNK( buf.st_mode ) )
			ptr = "symbol link";
		else if( S_ISSOCK( buf.st_mode ) )
			ptr = "socket";
		else 
			ptr = "unkown mode";
		
		printf("%s\n", ptr );
	}
	return 0;
}
