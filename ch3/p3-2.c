#include "apue.h"
#include "myerr.h"
#include <fcntl.h>

char buf1[] = "zbcdefghij";
char buf2[] = "ABCDEFGHIJ";

int 
main( int argc, char* argv[] )
{
	int fd;
	/*
	if( ( fd = creat( "file.jole", FILE_MODE ) ) < 0 )
		err_sys( "creat error" );
        */
	if( ( fd = open("file.jole", O_WRONLY | O_CREAT | O_TRUNC ) ) < 0 )
		err_sys("open error");
		
	if( write( fd, buf1, 10 ) != 10 )
		err_sys("buf1 write error\n");//当前的偏移量为10
	
	if( lseek( fd, 16384, SEEK_SET ) == -1 ) 
		err_sys("lseek error");//当前的偏移量为16384

	if( write( fd, buf2, 10 ) != 10 )
		err_sys("buf2 write error\n");//当前的偏移量为16394
	 
	return 0;
}
