#include "apue.h"
#include "myerr.h"

#include <fcntl.h>// for open()
#include <unistd.h>//for lseek()

int 
main( int argc, char *argv[] )
{
	int fd;
	char buf[10] = "absdefghij";
	char readbuf[10];

	if( ( fd = open("test", O_RDWR|O_APPEND ) ) < 0 )
	{
		err_sys("open error");
		return -1;
	}

	lseek( fd, 10, SEEK_SET );
	
	if( write( fd, buf, 10) < 0 )
	{
		err_sys("wirte error");
		return -1;
	}

	lseek( fd , 5, SEEK_SET );
	if( read( fd, readbuf, 10 ) < 0 )
	{
		err_sys("read error");
		return -1;
	}	

	printf("%s", readbuf );	
	return 0;

}
