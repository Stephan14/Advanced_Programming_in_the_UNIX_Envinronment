#include "apue.h"
#include "myerr.h"
/*测试对标准输入是否可以设置偏移量*/
int 
main( int argc, char* argv[] )
{
	if( lseek ( STDIN_FILENO , 0, SEEK_CUR ) == -1 )
		printf("cannot seek\n");
	else
		printf("seek OK\n");
	
	return 0;
}
