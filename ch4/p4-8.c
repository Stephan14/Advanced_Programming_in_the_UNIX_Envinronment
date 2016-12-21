#include <fcntl.h>
#include "apue.h"
#include "myerr.h"

int 
main( int argc, char* argv[] )
{
	if (argc != 2)
		err_quit("usage: p4-8 <pathname>");		
	
	if (access(argv[1], R_OK) < 0)
		err_ret("access error for %s", argv[1]);
	else 
		printf("read access OK");

	if (open(argv[1], O_RDONLY) < 0 )
		err_ret("open error for %s", argv[1]);
	else
		printf("open for read OK");
	return 0;
}

