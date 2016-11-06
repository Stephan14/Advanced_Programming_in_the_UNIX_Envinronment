#include "apue.h"

int 
main( int argc, char *argv[] )
{
	printf("hello word from process ID %d\n", (long)getpid() );//转换成长整型可以提高可移植性
	return 0;
}
