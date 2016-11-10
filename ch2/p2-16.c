#include "apue.h"
#include "myerr.h"
#include <errno.h>
#include <limits.h>

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else 
static int pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;

#define PATH_MAX_GUESS 1024

char *
path_alloc( size_t *sizep )
{
	char *ptr;
	size_t size;

	if( posix_version == 0 )
		posix_version = sysconf( _SC_VERSION );
	if( xsi_version == 0 )
		xsi_version = sysconf( _SC_XOPEN_VERSION );
	
	if( pathmax == 0 )
	{
		errno = 0;
		if( ( pathmax = pathconf("/", _PC_PATH_MAX) ) < 0 )
		{
			if( pathmax == 0 )
				pathmax = PATH_MAX_GUESS;
			else
				err_sys("pathconf error for _PC_PATH_MAX");
		}
		else
			pathmax++;//相对于根路径
        }
	
	if( posix_version < 200112L && xsi_version < 4 )
		size = pathmax + 1;
	else 
		size = pathmax;
		 
	if( ( ptr = malloc( size ) ) == NULL )
		err_sys("malloc error for pathmax");
	if( sizep != NULL )//判断是否为空指针
		*sizep = size;
	
	return ptr;
}
int 
main( int argc, char * argv[] )
{
	return 0;
}


