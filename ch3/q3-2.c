#include "apue.h"
#include "myerr.h"

int mydup2( int fd, int fd2 )
{
	if( fd < 0 || fd > 256 )
	{
		err_sys("fd error\n");
		return -1;
	}

	if( fd2 < 0 || fd2 > 256 )
	{
		err_sys("fd2 error\n");
		return -1;
	}
	
	int index = 0;
	int fdarray[ fd2 ];
	//
	if( ( fdarray[index] = dup( fd )) == -1 )
	{
		err_sys("dup %d error \n", fd );
		return -1;
	}
	else
		close( fdarray[ index ] );

	if( fd == fd2 )
		return fd;
	
	close( fd2 );
	
	int newfdindex;

	for( index = 0; index <= fd2; index++ )
	{
		fdarray[ index ] = dup( fd );
		if( fdarray[ index ] == -1 )
		{
			err_sys("dup %d error\n", fdarray[ index ] );
			return -1;			
		}
		else
		{
			if(  fdarray[ index ] == fd2 )
			{
				newfdindex = index;
				break;
			}
		}
	}

	for( index = 0; index < newfdindex; index++ )
		close( fdarray[ index ] );

	return fdarray[ newfdindex ];
}
int 
main( int argc, char * argv)
{
	printf("%d %d", argv[1], argv[2]);
	printf("%d", mydup2( argv[1], argv[2] ) );
	return 0;
}
