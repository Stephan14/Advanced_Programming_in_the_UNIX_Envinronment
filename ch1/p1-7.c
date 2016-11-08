#include "apue.h"
#include "myerr.h"

int 
main( int argc, char * argv[])
{
	printf("uid = %d. gid = %d", getuid(), getpid() );
	return 0;
}
