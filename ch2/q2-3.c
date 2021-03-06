#include "apue.h"
#include "myerr.h"
#include <limits.h>
#include <sys/resource.h>

#define OPEN_MAX_GUESS 256
/*
struct rlimit
{        
   rlim_t rlim_cur;        
   rlim_t rlim_max;
};
结构体中 rlim_cur是要取得或设置的资源软限制的值，rlim_max是硬限制

*/
long 
open_max( void )
{
	long openmax;
	//可以在/usr/include/x86_64-linux-gnu/bits/resource.h头文件中查看struct rlimit的定义
	struct rlimit rl;

	if( (openmax = sysconf( _SC_OPEN_MAX )) < 0 || openmax == LONG_MAX )
	{
		if( getrlimit( RLIMIT_NOFILE, &rl ) < 0 )//RLIMIT_NOFILE:一个进程能打开的最大文件数，内核默认是1024
//每个进程都有一组资源限制，其中某一些可以用getrlimit和setrlimit函数查询和更改。
			err_sys("can't get file limit");
		if( rl.rlim_max == RLIM_INFINITY )
			openmax = OPEN_MAX_GUESS;
		else
			openmax = rl.rlim_max; 
	}

	return openmax;
}
int 
main( int argc, char *argv[] )
{
	printf("openmax = %ld\n ", open_max() );
	return 0;
}
